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

ContextImpl::ContextImpl(unsigned features)
    : m_ctx(JS_NewContext(g_runtime))
    , m_consoleEnabled(0 != (BK_CTX_CONSOLE & features))
{
    using namespace qjs;

    if (0 == features)
        return;

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

BKEXPORT BkContext BKAPI BkCreateContext(unsigned features)
{
    return new ContextImpl(features);
}

BKEXPORT int BKAPI BkEvaluate(BkJSContext ctx, const char *code, unsigned len, BkBuffer *ret)
{
    int r = BK_ERR_SUCCESS;

    if (0 == len)
        len = strlen(code);

    std::string input(code, len);
    JSValue v = JS_Eval(ctx, input.c_str(), input.length(), "eval", JS_EVAL_TYPE_GLOBAL);
    if (!JS_IsException(v))
    {
        if (!JS_IsUndefined(v))
        {
            std::string output;
            qjs::Context context(*ctx);
            if (context.ToString(output, v))
                BkSetBufferData(ret, output.data(), output.length());
            else
                r = BK_ERR_TYPE;
        }
    }
    else
    {
        r = BK_ERR_EXCEPTION;
    }
    JS_FreeValue(ctx, v);

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

} // extern "C"
