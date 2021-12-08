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
#include "blinkit/blink/public/web/WebInputEvent.h"
#include "blinkit/blink/renderer/core/loader/FrameLoaderClient.h"
#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/blink/renderer/platform/graphics/GraphicsLayer.h"
#include "blinkit/blink/renderer/web/ContextMenuClientImpl.h"
#include "blinkit/blink/renderer/web/DragClientImpl.h"
#include "blinkit/blink/renderer/web/EditorClientImpl.h"
#include "blinkit/blink/renderer/web/PageWidgetDelegate.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/frame/resize_viewport_anchor.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page_visibility_state.h"
#include "third_party/blink/renderer/core/page/page_widget_delegate.h"
#endif
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/zed/include/zed/mutex.hpp"
#include "third_party/zed/include/zed/shared_mutex.hpp"

namespace blink {
class BrowserControls;
class PageScaleConstraintsSet;
class TopControls;
struct ViewportDescription;
struct WebContextMenuData;
struct WebCursorInfo;
class WebLayerTreeView;
}

namespace BlinKit {
class MouseEvent;
class WebViewHost;
}

class WebViewImpl final : public blink::FrameLoaderClient, public blink::PageWidgetEventHandler
{
public:
    WebViewImpl(const BkWebViewClient &client, blink::PageVisibilityState visibilityState, SkColor baseBackgroundColor = SK_ColorWHITE);
    ~WebViewImpl(void);

    void Initialize(BlinKit::WebViewHost *host);

    static WebViewImpl* From(blink::Document &document);

    void BeginFrame(double tick);
    void UpdateLifecycle(void);
    void PaintContent(SkCanvas *canvas, const blink::IntRect &rect);

    blink::IntSize MainFrameSize(void);

    void registerForAnimations(blink::WebLayer *layer);

    void setRootGraphicsLayer(blink::GraphicsLayer *layer);

    SkColor BaseBackgroundColor(void) const { return m_baseBackgroundColor; }
    void clearContextMenu(void);
    void convertViewportToWindow(blink::IntRect *rect) const;
    void didChangeContentsSize(void);
    void didChangeCursor(const blink::WebCursorInfo &cursorInfo);
    blink::FloatSize elasticOverscroll(void) const { return m_elasticOverscroll; }
    blink::LocalFrame* focusedCoreFrame(void) const { return m_frame.get(); }
    blink::LocalFrame& GetFrame(void) const { return *m_frame; }
    blink::GraphicsLayerFactory* graphicsLayerFactory(void) const { return m_graphicsLayerFactory.get(); }
    bool HasHost(void) const { return nullptr != m_host; }
    void invalidateRect(const blink::IntRect &rect);
    void layoutUpdated(blink::LocalFrame *frame);
    blink::Page* page(void) const { return m_page.get(); }
    void ProcessKeyEvent(blink::WebInputEvent::Type type, int code, int modifiers);
    void ProcessMouseEvent(const BlinKit::MouseEvent &e);
    bool ProcessTitleChange(const std::string &newTitle) const;
    void Resize(const blink::IntSize &size);
    void scheduleAnimation(void);
    bool SelectionBounds(blink::IntRect &anchor, blink::IntRect &focus) const;
    void SetScaleFactor(float scaleFactor);
    void SetFocus(bool focus);
    void SetVisibilityState(blink::PageVisibilityState visibilityState, bool isInitialState = false);
    void showContextMenu(const blink::WebContextMenuData &data);
    const blink::IntSize& Size(void) const { return m_size; }
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

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    bool AddClickObserver(const char *id, BkClickObserver ob, void *userData);
    ElementImpl* GetElementById(const char *id) const;
    int LoadUI(const char *URI);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
    void UpdateLayerTreeBackgroundColor(void);
#if 0 // BKTODO:
    void CancelPagePopup(void);

    blink::BrowserControls& GetBrowserControls(void);

    blink::IntSize FrameSize(void);
#endif
    SkColor BackgroundColor(void) const;
    float ClampPageScaleFactorToLimits(float scaleFactor) const;
    blink::IntSize ContentsSize(void) const;

    blink::TopControls& GetTopControls(void);
    // Called anytime top controls layout height or content offset have changed.
    void DidUpdateTopControls(void);
    bool EndActiveFlingAnimation(void);
    blink::PageScaleConstraintsSet& GetPageScaleConstraintsSet(void) const;
    void HidePopups(void);
    constexpr bool isPointerLocked(void) { return false; } // May be useful in the future, just leave it here.
    // Returns true if the event was actually processed.
    bool KeyEventDefault(const blink::WebKeyboardEvent &event);
    // Returns true if the event leads to scrolling.
    static bool MapKeyCodeForScroll(int keyCode, blink::ScrollDirectionPhysical *scrollDirection,
        blink::ScrollGranularity *scrollGranularity);
    float MinimumPageScaleFactor(void) const;
    float MaximumPageScaleFactor(void) const;
    void mouseContextMenu(const blink::WebMouseEvent &event);
    float PageScaleFactor(void) const;
    void PerformResize(void);
    void PostLayoutResize(blink::LocalFrame *frame);
    blink::WebInputEventResult ProcessInput(const blink::WebInputEvent &e);
    void RefreshPageScaleFactorAfterLayout(void);
    void ResizeViewWhileAnchored(blink::FrameView *view);
    void ResumeTreeViewCommitsIfRenderingReady(void);
    // Returns true if the view was scrolled.
    bool ScrollViewWithKeyboard(int keyCode, int modifiers);
#if 0 // BKTODO:
    bool ShouldAutoResize(void) const { return m_shouldAutoResize; }
    void UpdateICBAndResizeViewport(void);
    void ResizeWithBrowserControls(const blink::WebSize &newSize, float topControlsHeight, float bottomControlsHeight,
        bool browserControlsShrinkLayout);
#endif
    void SendResizeEventAndRepaint(void);
    void SetPageScaleFactor(float scaleFactor);
    static bool ShouldShowContextMenu(const blink::WebContextMenuData &data);
    void UpdatePageOverlays(void);
    void UpdateRootLayerTransform(void);

    // BKTODO: bool IsAcceleratedCompositingActive(void) const;

    // FrameClient
#ifdef BLINKIT_FULL_BUILD
    Type GetType(void) const override { return Type::WebView; }
#endif
    // FrameLoaderClient
    void dispatchDidReceiveTitle(const String &title) override;
    void dispatchDidFinishLoad(void) override;
    void transitionToCommittedForNewPage(void) override;
    void didRemoveAllPendingStylesheet(void) override;
    // PageWidgetEventHandler
    void handleMouseDown(blink::LocalFrame &frame, const blink::WebMouseEvent &event) override;
    void handleMouseUp(blink::LocalFrame &frame, const blink::WebMouseEvent &event) override;
    blink::WebInputEventResult handleMouseWheel(blink::LocalFrame &frame, const blink::WebMouseWheelEvent &event) override;
    blink::WebInputEventResult handleKeyEvent(const blink::WebKeyboardEvent &event) override;
    blink::WebInputEventResult handleCharEvent(const blink::WebKeyboardEvent &event) override;

#if 0 // BKTODO:
    // LocalFrameClient
    bool HasWebView(void) const final { return true; }
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) final;
#endif

    const BkWebViewClient m_client;
    BlinKit::WebViewHost *m_host = nullptr;

    std::unique_ptr<blink::GraphicsLayerFactory> m_graphicsLayerFactory;
    bool m_matchesHeuristicsForGpuRasterization = false;
    blink::WebLayerTreeView *m_layerTreeView = nullptr;
    blink::WebLayer *m_rootLayer = nullptr;
    blink::GraphicsLayer *m_rootGraphicsLayer = nullptr;
    blink::IntSize m_rootLayerOffset;
    float m_rootLayerScale = 1.f;

    std::unique_ptr<blink::ChromeClient> m_chromeClient;
    blink::ContextMenuClientImpl m_contextMenuClientImpl;
    blink::DragClientImpl m_dragClientImpl;
    blink::EditorClientImpl m_editorClientImpl;
    blink::IntSize m_size;
    // If true, automatically resize the layout view around its content.
    bool m_shouldAutoResize = false;
    std::unique_ptr<blink::Page> m_page;
    BlinKit::GCUniquePtr<blink::LocalFrame> m_frame;
    SkColor m_baseBackgroundColor;
    blink::FloatSize m_elasticOverscroll;

    static const blink::WebInputEvent *m_currentInputEvent;
    // TODO(ekaramad): Can we remove this and make sure IME events are not called
    // when there is no page focus?
    // Represents whether or not this object should process incoming IME events.
    bool m_imeAcceptEvents = true;
    bool m_doingDragAndDrop = false;
    bool m_ignoreInputEvents = false;
    // Webkit expects keyPress events to be suppressed if the associated keyDown
    // event was handled. Safari implements this behavior by peeking out the
    // associated WM_CHAR event if the keydown was handled. We emulate
    // this behavior by setting this flag if the keyDown was handled.
    bool m_suppressNextKeypressEvent = false;
#if 0 // BKTODO:
    bool m_shouldDispatchFirstVisuallyNonEmptyLayout = false;
    bool m_shouldDispatchFirstLayoutAfterFinishedParsing = false;
    bool m_shouldDispatchFirstLayoutAfterFinishedLoading = false;

    std::unique_ptr<blink::ResizeViewportAnchor> m_resizeViewportAnchor;
#endif

    bool m_loadFinished = false;
    mutable std::unordered_map<blink::Element *, std::unique_ptr<ElementImpl>> m_exposedElements;
};

// BKTODO: DEFINE_TYPE_CASTS(WebViewImpl, ::blink::LocalFrameClient, client, client->IsWebView(), client.IsWebView());

#endif // BLINKIT_BLINKIT_WEB_VIEW_IMPL_H
