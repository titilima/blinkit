// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_document.cpp
// Description: Bindings for Document
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

static JSValue GetBody(JSContext *ctx, JSValueConst thisVal)
{
    ASSERT(false); // BKTODO:
    return JS_UNDEFINED;
}

#ifdef BLINKIT_CRAWLER_ENABLED
JSValue CreateDocumentPrototypeForCrawler(JSContext *ctx, JSValue containerNodePrototype)
{
    static const JSCFunctionListEntry Funcs[] = {
        CGetSetEntry("body", GetBody, nullptr)
    };

    JSValue ret = JS_NewObjectProto(ctx, containerNodePrototype);
    JS_SetPropertyFunctionList(ctx, ret, Funcs, std::size(Funcs));
    return ret;
}
#endif

} // namespace qjs
} // namespace BlinKit
