// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_container_node.cpp
// Description: Bindings for ContainerNode
//      Author: Ziming Li
//     Created: 2022-03-08
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./qjs_bindings.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_c_function_list_entries.h"
#include "blinkit/blink/renderer/bindings/core/script_controller.h"
#include "blinkit/blink/renderer/core/dom/container_node.h"

namespace BlinKit {
namespace qjs {

static JSValue GetFirstChild(JSContext *ctx, JSValueConst thisVal)
{
    ContainerNode *node = ScriptWrappable::Cast<ContainerNode>(thisVal);
    return ScriptController::From(ctx)->ReturnNode(node->firstChild());
}

static JSValue QuerySelector(JSContext *ctx, JSValueConst thisVal, int argc, JSValueConst *argv)
{
    if (!JS_IsString(argv[0]))
        return JS_EXCEPTION;

    ContainerNode *node = ScriptWrappable::Cast<ContainerNode>(thisVal);

    Context context(*ctx);
    Element *ret = node->querySelector(ToAtomicString(ctx, argv[0]), context);
    if (context.hadException())
        return context.ReturnException();

    return ScriptController::From(ctx)->ReturnElement(ret);
}

#ifdef BLINKIT_CRAWLER_ENABLED
JSValue CreateContainerNodePrototypeForCrawler(JSContext *ctx, JSValue nodePrototype)
{
    static const JSCFunctionListEntry Funcs[] = {
        CGetSetEntry("firstChild", GetFirstChild, nullptr),
        CFunctionEntry("querySelector", 1, QuerySelector)
    };

    JSValue ret = JS_NewObjectProto(ctx, nodePrototype);
    JS_SetPropertyFunctionList(ctx, ret, Funcs, std::size(Funcs));
    return ret;
}
#endif

} // namespace qjs
} // namespace BlinKit
