// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_element.cpp
// Description: Bindings for Element
//      Author: Ziming Li
//     Created: 2022-03-08
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./qjs_bindings.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_c_function_list_entries.h"

namespace BlinKit {
namespace qjs {

static JSValue GetAttribute(JSContext *ctx, JSValueConst thisVal, int argc, JSValueConst *argv)
{
    ASSERT(false); // BKTODO:
    return JS_UNDEFINED;
}

#ifdef BLINKIT_CRAWLER_ENABLED
JSValue CreateElementPrototypeForCrawler(JSContext *ctx, JSValue containerNodePrototype)
{
    static const JSCFunctionListEntry Funcs[] = {
        CFunctionEntry("getAttribute", 1, GetAttribute)
    };

    JSValue ret = JS_NewObjectProto(ctx, containerNodePrototype);
    JS_SetPropertyFunctionList(ctx, ret, Funcs, std::size(Funcs));
    return ret;
}
#endif

} // namespace qjs
} // namespace BlinKit
