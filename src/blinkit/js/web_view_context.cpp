// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_context.cpp
// Description: WebViewContext Class
//      Author: Ziming Li
//     Created: 2021-08-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./web_view_context.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk_attr.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_console.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_document.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_dom_token_list.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_event.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_location.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_navigator.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_script_element.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_window.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_xhr.h"

using namespace blink;

namespace BlinKit {

WebViewContext::WebViewContext(LocalFrame &frame)
    : ScriptController(frame, DukElement::PrototypeMapForUI())
{
}

void WebViewContext::Attach(duk_context *ctx, duk_idx_t globalStashIndex)
{
    ScriptController::Attach(ctx, globalStashIndex);
    RegisterPrototypes(ctx, globalStashIndex);
}

void WebViewContext::RegisterPrototypes(duk_context *ctx, duk_idx_t globalStashIndex)
{
    PrototypeHelper helper(ctx, globalStashIndex);
    DukAttr::RegisterPrototype(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::Comment);
    DukConsole::RegisterPrototype(helper);
    DukDOMTokenList::RegisterPrototype(helper);
    DukUIDocument::RegisterPrototype(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::DocumentFragment);
    DukElement::RegisterPrototypeForUI(helper);
    DukEvent::RegisterPrototype(helper);
    DukLocation::RegisterPrototype(helper);
    DukNavigator::RegisterPrototype(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::DocumentType);
    DukNode::RegisterPrototype(helper, ProtoNames::Text);
    DukWindow::RegisterPrototypeForUI(helper);
    DukXHR::RegisterPrototype(helper);
}

} // namespace BlinKit
