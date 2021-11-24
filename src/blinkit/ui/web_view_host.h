#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_host.h
// Description: WebViewHost Class
//      Author: Ziming Li
//     Created: 2021-11-22
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_WEB_VIEW_HOST_H
#define BLINKIT_WEB_VIEW_HOST_H

#include "blinkit/blink/public/platform/WebCursorInfo.h"
#include "blinkit/blink/public/web/WebContextMenuData.h"
#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "third_party/skia/include/core/SkCanvas.h"

class WebViewImpl;

namespace BlinKit {

class WebViewHost
{
public:
    virtual ~WebViewHost(void) = default;

    virtual SkCanvas* RequireCanvas(int width, int height) = 0;
    virtual void Invalidate(const blink::IntRect *rect) = 0;
    virtual void ScheduleAnimation(void) = 0;

    virtual void ChangeTitle(const std::string &title) {}

    virtual void DidChangeCursor(const blink::WebCursorInfo &cursorInfo) = 0;

    virtual void ShowContextMenu(const blink::WebContextMenuData &data) = 0;
    virtual void ClearContextMenu(void) {}
protected:
    WebViewHost(void) = default;
};

} // namespace BlinKit

#endif // BLINKIT_WEB_VIEW_HOST_H
