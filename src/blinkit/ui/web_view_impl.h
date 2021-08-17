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
#include "blinkit/blink/renderer/core/loader/FrameLoaderClient.h"
#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/blink/renderer/web/ContextMenuClientImpl.h"
#include "blinkit/blink/renderer/web/DragClientImpl.h"
#include "blinkit/blink/renderer/web/EditorClientImpl.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/frame/resize_viewport_anchor.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page_visibility_state.h"
#include "third_party/blink/renderer/core/page/page_widget_delegate.h"
#include "third_party/blink/renderer/platform/graphics/graphics_layer.h"
#endif
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/zed/include/zed/mutex.hpp"
#include "third_party/zed/include/zed/shared_mutex.hpp"

namespace blink {
class BrowserControls;
class PageScaleConstraintsSet;
struct ViewportDescription;
}

class WebViewImpl : public blink::FrameLoaderClient
{
public:
    virtual ~WebViewImpl(void);

    void Initialize(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    int LoadUI(const char *URI);
    void SetClient(const BkWebViewClient &client);

#if 0 // BKTODO:
    // Returns the page object associated with this view. This may be null when
    // the page is shutting down, but will be valid at all other times.
    blink::Page* GetPage(void) const { return m_page.get(); }
#endif
    blink::IntSize MainFrameSize(void);

    virtual void InvalidateNativeView(const blink::IntRect &rect) = 0;

    void didChangeContentsSize(void);
    void invalidateRect(const blink::IntRect &rect);
    void layoutUpdated(blink::LocalFrame *frame);
    void scheduleAnimation(void);
    void UpdateAndPaint(void);
    void updatePageDefinedViewportConstraints(const blink::ViewportDescription &description);
    void UpdateLayerTreeViewport(void);
    void UpdateMainFrameLayoutSize(void);

#if 0 // BKTODO:
    void ResizeAfterLayout(void);

    void MainFrameLayoutUpdated(void);

    void StopDeferringCommits(void) {
        // BKTODO: Check this later.
    }
#endif
protected:
    WebViewImpl(BlinKit::ClientCaller &clientCaller, blink::PageVisibilityState visibilityState, SkColor baseBackgroundColor = SK_ColorWHITE);

    bool ProcessTitleChange(const std::string &newTitle) const;
    // BKTODO: void PaintContent(cc::PaintCanvas *canvas, const blink::WebRect &rect);
    void Resize(const blink::IntSize &size);
    void SetScaleFactor(float scaleFactor);
    void SetFocus(bool focus);
    void SetVisibilityState(blink::PageVisibilityState visibilityState);

    mutable zed::mutex m_canvasLock;
private:
    void SetFocusImpl(bool focus);
    void SetVisibilityStateImpl(blink::PageVisibilityState visibilityState, bool isInitialState);

    // By default, all phases are updated by |UpdateLifecycle| (e.g., style,
    // layout, prepaint, paint, etc. See: document_lifecycle.h). |LifecycleUpdate|
    // can be used to only update to a specific lifecycle phase.
    void UpdateLifecycle(void);
    void UpdateLayerTreeBackgroundColor(void);
#if 0 // BKTODO:
    void CancelPagePopup(void);

    blink::BrowserControls& GetBrowserControls(void);

    blink::IntSize FrameSize(void);
    blink::Color BaseBackgroundColor(void) const;
#endif
    float ClampPageScaleFactorToLimits(float scaleFactor) const;
    blink::IntSize ContentsSize(void) const;
    // Called anytime top controls layout height or content offset have changed.
    void DidUpdateTopControls(void);
    blink::PageScaleConstraintsSet& GetPageScaleConstraintsSet(void) const;
    void HidePopups(void);
    float MinimumPageScaleFactor(void) const;
    float PageScaleFactor(void) const;
    void PerformResize(void);
    void PostLayoutResize(blink::LocalFrame *frame);
    void RefreshPageScaleFactorAfterLayout(void);
    void ResizeViewWhileAnchored(blink::FrameView *view);
#if 0 // BKTODO:
    bool ShouldAutoResize(void) const { return m_shouldAutoResize; }
    void UpdateICBAndResizeViewport(void);
    void ResizeWithBrowserControls(const blink::WebSize &newSize, float topControlsHeight, float bottomControlsHeight,
        bool browserControlsShrinkLayout);
#endif
    void SendResizeEventAndRepaint(void);
    void SetPageScaleFactor(float scaleFactor);
    void UpdatePageOverlays(void);

    // BKTODO: bool IsAcceleratedCompositingActive(void) const;
    virtual SkBitmap PrepareBitmapForCanvas(const blink::IntSize &size) = 0;

    // FrameClient
#ifdef BLINKIT_FULL_BUILD
    Type GetType(void) const override { return Type::WebView; }
#endif
    // FrameLoaderClient
    void transitionToCommittedForNewPage(void) final;
#if 0 // BKTODO:
    // LocalFrameClient
    bool HasWebView(void) const final { return true; }
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) final;
    void DidFinishLoad(void) final;
#endif

    mutable zed::shared_mutex m_lock;
    BkWebViewClient m_client;
    std::unique_ptr<blink::ChromeClient> m_chromeClient;
    blink::ContextMenuClientImpl m_contextMenuClientImpl;
    blink::DragClientImpl m_dragClientImpl;
    blink::EditorClientImpl m_editorClientImpl;
    blink::IntSize m_size;
    // If true, automatically resize the layout view around its content.
    bool m_shouldAutoResize = false;
    std::unique_ptr<blink::Page> m_page;
    std::unique_ptr<blink::LocalFrame> m_frame;
    SkColor m_baseBackgroundColor;
    std::unique_ptr<SkCanvas> m_canvas;

    // TODO(ekaramad): Can we remove this and make sure IME events are not called
    // when there is no page focus?
    // Represents whether or not this object should process incoming IME events.
    bool m_imeAcceptEvents = true;

#if 0 // BKTODO:
    bool m_shouldDispatchFirstVisuallyNonEmptyLayout = false;
    bool m_shouldDispatchFirstLayoutAfterFinishedParsing = false;
    bool m_shouldDispatchFirstLayoutAfterFinishedLoading = false;

    std::unique_ptr<blink::ResizeViewportAnchor> m_resizeViewportAnchor;
#endif
};

// BKTODO: DEFINE_TYPE_CASTS(WebViewImpl, ::blink::LocalFrameClient, client, client->IsWebView(), client.IsWebView());

#endif // BLINKIT_BLINKIT_WEB_VIEW_IMPL_H
