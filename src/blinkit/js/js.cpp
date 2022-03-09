// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: js.cpp
// Description: JS APIs
//      Author: Ziming Li
//     Created: 2022-03-07
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "bk_js.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_bindings.h"
#include "blinkit/js/runtime.h"

using namespace BlinKit;

extern "C" {

BKEXPORT BkJSContext BKAPI BkCreateJSContext(BkJSRuntime runtime, unsigned featureFlags)
{
    JSContext *ret;

    if (nullptr == runtime)
        runtime = g_runtime;

    ret = JS_NewContext(g_runtime);
    if (nullptr != ret && 0 != featureFlags)
    {
        JSValue global = JS_GetGlobalObject(ret);

        if (BK_CTX_CONSOLE & featureFlags)
            qjs::AddConsole(ret, global);

        JS_FreeValue(ret, global);
    }
    return ret;
}

BKEXPORT int BKAPI BkEvaluate(BkJSContext ctx, const char *code, unsigned len, BkBuffer *ret)
{
    int r = BK_ERR_SUCCESS;

    if (0 == len)
        len = strlen(code);

    JSValue v = JS_Eval(ctx, code, len, "eval", JS_EVAL_TYPE_GLOBAL);
    if (JS_EXCEPTION != v)
    {
        if (JS_UNDEFINED != v)
        {
            std::string s;
            qjs::Context context(*ctx);
            if (context.ToString(s, v))
                BkSetBufferData(ret, s.data(), s.length());
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

BKEXPORT void BKAPI BkReleaseJSContext(BkJSContext ctx)
{
    JS_FreeContext(ctx);
}

} // extern "C"
