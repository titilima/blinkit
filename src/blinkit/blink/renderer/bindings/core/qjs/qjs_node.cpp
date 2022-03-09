// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_node.cpp
// Description: Bindings for Node
//      Author: Ziming Li
//     Created: 2022-03-08
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./qjs_bindings.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_c_function_list_entries.h"
#include "blinkit/blink/renderer/core/dom/node.h"

namespace BlinKit {
namespace qjs {

static JSValue GetNodeType(JSContext *ctx, JSValueConst thisVal)
{
    Node *node = ScriptWrappable::Cast<Node>(thisVal);
    return JS_NewInt32(ctx, node->nodeType());
}

static JSValue GetTextContent(JSContext *ctx, JSValueConst thisVal)
{
    Node *node = ScriptWrappable::Cast<Node>(thisVal);
    return ReturnString(ctx, node->textContent());
}

static JSValue SetTextContent(JSContext *ctx, JSValueConst thisVal, JSValueConst textContent)
{
    Node *node = ScriptWrappable::Cast<Node>(thisVal);
    ASSERT(false); // BKTODO:
    return JS_UNDEFINED;
}

#ifdef BLINKIT_CRAWLER_ENABLED
JSValue CreateNodePrototypeForCrawler(JSContext *ctx, JSValue eventTargetPrototype)
{
    static const JSCFunctionListEntry Funcs[] = {
        CGetSetEntry("nodeType", GetNodeType, nullptr),
        CGetSetEntry("textContent", GetTextContent, SetTextContent)
    };

    JSValue ret = JS_NewObjectProto(ctx, eventTargetPrototype);
    JS_SetPropertyFunctionList(ctx, ret, Funcs, std::size(Funcs));
    return ret;
}
#endif

} // namespace qjs
} // namespace BlinKit
