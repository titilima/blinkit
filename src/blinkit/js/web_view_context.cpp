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
#include "blinkit/js/runtime.h"

namespace BlinKit {

WebViewContext::WebViewContext(LocalFrame &frame) : ScriptController(frame, g_runtime)
{
}

void WebViewContext::OnContextCreated(JSContext *ctx, JSValue global, Prototypes &prototypes)
{
    using namespace qjs;

    JSValue eventTargetPrototype = CreateEventTargetPrototype(ctx);

    prototypes.window = CreateWindowPrototypeForUI(ctx);

    JS_FreeValue(ctx, eventTargetPrototype);
}

} // namespace BlinKit
