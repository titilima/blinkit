// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: context_impl.cpp
// Description: ContextImpl Class
//      Author: Ziming Li
//     Created: 2022-03-13
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./context_impl.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_bindings.h"
#include "blinkit/js/runtime.h"

using namespace BlinKit;

ContextImpl::ContextImpl(JSContext *ctx, unsigned features)
    : m_ctx(ctx)
    , m_consoleEnabled(0 != (BK_CTX_CONSOLE & features))
{
    using namespace qjs;

    JS_SetContextOpaque(ctx, this);

    JSValue global = JS_GetGlobalObject(m_ctx);
    if (m_consoleEnabled)
        AddConsole(m_ctx, global);
    JS_FreeValue(m_ctx, global);
}

ContextImpl::~ContextImpl(void)
{
    if (nullptr != m_ctx)
        JS_FreeContext(m_ctx);
}

int ContextImpl::Evaluate(const std::string &code, std::string &ret)
{
    int r = BK_ERR_SUCCESS;

    JSValue v = JS_Eval(m_ctx, code.c_str(), code.length(), "eval", JS_EVAL_TYPE_GLOBAL);
    if (!JS_IsException(v))
    {
        if (!JS_IsUndefined(v))
        {
            qjs::Context context(*m_ctx);
            if (!context.ToString(ret, v))
                r = BK_ERR_TYPE;
        }
    }
    else
    {
        r = BK_ERR_EXCEPTION;
    }
    JS_FreeValue(m_ctx, v);

    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkEvaluate(BkContext ctx, const char *code, unsigned len, BkBuffer *ret)
{
    int r;

    if (0 == len)
        len = strlen(code);

    std::string s;
    r = ctx->Evaluate(std::string(code, len), s);
    if (BK_ERR_SUCCESS == r)
        BkSetBufferData(ret, s.data(), s.length());

    return r;
}

BKEXPORT BkJSContext BKAPI BkGetJSContext(BkContext ctx)
{
    return ctx->GetJSContext();
}

BKEXPORT void BKAPI BkReleaseContext(BkContext ctx)
{
    delete ctx;
}

BKEXPORT BkContext BKAPI BkWrapContext(BkJSContext jsCtx, unsigned features)
{
    if (nullptr == jsCtx)
        jsCtx = JS_NewContext(g_runtime);
    else if (nullptr != JS_GetContextOpaque(jsCtx))
        return nullptr; // May be wrapped by some one else.
    return new ContextImpl(jsCtx, features);
}

} // extern "C"
