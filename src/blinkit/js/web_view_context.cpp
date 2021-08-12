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

#include "blinkit/blink/renderer/bindings/core/duk/duk_image_element.h"

using namespace blink;

namespace BlinKit {

WebViewContext::WebViewContext(LocalFrame &frame)
    : ScriptController(frame, DukElement::PrototypeMapForUI())
{
}

} // namespace BlinKit
