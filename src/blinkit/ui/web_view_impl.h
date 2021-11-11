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
#include "blinkit/blink/renderer/web/PageWidgetDelegate.h"
#include "blinkit/ui/mouse_event_session.h"
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
struct WebContextMenuData;
struct WebCursorInfo;
}

class WebViewImpl : public blink::FrameLoaderClient, public blink::PageWidgetEventHandler
{
public:
    virtual ~WebViewImpl(void);

    void Initialize(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    int LoadUI(const char *URI);
    bool QueryElementPosition(const char *id, blink::IntRect &dst) const;
    void SetClient(const BkWebViewClient &client);

    blink::IntSize MainFrameSize(void);

    virtual void InvalidateNativeView(const blink::IntRect *rect = nullptr) = 0;

    void BeginFrame(void);
    virtual void clearContextMenu(void) {}
    void convertViewportToWindow(blink::IntRect *rect) const;
    void didChangeContentsSize(void);
    virtual void didChangeCursor(const blink::WebCursorInfo &cursorInfo) = 0;
    blink::FloatSize elasticOverscroll(void) const { return m_elasticOverscroll; }
    blink::LocalFrame* focusedCoreFrame(void) const { return m_frame.get(); }
    blink::LocalFrame& GetFrame(void) const { return *m_frame; }
    void invalidateRect(const blink::IntRect &rect);
    void layoutUpdated(blink::LocalFrame *frame);
    blink::Page* page(void) const { return m_page.get(); }
    void PostAnimationTask(void);
    void scheduleAnimation(void);
    bool SelectionBounds(blink::IntRect &anchor, blink::IntRect &focus) const;
    void showContextMenu(const blink::WebContextMenuData &data);
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

    class ContextMenu {
    public:
        virtual ~ContextMenu(void) = default;

        virtual void Show(void) = 0;
    };

    virtual void OnInitialized(void) {}
    void ProcessKeyEvent(blink::WebInputEvent::Type type, int code, int modifiers);
    void ProcessMouseEvent(blink::WebInputEvent::Type type, blink::WebPointerProperties::Button button, int x, int y);
    bool ProcessTitleChange(const std::string &newTitle) const;
    // BKTODO: void PaintContent(cc::PaintCanvas *canvas, const blink::WebRect &rect);
    void Resize(const blink::IntSize &size);
    void SetScaleFactor(float scaleFactor);
    void SetFocus(bool focus);
    void SetVisibilityState(blink::PageVisibilityState visibilityState);

    mutable zed::shared_mutex m_lock;
    mutable zed::mutex m_canvasLock;
    BlinKit::MouseEventSession m_mouseEventSession;
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
#endif
    void AnimationTimerFired(blink::Timer<WebViewImpl> *);
    float ClampPageScaleFactorToLimits(float scaleFactor) const;
    blink::IntSize ContentsSize(void) const;
    virtual std::shared_ptr<ContextMenu> CreateContextMenu(const blink::WebContextMenuData &data) = 0;
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

    // BKTODO: bool IsAcceleratedCompositingActive(void) const;
    virtual SkBitmap PrepareBitmapForCanvas(const blink::IntSize &size) = 0;

    // FrameClient
#ifdef BLINKIT_FULL_BUILD
    Type GetType(void) const override { return Type::WebView; }
#endif
    // FrameLoaderClient
    void transitionToCommittedForNewPage(void) final;
    void dispatchDidFinishLoad(void) final;
    void didRemoveAllPendingStylesheet(void) final;
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

    BkWebViewClient m_client;
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
    std::unique_ptr<SkCanvas> m_canvas;
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

    blink::Timer<WebViewImpl> m_animationTimer;
};

// BKTODO: DEFINE_TYPE_CASTS(WebViewImpl, ::blink::LocalFrameClient, client, client->IsWebView(), client.IsWebView());

#endif // BLINKIT_BLINKIT_WEB_VIEW_IMPL_H
