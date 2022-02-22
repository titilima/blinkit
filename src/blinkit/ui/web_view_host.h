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
#include "blinkit/ui/animation/animation_proxy.h"
#include "third_party/skia/include/core/SkCanvas.h"

namespace blink {
enum PageVisibilityState;
class WebLayerTreeView;
}

namespace BlinKit {

class LayerTreeHost;

class WebViewHost : public AnimationProxy
{
public:
    ~WebViewHost(void) override;

    WebViewImpl* GetView(void) const override { return m_view; }
    WebLayerTreeView* GetLayerTreeView(void) const;

    virtual void DidChangeCursor(const WebCursorInfo &cursorInfo) = 0;

    virtual void ShowContextMenu(const WebContextMenuData &data) = 0;
    virtual void ClearContextMenu(void) {}

    static SkColor DefaultBackgroundColor(void);

    void ProcessDocumentReady(void);
    virtual void ProcessTitleChange(const String &title) = 0;
    void ProcessSizeChanged(const IntSize &size);
protected:
    WebViewHost(const BkWebViewClient &client, PageVisibilityState visibilityState);

    void InitializeView(float scaleFactor);
    void Resize(const IntSize &size);
    void SetScaleFactor(float scaleFactor);

    const BkWebViewClient m_client;
private:
    void Commit(void) final;

    WebViewImpl *m_view;
    std::unique_ptr<LayerTreeHost> m_layerTreeHost;
};

} // namespace BlinKit

#endif // BLINKIT_WEB_VIEW_HOST_H
