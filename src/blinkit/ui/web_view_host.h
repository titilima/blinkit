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

#include "bk_ui.h"

#include "blinkit/blink/public/platform/WebCursorInfo.h"
#include "blinkit/blink/public/web/WebContextMenuData.h"
#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "third_party/skia/include/core/SkCanvas.h"

class WebViewImpl;

namespace blink {
enum PageVisibilityState;
class WebLayerTreeView;
}

namespace BlinKit {

class LayerTreeHost;
class PaintUITask;

class WebViewHost
{
public:
    virtual ~WebViewHost(void);

    WebViewImpl* GetView(void) const { return m_view.get(); }
    WebLayerTreeView* GetLayerTreeView(void) const;

    void EnterAnimationSession(void);
    void LeaveAnimationSession(void);

    virtual void Invalidate(const IntRect &rect) = 0;
    virtual void ScheduleAnimation(void) = 0;
    virtual std::unique_ptr<PaintUITask> PreparePaintTask(void) = 0;

    virtual void ChangeTitle(const std::string &title) {}

    virtual void DidChangeCursor(const WebCursorInfo &cursorInfo) = 0;

    virtual void ShowContextMenu(const WebContextMenuData &data) = 0;
    virtual void ClearContextMenu(void) {}

    static SkColor DefaultBackgroundColor(void);
protected:
    WebViewHost(const BkWebViewClient &client, PageVisibilityState visibilityState);

    void InitializeView(float scaleFactor);
    void Resize(const IntSize &size);
    void SetScaleFactor(float scaleFactor);
private:
    std::unique_ptr<WebViewImpl> m_view;
    std::unique_ptr<LayerTreeHost> m_layerTreeHost;
};

} // namespace BlinKit

#endif // BLINKIT_WEB_VIEW_HOST_H
