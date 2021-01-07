// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_impl.h
// Description: WebViewImpl Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WEB_VIEW_IMPL_H
#define BLINKIT_BLINKIT_WEB_VIEW_IMPL_H

#pragma once

#include "bk_ui.h"
#include "cc/paint/skia_paint_canvas.h"
#include "blinkit/blink_impl/local_frame_client_impl.h"
#include "third_party/blink/public/platform/web_rect.h"
#include "third_party/blink/public/platform/web_size.h"
#include "third_party/blink/renderer/core/frame/resize_viewport_anchor.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page_visibility_state.h"
#include "third_party/blink/renderer/core/page/page_widget_delegate.h"
#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/graphics/graphics_layer.h"

namespace blink {
class BrowserControls;
class PageScaleConstraintsSet;
struct ViewportDescription;
}

class WebViewImpl : public blink::WebWidget, public BlinKit::LocalFrameClientImpl
{
public:
    virtual ~WebViewImpl(void);

    void Initialize(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    int LoadUI(const char *URI);
    void SetClient(const BkWebViewClient &client);

    // Returns the page object associated with this view. This may be null when
    // the page is shutting down, but will be valid at all other times.
    blink::Page* GetPage(void) const { return m_page.get(); }
    float PageScaleFactor(void) const;
    void SetPageScaleFactor(float scaleFactor);
    blink::IntSize MainFrameSize(void);

    void InvalidateRect(const blink::IntRect &rect);

    // By default, all phases are updated by |UpdateLifecycle| (e.g., style,
    // layout, prepaint, paint, etc. See: document_lifecycle.h). |LifecycleUpdate|
    // can be used to only update to a specific lifecycle phase.
    void UpdateLifecycle(LifecycleUpdate requestedUpdate = LifecycleUpdate::kAll);

    float ClampPageScaleFactorToLimits(float scaleFactor) const;
    void ScheduleAnimation(void);
    void UpdateMainFrameLayoutSize(void);
    void UpdatePageDefinedViewportConstraints(const blink::ViewportDescription &description);
    void UpdateLayerTreeViewport(void);

    void ResizeAfterLayout(void);
    void RefreshPageScaleFactor(void);

    void DidChangeContentsSize(void);
    void MainFrameLayoutUpdated(void);

    void StopDeferringCommits(void) {
        // BKTODO: Check this later.
    }
protected:
    WebViewImpl(blink::PageVisibilityState visibilityState, SkColor baseBackgroundColor = SK_ColorWHITE);

    bool ProcessTitleChange(const std::string &newTitle) const;
    void PaintContent(cc::PaintCanvas *canvas, const blink::WebRect &rect);
    void Resize(const blink::WebSize &size);
    void SetScaleFactor(float scaleFactor);
    void UpdateAndPaint(void);
private:
    blink::BrowserControls& GetBrowserControls(void);

    blink::IntSize FrameSize(void);
    blink::IntSize ContentsSize(void) const;
    blink::Color BaseBackgroundColor(void) const;
    blink::PageScaleConstraintsSet& GetPageScaleConstraintsSet(void) const;
    float MinimumPageScaleFactor(void) const;
    void SetVisibilityState(blink::PageVisibilityState visibilityState, bool isInitialState);
    bool ShouldAutoResize(void) const { return m_shouldAutoResize; }
    void UpdateICBAndResizeViewport(void);
    void ResizeWithBrowserControls(const blink::WebSize &newSize, float topControlsHeight, float bottomControlsHeight,
        bool browserControlsShrinkLayout);
    void ResizeViewWhileAnchored(float topControlsHeight, float bottomControlsHeight, bool browserControlsShrinkLayout);
    void SendResizeEventAndRepaint(void);

    void UpdateLayerTreeBackgroundColor(void);

    bool IsAcceleratedCompositingActive(void) const;
    virtual SkBitmap PrepareBitmapForCanvas(const blink::WebSize &size) = 0;

    // LocalFrameClient
    bool IsCrawler(void) const final { return false; }
    bool HasWebView(void) const final { return true; }
    void TransitionToCommittedForNewPage(void) final;
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) final;
    void DispatchDidFinishLoad(void) final;

    BkWebViewClient m_client;
    std::unique_ptr<blink::ChromeClient> m_chromeClient;
    blink::WebSize m_size;
    // If true, automatically resize the layout view around its content.
    bool m_shouldAutoResize = false;
    std::unique_ptr<blink::Page> m_page;
    std::unique_ptr<blink::LocalFrame> m_frame;
    SkColor m_baseBackgroundColor;
    std::unique_ptr<cc::SkiaPaintCanvas> m_canvas;

    bool m_shouldDispatchFirstVisuallyNonEmptyLayout = false;
    bool m_shouldDispatchFirstLayoutAfterFinishedParsing = false;
    bool m_shouldDispatchFirstLayoutAfterFinishedLoading = false;

    std::unique_ptr<blink::ResizeViewportAnchor> m_resizeViewportAnchor;
};

#endif // BLINKIT_BLINKIT_WEB_VIEW_IMPL_H
