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

extern "C" {

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
            BlinKit::qjs::Context context(*ctx);
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

} // extern "C"
