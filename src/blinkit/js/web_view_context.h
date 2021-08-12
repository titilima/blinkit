#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_context.h
// Description: WebViewContext Class
//      Author: Ziming Li
//     Created: 2021-08-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WEB_VIEW_CONTEXT_H
#define BLINKIT_BLINKIT_WEB_VIEW_CONTEXT_H

#include "blinkit/blink/renderer/bindings/core/duk/script_controller.h"

namespace BlinKit {

class WebViewContext final : public blink::ScriptController
{
public:
    WebViewContext(blink::LocalFrame &frame);
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WEB_VIEW_CONTEXT_H
