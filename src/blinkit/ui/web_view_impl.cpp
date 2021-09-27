// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_view_impl.cpp
// Description: WebViewImpl Class
//      Author: Ziming Li
//     Created: 2020-10-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./web_view_impl.h"

#include "blinkit/app/app_impl.h"
#include "blinkit/blink/renderer/core/editing/FrameSelection.h"
#include "blinkit/blink/renderer/core/editing/InputMethodController.h"
#include "blinkit/blink/renderer/core/events/UIEventWithKeyState.h"
#include "blinkit/blink/renderer/core/frame/FrameHost.h"
#include "blinkit/blink/renderer/core/frame/FrameView.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/blink/renderer/core/frame/Settings.h"
#include "blinkit/blink/renderer/core/input/EventHandler.h"
#include "blinkit/blink/renderer/core/layout/LayoutView.h"
#include "blinkit/blink/renderer/core/layout/TextAutosizer.h"
#include "blinkit/blink/renderer/core/loader/FrameLoadRequest.h"
#include "blinkit/blink/renderer/core/page/FocusController.h"
#include "blinkit/blink/renderer/core/page/Page.h"
#include "blinkit/blink/renderer/web/ChromeClientImpl.h"
#include "blinkit/blink/renderer/web/ResizeViewportAnchor.h"
#include "blinkit/ui/rendering_scheduler.h"
#include "chromium/base/time/time.h"
#include "third_party/zed/include/zed/float.hpp"
#if 0 // BKTODO:
#include "blinkit/blink/renderer/core/editing/editor.h"
#include "blinkit/blink/renderer/core/frame/browser_controls.h"
#include "blinkit/blink/renderer/core/frame/page_scale_constraints_set.h"
#include "blinkit/blink/renderer/core/frame/viewport_data.h"
#include "blinkit/blink/renderer/core/frame/visual_viewport.h"
#include "blinkit/blink/renderer/core/editing/editing_utilities.h"
#include "blinkit/blink/renderer/core/editing/selection_template.h"
#include "blinkit/blink/renderer/core/layout/text_autosizer.h"
#include "blinkit/blink/renderer/core/paint/paint_layer_scrollable_area.h"
#endif

using namespace blink;
using namespace BlinKit;

// Constants for viewport anchoring on resize.
static const float ViewportAnchorCoordX = 0.5f;
static const float ViewportAnchorCoordY = 0;

const WebInputEvent* WebViewImpl::m_currentInputEvent = nullptr;

WebViewImpl::WebViewImpl(ClientCaller &clientCaller, PageVisibilityState visibilityState, SkColor baseBackgroundColor)
    : FrameLoaderClient(AppImpl::Get().GetAppCaller(), clientCaller)
    , m_chromeClient(ChromeClientImpl::create(this))
    , m_contextMenuClientImpl(this)
    , m_dragClientImpl(this)
    , m_editorClientImpl(this)
    , m_baseBackgroundColor(baseBackgroundColor)
{
    ASSERT(isMainThread());
    memset(&m_client, 0, sizeof(m_client));

    Page::PageClients pageClients;
    pageClients.chromeClient = m_chromeClient.get();
    pageClients.contextMenuClient = &m_contextMenuClientImpl;
    pageClients.dragClient = &m_dragClientImpl;
    pageClients.editorClient = &m_editorClientImpl;
    m_page = Page::create(pageClients);
    SetVisibilityStateImpl(visibilityState, true);

#if 0 // BKTODO:
    initializeLayerTreeView();

    allInstances().add(this);

    m_pageImportanceSignals.setObserver(client);
#endif
}

WebViewImpl::~WebViewImpl(void)
{
    ASSERT(isMainThread());
    m_page->willBeDestroyed();
}

#if 0 // BKTODO:
Color WebViewImpl::BaseBackgroundColor(void) const
{
    return m_baseBackgroundColor;
}

void WebViewImpl::CancelPagePopup(void)
{
    BKLOG("// BKTODO: CancelPagePopup");
}
#endif

float WebViewImpl::ClampPageScaleFactorToLimits(float scaleFactor) const
{
    return GetPageScaleConstraintsSet().finalConstraints().clampToConstraints(scaleFactor);
}

IntSize WebViewImpl::ContentsSize(void) const
{
    if (LayoutView *layoutView = m_frame->contentLayoutObject())
        return layoutView->documentRect().size();
    return IntSize();
}

void WebViewImpl::didChangeContentsSize(void)
{
    GetPageScaleConstraintsSet().didChangeContentsSize(ContentsSize(), PageScaleFactor());
}

void WebViewImpl::didRemoveAllPendingStylesheet(void)
{
    Document *document = m_frame->document();

    ASSERT(document->isHTMLDocument());
    // For HTML if we have no more stylesheets to load and we're past the body
    // tag, we should have something to paint so resume.
    if (nullptr == document->body())
        return;

    ResumeTreeViewCommitsIfRenderingReady();
}

void WebViewImpl::DidUpdateTopControls(void)
{
#if 0 // BKTODO:
    if (m_layerTreeView)
    {
        m_layerTreeView->setTopControlsShownRatio(topControls().shownRatio());
        m_layerTreeView->setTopControlsHeight(topControls().height(), topControls().shrinkViewport());
    }
#endif

    if (!m_frame)
        return;

    FrameView *view = m_frame->view();
    if (nullptr == view)
        return;

    VisualViewport &visualViewport = m_page->frameHost().visualViewport();
    TopControls &topControls = m_page->frameHost().topControls();

    float topControlsViewportAdjustment = topControls.layoutHeight() - topControls.contentOffset();
    visualViewport.setTopControlsAdjustment(topControlsViewportAdjustment);

    // Shrink the FrameView by the amount that will maintain the aspect-ratio with the VisualViewport.
    view->setTopControlsViewportAdjustment(topControlsViewportAdjustment / MinimumPageScaleFactor());
}

#if 0 // BKTODO:
void WebViewImpl::DispatchDidFailProvisionalLoad(const ResourceError &error)
{
    ASSERT(false); // BKTODO:
}
#endif

void WebViewImpl::dispatchDidFinishLoad(void)
{
    auto _ = m_lock.guard_shared();
    m_client.DocumentReady(m_client.UserData);
}

#if 0 // BKTODO:
IntSize WebViewImpl::FrameSize(void)
{
    // The frame size should match the viewport size at minimum scale, since the
    // viewport must always be contained by the frame.
    FloatSize frameSize(m_size);
    frameSize.Scale(1 / MinimumPageScaleFactor());
    return ExpandedIntSize(frameSize);
}
#endif

WebViewImpl* WebViewImpl::From(LocalFrame &frame)
{
    ASSERT(frame.client()->GetType() == FrameClient::Type::WebView);
    return static_cast<WebViewImpl *>(frame.client());
}

WebViewImpl* WebViewImpl::From(Document &document)
{
    ASSERT(document.isUINode());
    if (LocalFrame *frame = document.frame())
        return WebViewImpl::From(*frame);
    return nullptr;
}

#if 0 // BKTODO:
BrowserControls& WebViewImpl::GetBrowserControls(void)
{
    return m_page->GetBrowserControls();
}
#endif

PageScaleConstraintsSet& WebViewImpl::GetPageScaleConstraintsSet(void) const
{
    return m_page->frameHost().pageScaleConstraintsSet();
}

WebInputEventResult WebViewImpl::handleCharEvent(const WebKeyboardEvent &event)
{
    ASSERT(false); // BKTODO:
    return WebInputEventResult::NotHandled;
}

WebInputEventResult WebViewImpl::handleKeyEvent(const WebKeyboardEvent &event)
{
    ASSERT(false); // BKTODO:
    return WebInputEventResult::NotHandled;
}

void WebViewImpl::handleMouseDown(LocalFrame &frame, const WebMouseEvent &event)
{
#if 0 // BKTODO:
    // If there is a popup open, close it as the user is clicking on the page
    // (outside of the popup). We also save it so we can prevent a click on an
    // element from immediately reopening the same popup.
    RefPtr<WebPagePopupImpl> pagePopup;
    if (event.button == WebMouseEvent::ButtonLeft) {
        pagePopup = m_pagePopup;
        hidePopups();
        ASSERT(!m_pagePopup);
    }
#endif

    m_mouseEventSession.SetLastMouseDownPosition(event.x, event.y);

    // Take capture on a mouse down on a plugin so we can send it mouse events.
    // If the hit node is a plugin but a scrollbar is over it don't start mouse
    // capture because it will interfere with the scrollbar receiving events.
    IntPoint point(event.x, event.y);
    if (WebMouseEvent::ButtonLeft == event.button && m_page->mainFrame()->isLocalFrame())
    {
        point = m_page->deprecatedLocalMainFrame()->view()->rootFrameToContents(point);
        HitTestResult result(m_page->deprecatedLocalMainFrame()->eventHandler().hitTestResultAtPoint(point));
        result.setToShadowHostIfInUserAgentShadowRoot();
#if 0 // BKTODO:
        Node* hitNode = result.innerNodeOrImageMapImage();

        if (!result.scrollbar() && hitNode && hitNode->layoutObject() && hitNode->layoutObject()->isEmbeddedObject()) {
            m_mouseCaptureNode = hitNode;
            TRACE_EVENT_ASYNC_BEGIN0("input", "capturing mouse", this);
        }
#endif
    }

    PageWidgetEventHandler::handleMouseDown(frame, event);

#if 0 // BKTODO:
    if (event.button == WebMouseEvent::ButtonLeft && m_mouseCaptureNode)
        m_mouseCaptureGestureToken = mainFrame.eventHandler().takeLastMouseDownGestureToken();

    if (m_pagePopup && pagePopup && m_pagePopup->hasSamePopupClient(pagePopup.get())) {
        // That click triggered a page popup that is the same as the one we just closed.
        // It needs to be closed.
        cancelPagePopup();
    }
#endif

    // Dispatch the contextmenu event regardless of if the click was swallowed.
    if (!Settings::showContextMenuOnMouseUp())
    {
#if OS(MACOSX)
        if (event.button == WebMouseEvent::ButtonRight
            || (event.button == WebMouseEvent::ButtonLeft
                && event.modifiers & WebMouseEvent::ControlKey))
            mouseContextMenu(event);
#else
        if (WebMouseEvent::ButtonRight == event.button)
            mouseContextMenu(event);
#endif
    }
}

void WebViewImpl::handleMouseUp(LocalFrame &frame, const WebMouseEvent &event)
{
    PageWidgetEventHandler::handleMouseUp(frame, event);

    if (Settings::showContextMenuOnMouseUp())
    {
        // Dispatch the contextmenu event regardless of if the click was swallowed.
        // On Mac/Linux, we handle it on mouse down, not up.
        if (WebMouseEvent::ButtonRight == event.button)
            mouseContextMenu(event);
    }
}

WebInputEventResult WebViewImpl::handleMouseWheel(LocalFrame &frame, const WebMouseWheelEvent &event)
{
    ASSERT(false); // BKTODO:
    return WebInputEventResult::NotHandled;
}

void WebViewImpl::HidePopups(void)
{
#if 0 // BKTODO:
    if (m_pagePopup)
        m_pagePopup->cancel();
#endif
}

void WebViewImpl::Initialize(void)
{
    ScopedRenderingScheduler scheduler(this);
    m_frame = LocalFrame::create(this, &(m_page->frameHost()));
    m_frame->init();
    OnInitialized();
}

void WebViewImpl::invalidateRect(const IntRect &rect)
{
#if 0 // BKTODO:
    if (m_layerTreeView)
        updateLayerTreeViewport();
    else if (m_client)
        m_client->didInvalidateRect(rect);
#else
    RenderingScheduler::From(this)->InvalidateRect(rect);
#endif
}

#if 0 // BKTODO:
bool WebViewImpl::IsAcceleratedCompositingActive(void) const
{
    return false; // BKTODO: Support GPU.
}
#endif

void WebViewImpl::layoutUpdated(LocalFrame *frame)
{
    if (m_shouldAutoResize)
    {
        ASSERT(false); // BKTODO:
#if 0
        WebSize frameSize = frame->view()->frameRect().size();
        if (frameSize != m_size) {
            m_size = frameSize;

            page()->frameHost().visualViewport().setSize(m_size);
            pageScaleConstraintsSet().didChangeViewSize(m_size);

            m_client->didAutoResize(m_size);
            sendResizeEventAndRepaint();
        }
#endif
    }

    if (GetPageScaleConstraintsSet().constraintsDirty())
        RefreshPageScaleFactorAfterLayout();

    FrameView *view = frame->view();

    view->resize(m_size);
    // Relayout immediately to avoid violating the rule that needsLayout()
    // isn't set at the end of a layout.
    if (view->needsLayout())
        view->layout();

    // In case we didn't have a size when the top controls were updated.
    DidUpdateTopControls();

    // BKTODO: didUpdateLayout may be useful in the future, leave it here.
    // m_client->didUpdateLayout();
}

int WebViewImpl::LoadUI(const char *URI)
{
    KURL u(URI);
    if (!u.isValid() || u.scheme_is_in_http_family())
    {
        ASSERT(u.isValid());
        ASSERT(!u.scheme_is_in_http_family()); // Standard URL is for crawlers only!
        return BK_ERR_URI;
    }

    auto task = [this, u]
    {
        ResourceRequest request(u);
        request.SetView(this);
        m_frame->loader().load(FrameLoadRequest(nullptr, request));
    };
    m_appCaller.Call(BLINK_FROM_HERE, task);
    return BK_ERR_SUCCESS;
}

#if 0 // BKTODO:
void WebViewImpl::MainFrameLayoutUpdated(void)
{
    // May be useful, leave it here.
}
#endif

IntSize WebViewImpl::MainFrameSize(void)
{
    return GetPageScaleConstraintsSet().mainFrameSize();
}

float WebViewImpl::MinimumPageScaleFactor(void) const
{
    return GetPageScaleConstraintsSet().finalConstraints().minimumScale;
}

void WebViewImpl::mouseContextMenu(const WebMouseEvent &event)
{
    ASSERT(false); // BKTODO:
}

float WebViewImpl::PageScaleFactor(void) const
{
    if (m_page)
        return m_page->frameHost().visualViewport().scale();
    return 1.0;
}

#if 0 // BKTODO:
void WebViewImpl::PaintContent(cc::PaintCanvas *canvas, const WebRect &rect)
{
    // This should only be used when compositing is not being used for this
    // WebView, and it is painting into the recording of its parent.
    ASSERT(!IsAcceleratedCompositingActive());
    PageWidgetDelegate::PaintContent(*m_page, canvas, rect, *m_page->MainFrame());
}
#endif

void WebViewImpl::PerformResize(void)
{
    GetPageScaleConstraintsSet().didChangeViewSize(m_size);

    updatePageDefinedViewportConstraints(m_frame->document()->viewportDescription());
    UpdateMainFrameLayoutSize();

    m_page->frameHost().visualViewport().setSize(m_size);

    if (FrameView *view = m_frame->view())
    {
        if (!view->needsLayout())
            PostLayoutResize(m_frame.get());
    }
}

void WebViewImpl::PostLayoutResize(LocalFrame *frame)
{
    frame->view()->resize(MainFrameSize());
}

WebInputEventResult WebViewImpl::ProcessInput(const WebInputEvent &e)
{
    ASSERT(isMainThread());

    // TODO(dcheng): The fact that this is getting called when there is no local
    // main frame is problematic and probably indicates a bug in the input event
    // routing code.
    if (!m_frame)
        return WebInputEventResult::NotHandled;

    // If we've started a drag and drop operation, ignore input events until
    // we're done.
    if (m_doingDragAndDrop)
        return WebInputEventResult::HandledSuppressed;

    // Report the event to be NOT processed by WebKit, so that the browser can handle it appropriately.
    if (m_ignoreInputEvents)
        return WebInputEventResult::NotHandled;

    zed::scoped_swap<const WebInputEvent *> currentEventChange(m_currentInputEvent, &e);
    UIEventWithKeyState::clearNewTabModifierSetFromIsolatedWorld();

    if (isPointerLocked() && WebInputEvent::isMouseEventType(e.type))
    {
        ASSERT(false); // BKTODO: pointerLockMouseEvent(inputEvent);
        return WebInputEventResult::HandledSystem;
    }

    // FIXME: This should take in the intended frame, not the local frame root.
    WebInputEventResult result = PageWidgetDelegate::handleInputEvent(*this, e, m_frame.get());
    if (result != WebInputEventResult::NotHandled)
        return result;

    return WebInputEventResult::NotHandled;
}

void WebViewImpl::ProcessMouseEvent(WebInputEvent::Type type, WebPointerProperties::Button button, int x, int y)
{
    ASSERT(IsClientThread());
    auto task = [this, type, button, x, y]
    {
        ScopedRenderingScheduler _(this);

        WebMouseEvent e;
        e.timeStampSeconds = base::Time::Now().ToDoubleT();
        e.type = type;
        if (WebInputEvent::MouseDown == type || WebInputEvent::MouseMove == type)
        {
            switch (button)
            {
                case WebPointerProperties::ButtonLeft:
                    e.modifiers |= WebInputEvent::LeftButtonDown;
                    break;
                case WebPointerProperties::ButtonRight:
                    e.modifiers |= WebInputEvent::RightButtonDown;
                    break;
                case WebPointerProperties::ButtonMiddle:
                    e.modifiers |= WebInputEvent::MiddleButtonDown;
                    break;
            }
        }
        e.x = e.windowX = e.globalX = x;
        e.y = e.windowY = e.globalY = y;
        e.button = button;
        m_mouseEventSession.PreProcess(e);
        ProcessInput(e);
        m_mouseEventSession.PostProcess(e);
    };
    m_appCaller.SyncCall(BLINK_FROM_HERE, task);
}

bool WebViewImpl::ProcessTitleChange(const std::string &title) const
{
    ASSERT(IsClientThread());

    auto _ = m_lock.guard_shared();
    if (nullptr == m_client.TitleChange)
        return false;
    return m_client.TitleChange(title.c_str(), m_client.UserData);
}

void WebViewImpl::RefreshPageScaleFactorAfterLayout(void)
{
    if (!m_frame || !m_page || nullptr == m_page->mainFrame())
        return;

    FrameView *view = m_page->deprecatedLocalMainFrame()->view();
    if (nullptr == view)
        return;

    updatePageDefinedViewportConstraints(m_frame->document()->viewportDescription());

    PageScaleConstraintsSet &pageScaleConstraintsSet = GetPageScaleConstraintsSet();
    pageScaleConstraintsSet.computeFinalConstraints();

    int verticalScrollbarWidth = 0;
    if (Scrollbar *vb = view->verticalScrollbar())
    {
        if (!vb->isOverlayScrollbar())
            verticalScrollbarWidth = vb->width();
    }
    pageScaleConstraintsSet.adjustFinalConstraintsToContentsSize(ContentsSize(), verticalScrollbarWidth, Settings::shrinksViewportContentToFit());

    float newPageScaleFactor = PageScaleFactor();
    if (pageScaleConstraintsSet.needsReset() && pageScaleConstraintsSet.finalConstraints().initialScale != -1)
    {
        newPageScaleFactor = pageScaleConstraintsSet.finalConstraints().initialScale;
        pageScaleConstraintsSet.setNeedsReset(false);
    }
    SetPageScaleFactor(newPageScaleFactor);

    UpdateLayerTreeViewport();
}

void WebViewImpl::Resize(const IntSize &size)
{
    ASSERT(isMainThread());

    if (m_shouldAutoResize || m_size == size)
        return;

    if (!m_frame)
        return;

    FrameView *view = m_frame->view();
    if (nullptr == view)
        return;
    m_size = size;

    ScopedRenderingScheduler scheduler(this);
    {
        ResizeViewportAnchor anchor(*view, m_page->frameHost().visualViewport());
        ResizeViewWhileAnchored(view);
        SendResizeEventAndRepaint();
    }
}

#if 0 // BKTODO:
void WebViewImpl::ResizeAfterLayout(void)
{
    Page *page = GetPage();
    ASSERT(nullptr != page->GetFrame());

    if (m_shouldAutoResize)
    {
        LocalFrameView *view = page->GetFrame()->View();
        WebSize frameSize = view->Size();
        if (frameSize != m_size)
        {
            m_size = frameSize;

            page->GetVisualViewport().SetSize(m_size);
            GetPageScaleConstraintsSet().DidChangeInitialContainingBlockSize(m_size);
            view->SetInitialViewportSize(m_size);

            ASSERT(false); // BKTODO:
#if 0
            client_->DidAutoResize(size_);
#endif
            SendResizeEventAndRepaint();
        }
    }

    if (GetPageScaleConstraintsSet().ConstraintsDirty())
        RefreshPageScaleFactor();

    m_resizeViewportAnchor->ResizeFrameView(MainFrameSize());
}
#endif

void WebViewImpl::ResizeViewWhileAnchored(FrameView *view)
{
    ASSERT(m_frame);

    {
        // Avoids unnecessary invalidations while various bits of state in TextAutosizer are updated.
        TextAutosizer::DeferUpdatePageInfo deferUpdatePageInfo(m_page.get());
        PerformResize();
    }

    // Update lifecyle phases immediately to recalculate the minimum scale limit for rotation anchoring,
    // and to make sure that no lifecycle states are stale if this WebView is embedded in another one.
    UpdateLifecycle();
}

#if 0 // BKTODO:
void WebViewImpl::ResizeWithBrowserControls(
    const WebSize &newSize,
    float topControlsHeight, float bottomControlsHeight,
    bool browserControlsShrinkLayout)
{
    if (m_shouldAutoResize)
        return;

    if (m_size == newSize)
    {
        BrowserControls &browserControls = GetBrowserControls();
        if (browserControls.TopHeight() == topControlsHeight
            && browserControls.BottomHeight() == bottomControlsHeight
            && browserControls.ShrinkViewport() == browserControlsShrinkLayout)
        {
            return;
        }
    }

    LocalFrame *mainFrame = m_page->MainFrame();
    if (nullptr == mainFrame)
        return;

    LocalFrameView *view = mainFrame->View();
    if (nullptr == view)
        return;

    VisualViewport &visualViewport = m_page->GetVisualViewport();

    bool isRotation = Settings::MainFrameResizesAreOrientationChanges
        && 0 != m_size.width
        && 0 != ContentsSize().Width()
        && newSize.width != m_size.width;
    m_size = newSize;

    FloatSize viewportAnchorCoords(ViewportAnchorCoordX, ViewportAnchorCoordY);
    if (isRotation)
    {
        ASSERT(false); // BKTODO:
#if 0
        RotationViewportAnchor anchor(*view, visualViewport,
            viewportAnchorCoords,
            GetPageScaleConstraintsSet());
        ResizeViewWhileAnchored(top_controls_height, bottom_controls_height,
            browser_controls_shrink_layout);
#endif
    }
    else
    {
        ResizeViewportAnchor::ResizeScope resizeScope(*m_resizeViewportAnchor);
        ResizeViewWhileAnchored(topControlsHeight, bottomControlsHeight, browserControlsShrinkLayout);
    }
    SendResizeEventAndRepaint();
}
#endif

void WebViewImpl::ResumeTreeViewCommitsIfRenderingReady(void)
{
#if 0 // BKTODO:
    LocalFrame* frame = mainFrameImpl()->frame();
    if (!frame->loader().stateMachine()->committedFirstRealDocumentLoad())
        return;
    if (!frame->document()->isRenderingReady())
        return;
    if (m_layerTreeView) {
        m_layerTreeView->setDeferCommits(false);
        m_layerTreeView->setNeedsBeginFrame();
    }
#endif
}

void WebViewImpl::scheduleAnimation(void)
{
#if 0 // BKTODO:
    if (m_layerTreeView)
    {
        m_layerTreeView->setNeedsBeginFrame();
        return;
    }
#endif
    RenderingScheduler::From(this)->ScheduleAnimation();
}

void WebViewImpl::SendResizeEventAndRepaint(void)
{
    // FIXME: This is wrong. The FrameView is responsible sending a resizeEvent
    // as part of layout. Layout is also responsible for sending invalidations
    // to the embedder. This method and all callers may be wrong. -- eseidel.
    if (nullptr != m_frame->view())
    {
        // Enqueues the resize event.
        m_frame->document()->enqueueResizeEvent();
    }

#if 0 // BKTODO:
    if (m_layerTreeView)
        UpdateLayerTreeViewport();
#else
    IntRect damagedRect(IntPoint(0, 0), m_size);
    RenderingScheduler::From(this)->InvalidateRect(damagedRect);
#endif
    UpdatePageOverlays();
}

void WebViewImpl::SetClient(const BkWebViewClient &client)
{
    auto _ = m_lock.guard();
    memset(&m_client, 0, sizeof(m_client));

    m_client.UserData = client.UserData;
    m_client.DocumentReady = client.DocumentReady;
    m_client.TitleChange = client.TitleChange;

    // Use `offsetof` macro for different client versions.
}

void WebViewImpl::SetFocus(bool focus)
{
    ASSERT(IsClientThread());
    m_appCaller.Call(
        BLINK_FROM_HERE,
        std::bind(&WebViewImpl::SetFocusImpl, this, focus)
    );
}

void WebViewImpl::SetFocusImpl(bool focus)
{
    ASSERT(isMainThread());

    ScopedRenderingScheduler scheduler(this);
    m_page->focusController().setFocused(focus);
    if (focus)
    {
        m_page->focusController().setActive(true);
        RefPtrWillBeRawPtr<LocalFrame> focusedFrame = m_page->focusController().focusedFrame();
        if (focusedFrame)
        {
            Element *element = focusedFrame->document()->focusedElement();
            if (nullptr != element && focusedFrame->selection().selection().isNone())
            {
                // If the selection was cleared while the WebView was not
                // focused, then the focus element shows with a focus ring but
                // no caret and does respond to keyboard inputs.
                if (element->isTextFormControl())
                {
                    element->updateFocusAppearance(SelectionBehaviorOnFocus::Restore);
                }
                else if (element->isContentEditable())
                {
                    // updateFocusAppearance() selects all the text of
                    // contentseditable DIVs. So we set the selection explicitly
                    // instead. Note that this has the side effect of moving the
                    // caret back to the beginning of the text.
                    Position position(element, 0);
                    focusedFrame->selection().setSelection(VisibleSelection(position, SEL_DEFAULT_AFFINITY));
                }
            }
        }
        m_imeAcceptEvents = true;
    }
    else
    {
        HidePopups();

        // Clear focus on the currently focused frame if any.
        if (!m_page)
            return;

        Frame *frame = m_page->mainFrame();
        if (nullptr == frame)
            return;

        RefPtrWillBeRawPtr<LocalFrame> focusedFrame = m_page->focusController().focusedFrame();
        if (focusedFrame)
        {
            // Finish an ongoing composition to delete the composition node.
            if (focusedFrame->inputMethodController().hasComposition())            
            {
#if 0 // BKTODO:
                WebAutofillClient* autofillClient = WebLocalFrameImpl::fromFrame(focusedFrame.get())->autofillClient();

                if (autofillClient)
                    autofillClient->setIgnoreTextChanges(true);
#endif

                focusedFrame->inputMethodController().confirmComposition();
#if 0 // BKTODO:
                if (autofillClient)
                    autofillClient->setIgnoreTextChanges(false);
#endif
            }
            m_imeAcceptEvents = false;
        }
    }
}

void WebViewImpl::SetPageScaleFactor(float scaleFactor)
{
    ASSERT(m_page);

    scaleFactor = ClampPageScaleFactorToLimits(scaleFactor);
    if (zed::almost_equals(scaleFactor, PageScaleFactor()))
        return;

    m_page->frameHost().visualViewport().setScale(scaleFactor);
}

void WebViewImpl::SetScaleFactor(float scaleFactor)
{
    if (m_frame)
        m_frame->setPageZoomFactor(scaleFactor);
}

void WebViewImpl::SetVisibilityState(PageVisibilityState visibilityState)
{
    ASSERT(IsClientThread());
    m_appCaller.Call(
        BLINK_FROM_HERE,
        std::bind(&WebViewImpl::SetVisibilityStateImpl, this, visibilityState, false)
    );
}

void WebViewImpl::SetVisibilityStateImpl(PageVisibilityState visibilityState, bool isInitialState)
{
    ASSERT(isMainThread());
    ASSERT(m_page);
    m_page->setVisibilityState(visibilityState, isInitialState);

#if 0 // BKTODO:
    if (m_layerTreeView) {
        bool visible = visibilityState == WebPageVisibilityStateVisible;
        m_layerTreeView->setVisible(visible);
    }

    if (visibilityState == WebPageVisibilityStateVisible) {
        m_scheduler->setPageInBackground(false);
    }
    else {
        m_scheduler->setPageInBackground(true);
    }
#endif
}

void WebViewImpl::transitionToCommittedForNewPage(void)
{
    ASSERT(m_frame);

    m_frame->createView(m_size, m_baseBackgroundColor, false); // BKTODO: Is transparent needed?
    if (m_shouldAutoResize)
        ASSERT(false); // BKTODO: frame()->view()->enableAutoSizeMode(webView->minAutoSize(), webView->maxAutoSize());

#if 0 // BKTODO: Check if necessary.
    m_frame->view()->setInputEventsTransformForEmulation(m_inputEventsOffsetForEmulation, m_inputEventsScaleFactorForEmulation);
    m_frame->view()->setDisplayMode(webView->displayMode());
#endif
}

void WebViewImpl::UpdateAndPaint(void)
{
    UpdateLifecycle();

    if (m_size.isEmpty())
        return;

    bool resetCanvas = true;
    if (m_canvas)
    {
        SkImageInfo imageInfo = m_canvas->imageInfo();
        if (imageInfo.width() == m_size.width() && imageInfo.height() == m_size.height())
            resetCanvas = false;
    }

    auto _ = m_canvasLock.guard();
    if (resetCanvas)
    {
        m_canvas = std::make_unique<SkCanvas>(PrepareBitmapForCanvas(m_size));
        m_canvas->drawColor(m_baseBackgroundColor);
    }
    PageWidgetDelegate::paint(*m_page, m_canvas.get(), IntRect(IntPoint(), m_size), *m_page->deprecatedLocalMainFrame());
}

#if 0 // BKTODO:
void WebViewImpl::UpdateICBAndResizeViewport(void)
{
    // We'll keep the initial containing block size from changing when the top
    // controls hide so that the ICB will always be the same size as the
    // viewport with the browser controls shown.
    IntSize icbSize = m_size;

    BrowserControls &browserControls = GetBrowserControls();
    if (browserControls.PermittedState() == cc::BrowserControlsState::kBoth && !browserControls.ShrinkViewport())
        icbSize.Expand(0, -browserControls.TotalHeight());

    GetPageScaleConstraintsSet().DidChangeInitialContainingBlockSize(icbSize);

    LocalFrame *mainFrame = m_page->MainFrame();
    UpdatePageDefinedViewportConstraints(mainFrame->GetDocument()->GetViewportData().GetViewportDescription());
    UpdateMainFrameLayoutSize();

    m_page->GetVisualViewport().SetSize(m_size);

    if (LocalFrameView *frameView = mainFrame->View())
    {
        frameView->SetInitialViewportSize(icbSize);
        if (!frameView->NeedsLayout())
            m_resizeViewportAnchor->ResizeFrameView(MainFrameSize());
    }
}
#endif

void WebViewImpl::UpdateLayerTreeBackgroundColor(void)
{
#if 0 // BKTODO:
    if (!m_layerTreeView)
        return;

    m_layerTreeView->setBackgroundColor(alphaChannel(m_backgroundColorOverride) ? m_backgroundColorOverride : backgroundColor());
#endif
}

void WebViewImpl::UpdateLayerTreeViewport(void)
{
    // BKTODO:
}

void WebViewImpl::UpdateLifecycle(void)
{
    if (!m_frame)
        return;

    UpdateLayerTreeBackgroundColor();

    PageWidgetDelegate::updateAllLifecyclePhases(*m_page, *m_frame);

#if 0 // BKTODO:
    if (m_pageColorOverlay)
        m_pageColorOverlay->graphicsLayer()->paint(nullptr);

    // TODO(chrishtr): link highlights don't currently paint themselves, it's still driven by cc.
    // Fix this.
    for (size_t i = 0; i < m_linkHighlights.size(); ++i)
        m_linkHighlights[i]->updateGeometry();

    if (FrameView* view = mainFrameImpl()->frameView()) {
        LocalFrame* frame = mainFrameImpl()->frame();

        if (m_shouldDispatchFirstVisuallyNonEmptyLayout && view->isVisuallyNonEmpty()) {
            m_shouldDispatchFirstVisuallyNonEmptyLayout = false;
            // TODO(esprehn): Move users of this callback to something
            // better, the heuristic for "visually non-empty" is bad.
            client()->didMeaningfulLayout(WebMeaningfulLayout::VisuallyNonEmpty);
        }

        if (m_shouldDispatchFirstLayoutAfterFinishedParsing && frame->document()->hasFinishedParsing()) {
            m_shouldDispatchFirstLayoutAfterFinishedParsing = false;
            client()->didMeaningfulLayout(WebMeaningfulLayout::FinishedParsing);
        }

        if (m_shouldDispatchFirstLayoutAfterFinishedLoading && frame->document()->isLoadCompleted()) {
            m_shouldDispatchFirstLayoutAfterFinishedLoading = false;
            client()->didMeaningfulLayout(WebMeaningfulLayout::FinishedLoading);
        }
    }
#endif
}

void WebViewImpl::UpdateMainFrameLayoutSize(void)
{
    if (m_shouldAutoResize || !m_frame)
        return;

    FrameView *view = m_frame->view();
    if (nullptr == view)
        return;

    IntSize layoutSize = m_size;
    if (Settings::viewportEnabled())
        layoutSize = GetPageScaleConstraintsSet().layoutSize();
    if (Settings::forceZeroLayoutHeight())
        layoutSize.setHeight(0);
    view->setLayoutSize(layoutSize);
#if 0 // BKTODO:
    // Resizing marks the frame as needsLayout. Inform clients so that they
    // will perform the layout. Widgets held by WebPluginContainerImpl do not otherwise
    // see this resize layout invalidation.
    if (client())
        client()->didUpdateLayoutSize(layoutSize);
#endif
}

void WebViewImpl::updatePageDefinedViewportConstraints(const ViewportDescription &description)
{
    // If we're not reading the viewport meta tag, allow GPU rasterization.
    if (!Settings::viewportMetaEnabled())
    {
#if 0 // BKTODO:
        m_matchesHeuristicsForGpuRasterization = true;
        if (m_layerTreeView)
            m_layerTreeView->heuristicsForGpuRasterizationUpdated(m_matchesHeuristicsForGpuRasterization);
#endif
    }

    if (!Settings::viewportEnabled() || !m_page || m_size.isZero())
        return;

    ASSERT(false); // BKTODO:
#if 0
    Document* document = page()->deprecatedLocalMainFrame()->document();

    m_matchesHeuristicsForGpuRasterization = description.matchesHeuristicsForGpuRasterization();
    if (m_layerTreeView)
        m_layerTreeView->heuristicsForGpuRasterizationUpdated(m_matchesHeuristicsForGpuRasterization);

    Length defaultMinWidth = document->viewportDefaultMinWidth();
    if (defaultMinWidth.isAuto())
        defaultMinWidth = Length(ExtendToZoom);

    ViewportDescription adjustedDescription = description;
    if (settingsImpl()->viewportMetaLayoutSizeQuirk() && adjustedDescription.type == ViewportDescription::ViewportMeta) {
        const int legacyWidthSnappingMagicNumber = 320;
        if (adjustedDescription.maxWidth.isFixed() && adjustedDescription.maxWidth.value() <= legacyWidthSnappingMagicNumber)
            adjustedDescription.maxWidth = Length(DeviceWidth);
        if (adjustedDescription.maxHeight.isFixed() && adjustedDescription.maxHeight.value() <= m_size.height)
            adjustedDescription.maxHeight = Length(DeviceHeight);
        adjustedDescription.minWidth = adjustedDescription.maxWidth;
        adjustedDescription.minHeight = adjustedDescription.maxHeight;
}

    float oldInitialScale = pageScaleConstraintsSet().pageDefinedConstraints().initialScale;
    pageScaleConstraintsSet().updatePageDefinedConstraints(adjustedDescription, defaultMinWidth);

    if (settingsImpl()->clobberUserAgentInitialScaleQuirk()
        && pageScaleConstraintsSet().userAgentConstraints().initialScale != -1
        && pageScaleConstraintsSet().userAgentConstraints().initialScale * deviceScaleFactor() <= 1) {
        if (description.maxWidth == Length(DeviceWidth)
            || (description.maxWidth.type() == Auto && pageScaleConstraintsSet().pageDefinedConstraints().initialScale == 1.0f))
            setInitialPageScaleOverride(-1);
    }

    Settings& pageSettings = page()->settings();
    pageScaleConstraintsSet().adjustForAndroidWebViewQuirks(
        adjustedDescription,
        defaultMinWidth.intValue(),
        deviceScaleFactor(),
        settingsImpl()->supportDeprecatedTargetDensityDPI(),
        pageSettings.wideViewportQuirkEnabled(),
        pageSettings.useWideViewport(),
        pageSettings.loadWithOverviewMode(),
        settingsImpl()->viewportMetaNonUserScalableQuirk());
    float newInitialScale = pageScaleConstraintsSet().pageDefinedConstraints().initialScale;
    if (oldInitialScale != newInitialScale && newInitialScale != -1) {
        pageScaleConstraintsSet().setNeedsReset(true);
        if (mainFrameImpl() && mainFrameImpl()->frameView())
            mainFrameImpl()->frameView()->setNeedsLayout();
    }

    updateMainFrameLayoutSize();

    if (LocalFrame* frame = page()->deprecatedLocalMainFrame()) {
        if (TextAutosizer* textAutosizer = frame->document()->textAutosizer())
            textAutosizer->updatePageInfoInAllFrames();
    }
#endif
}

void WebViewImpl::UpdatePageOverlays(void)
{
#if 0 // BKTODO:
    if (m_pageColorOverlay)
        m_pageColorOverlay->update();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkLoadUI(BkWebView view, const char *URI)
{
    return view->LoadUI(URI);
}

BKEXPORT void BKAPI BkWebViewSetClient(BkWebView view, BkWebViewClient *client)
{
    view->SetClient(*client);
}

} // extern "C"
