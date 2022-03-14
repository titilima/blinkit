// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_context.cpp
// Description: WebViewContext Class
//      Author: Ziming Li
//     Created: 2022-03-03
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./web_view_context.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs_bindings.h"

namespace BlinKit {

WebViewContext::WebViewContext(LocalFrame &frame) : ScriptController(frame)
{
}

void WebViewContext::OnContextCreated(JSContext *ctx, JSValue global, Prototypes &prototypes)
{
    using namespace qjs;

    prototypes.prototypes[PROTO_Window] = CreateWindowPrototypeForUI(ctx);

    JSValue protoEventTarget = CreateEventTargetPrototype(ctx);
    RegisterEventTargetPrototypes(ctx, prototypes, protoEventTarget);
    JS_FreeValue(ctx, protoEventTarget);

    prototypes.prototypes[PROTO_Location] = CreateLocationPrototypeForUI(ctx);
}

void WebViewContext::RegisterEventTargetPrototypes(JSContext *ctx, Prototypes &dst, JSValue protoEventTarget)
{
    ASSERT(false); // BKTODO:
}

} // namespace BlinKit
