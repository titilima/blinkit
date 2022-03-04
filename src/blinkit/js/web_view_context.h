#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_context.h
// Description: WebViewContext Class
//      Author: Ziming Li
//     Created: 2022-03-03
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_WEB_VIEW_CONTEXT_H
#define BLINKIT_WEB_VIEW_CONTEXT_H

#include "blinkit/blink/renderer/bindings/core/script_controller.h"

namespace BlinKit {

class WebViewContext final : public ScriptController
{
public:
    WebViewContext(LocalFrame &frame);
private:
    void OnContextCreated(JSContext *ctx, JSValue global) override;
};

} // namespace BlinKit

#endif // BLINKIT_WEB_VIEW_CONTEXT_H
