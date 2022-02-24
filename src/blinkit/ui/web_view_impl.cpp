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
#include "blinkit/blink/impl/graphics_layer_factory_impl.h"
#include "blinkit/blink/public/platform/web_layer_tree_view.h"
#include "blinkit/blink/public/web/WebContextMenuData.h"
#include "blinkit/blink/renderer/core/editing/Editor.h"
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
#include "blinkit/blink/renderer/core/page/ContextMenuController.h"
#include "blinkit/blink/renderer/core/page/FocusController.h"
#include "blinkit/blink/renderer/core/page/Page.h"
#include "blinkit/blink/renderer/platform/KeyboardCodes.h"
#include "blinkit/blink/renderer/web/chrome_client_impl.h"
#include "blinkit/blink/renderer/web/ContextMenuAllowedScope.h"
#include "blinkit/blink/renderer/web/ResizeViewportAnchor.h"
#include "blinkit/blink/renderer/web/WebInputEventConversion.h"
#include "blinkit/ui/click_observer_wrapper.h"
#include "blinkit/ui/element_impl.h"
#include "blinkit/ui/input_events.h"
#include "blinkit/ui/web_view_host.h"
#include "chromium/base/time/time.h"
#include "third_party/zed/include/zed/threading/signal.hpp"
#if 0 // BKTODO:
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

WebViewImpl::WebViewImpl(WebViewHost &host, PageVisibilityState visibilityState, SkColor baseBackgroundColor)
    : m_host(host)
    , m_graphicsLayerFactory(std::make_unique<GraphicsLayerFactoryImpl>(*this))
    , m_chromeClient(ChromeClientImpl::create(this))
    , m_contextMenuClientImpl(this)
    , m_dragClientImpl(this)
    , m_editorClientImpl(this)
    , m_baseBackgroundColor(baseBackgroundColor)
{
    ASSERT(isMainThread());

    Page::PageClients pageClients;
    pageClients.chromeClient = m_chromeClient.get();
    pageClients.contextMenuClient = &m_contextMenuClientImpl;
    pageClients.dragClient = &m_dragClientImpl;
    pageClients.editorClient = &m_editorClientImpl;
    m_page = Page::create(pageClients);
    m_page->setVisibilityState(visibilityState, true);

#if 0 // BKTODO:
    m_pageImportanceSignals.setObserver(client);
#endif
}

WebViewImpl::~WebViewImpl(void)
{
    ASSERT(isMainThread());
    m_page->willBeDestroyed();
}

bool WebViewImpl::AddClickObserver(const char *id, BkClickObserver ob, void *userData)
{
    Document *document = m_frame->document();
    if (nullptr == document)
        return false;

    Element *element = document->getElementById(AtomicString::fromUTF8(id));
    if (nullptr == document)
        return false;

    GCRefPtr<ClickObserverWrapper> listener = ClickObserverWrapper::Create(ob, userData);
    return element->addEventListener(EventTypeNames::click, listener.get());
}

SkColor WebViewImpl::BackgroundColor(void) const
{
#if 0 // BKTODO:
    if (isTransparent())
        return Color::transparent;
#endif

    if (!m_page || !m_frame)
        return m_baseBackgroundColor;

    FrameView *view = m_frame->view();
    return view->documentBackgroundColor().rgb();
}

void WebViewImpl::BeginFrame(double tick)
{
    ASSERT(isMainThread());
    if (m_page)
        PageWidgetDelegate::animate(*m_page, tick);
}

#if 0 // BKTODO:
void WebViewImpl::CancelPagePopup(void)
{
    BKLOG("// BKTODO: CancelPagePopup");
}
#endif

float WebViewImpl::ClampPageScaleFactorToLimits(float scaleFactor) const
{
    return GetPageScaleConstraintsSet().finalConstraints().clampToConstraints(scaleFactor);
}

void WebViewImpl::clearContextMenu(void)
{
    m_host.ClearContextMenu();
}

IntSize WebViewImpl::ContentsSize(void) const
{
    if (LayoutView *layoutView = m_frame->contentLayoutObject())
        return layoutView->documentRect().size();
    return IntSize();
}

void WebViewImpl::convertViewportToWindow(IntRect *rect) const
{
    rect->scale(m_frame->pageZoomFactor());
}

void WebViewImpl::didChangeContentsSize(void)
{
    GetPageScaleConstraintsSet().didChangeContentsSize(ContentsSize(), PageScaleFactor());
}

void WebViewImpl::didChangeCursor(const WebCursorInfo &cursorInfo)
{
    m_host.DidChangeCursor(cursorInfo);
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
    TopControls &topControls = GetTopControls();
    m_layerTreeView->setTopControlsShownRatio(topControls.shownRatio());
    m_layerTreeView->setTopControlsHeight(topControls.height(), topControls.shrinkViewport());

    if (!m_frame)
        return;

    FrameView *view = m_frame->view();
    if (nullptr == view)
        return;

    VisualViewport &visualViewport = m_page->frameHost().visualViewport();

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

void WebViewImpl::dispatchDidFinishDocumentLoad(bool)
{
    ResumeTreeViewCommitsIfRenderingReady();
}

void WebViewImpl::dispatchDidFinishLoad(void)
{
    AppImpl::Get().taskRunner()->postTask(BLINK_FROM_HERE, std::bind(&WebViewHost::ProcessDocumentReady, &m_host));
}

void WebViewImpl::dispatchDidReceiveTitle(const String &title)
{
    m_host.ProcessTitleChange(title);
}

bool WebViewImpl::EndActiveFlingAnimation(void)
{
#if 0 // BKTODO: Check the logic later
    if (m_gestureAnimation) {
        m_gestureAnimation.clear();
        m_flingSourceDevice = WebGestureDeviceUninitialized;
        if (m_layerTreeView)
            m_layerTreeView->didStopFlinging();
        return true;
    }
#endif
    return false;
}

WebViewImpl* WebViewImpl::From(Document &document)
{
    ASSERT(document.isUINode());
    if (LocalFrame *frame = document.frame())
        return static_cast<WebViewImpl *>(frame->client());
    return nullptr;
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

BrowserControls& WebViewImpl::GetBrowserControls(void)
{
    return m_page->GetBrowserControls();
}
#endif

ElementImpl* WebViewImpl::GetElementById(const char *id) const
{
    Document *document = m_frame->document();
    if (nullptr == document)
        return nullptr;

    Element *element = document->getElementById(AtomicString::fromUTF8(id));
    if (nullptr == document)
        return nullptr;

    auto it = m_exposedElements.find(element);
    if (m_exposedElements.end() != it)
        return it->second.get();

    ElementImpl *ret = new ElementImpl(*element);
    m_exposedElements.emplace(element, ret);
    return ret;
}

PageScaleConstraintsSet& WebViewImpl::GetPageScaleConstraintsSet(void) const
{
    return m_page->frameHost().pageScaleConstraintsSet();
}

TopControls& WebViewImpl::GetTopControls(void)
{
    return m_page->frameHost().topControls();
}

WebInputEventResult WebViewImpl::handleCharEvent(const WebKeyboardEvent &event)
{
    ASSERT(event.type == WebInputEvent::Char);

    // Please refer to the comments explaining the m_suppressNextKeypressEvent
    // member.  The m_suppressNextKeypressEvent is set if the KeyDown is
    // handled by Webkit. A keyDown event is typically associated with a
    // keyPress(char) event and a keyUp event. We reset this flag here as it
    // only applies to the current keyPress event.
    bool suppress = m_suppressNextKeypressEvent;
    m_suppressNextKeypressEvent = false;

#if 0 // BKTODO: Process later.
    // If there is a popup, it should be the one processing the event, not the
    // page.
    if (m_pagePopup)
        return m_pagePopup->handleKeyEvent(PlatformKeyboardEventBuilder(event));
#endif

    if (!m_frame)
        return suppress ? WebInputEventResult::HandledSuppressed : WebInputEventResult::NotHandled;

    EventHandler &handler = m_frame->eventHandler();

    PlatformKeyboardEventBuilder evt(event);
    if (!evt.isCharacterKey())
        return WebInputEventResult::HandledSuppressed;

    // Accesskeys are triggered by char events and can't be suppressed.
    if (handler.handleAccessKey(evt))
        return WebInputEventResult::HandledSystem;

    // Safari 3.1 does not pass off windows system key messages (WM_SYSCHAR) to
    // the eventHandler::keyEvent. We mimic this behavior on all platforms since
    // for now we are converting other platform's key events to windows key
    // events.
    if (evt.isSystemKey())
        return WebInputEventResult::NotHandled;

    if (suppress)
        return WebInputEventResult::HandledSuppressed;

    WebInputEventResult result = handler.keyEvent(evt);
    if (result != WebInputEventResult::NotHandled)
        return result;
    if (KeyEventDefault(event))
        return WebInputEventResult::HandledSystem;

    return WebInputEventResult::NotHandled;
}

WebInputEventResult WebViewImpl::handleKeyEvent(const WebKeyboardEvent &event)
{
    ASSERT((event.type == WebInputEvent::RawKeyDown)
        || (event.type == WebInputEvent::KeyDown)
        || (event.type == WebInputEvent::KeyUp));

    // Halt an in-progress fling on a key event.
    EndActiveFlingAnimation();

    // Please refer to the comments explaining the m_suppressNextKeypressEvent
    // member.
    // The m_suppressNextKeypressEvent is set if the KeyDown is handled by
    // Webkit. A keyDown event is typically associated with a keyPress(char)
    // event and a keyUp event. We reset this flag here as this is a new keyDown
    // event.
    m_suppressNextKeypressEvent = false;

#if 0 // BKTODO: Process later.
    // If there is a popup, it should be the one processing the event, not the
    // page.
    if (m_pagePopup) {
        m_pagePopup->handleKeyEvent(PlatformKeyboardEventBuilder(event));
        // We need to ignore the next Char event after this otherwise pressing
        // enter when selecting an item in the popup will go to the page.
        if (WebInputEvent::RawKeyDown == event.type)
            m_suppressNextKeypressEvent = true;
        return WebInputEventResult::HandledSystem;
    }
#endif

#if 0 // BKTODO: Check the logic later.
    RefPtrWillBeRawPtr<Frame> focusedFrame = focusedCoreFrame();
    if (focusedFrame && focusedFrame->isRemoteFrame()) {
        WebRemoteFrameImpl* webFrame = WebRemoteFrameImpl::fromFrame(*toRemoteFrame(focusedFrame.get()));
        webFrame->client()->forwardInputEvent(&event);
        return WebInputEventResult::HandledSystem;
    }

    if (!focusedFrame || !focusedFrame->isLocalFrame())
        return WebInputEventResult::NotHandled;

    LocalFrame* frame = toLocalFrame(focusedFrame.get());
#else
    if (!m_frame)
        return WebInputEventResult::NotHandled;

    LocalFrame *frame = m_frame.get();
#endif

    PlatformKeyboardEventBuilder evt(event);

    WebInputEventResult result = frame->eventHandler().keyEvent(evt);
    if (result != WebInputEventResult::NotHandled)
    {
        if (WebInputEvent::RawKeyDown == event.type)
        {
#if 0 // BKTODO:
            // Suppress the next keypress event unless the focused node is a plugin node.
            // (Flash needs these keypress events to handle non-US keyboards.)
            Element *element = focusedElement();
            if (element && element->layoutObject() && element->layoutObject()->isEmbeddedObject()) {
                if (event.windowsKeyCode == VKEY_TAB) {
                    // If the plugin supports keyboard focus then we should not send a tab keypress event.
                    Widget* widget = toLayoutPart(element->layoutObject())->widget();
                    if (widget && widget->isPluginContainer()) {
                        WebPluginContainerImpl* plugin = toWebPluginContainerImpl(widget);
                        if (plugin && plugin->supportsKeyboardFocus())
                            m_suppressNextKeypressEvent = true;
                    }
                }
            } else {
                m_suppressNextKeypressEvent = true;
            }
#else
            m_suppressNextKeypressEvent = true;
#endif
        }
        return result;
    }

#if !OS(MACOSX)
    const WebInputEvent::Type contextMenuTriggeringEventType =
#if OS(WIN)
        WebInputEvent::KeyUp;
#else
        WebInputEvent::RawKeyDown;
#endif

    bool isUnmodifiedMenuKey = !(event.modifiers & WebInputEvent::InputModifiers) && event.windowsKeyCode == VKEY_APPS;
    bool isShiftF10 = event.modifiers == WebInputEvent::ShiftKey && event.windowsKeyCode == VKEY_F10;
    if ((isUnmodifiedMenuKey || isShiftF10) && event.type == contextMenuTriggeringEventType)
    {
        ASSERT(false); // BKTODO: sendContextMenuEvent(event);
        return WebInputEventResult::HandledSystem;
    }
#endif // !OS(MACOSX)

    if (KeyEventDefault(event))
        return WebInputEventResult::HandledSystem;
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

void WebViewImpl::Initialize(float scaleFactor)
{
    m_layerTreeView = m_host.GetLayerTreeView();
    ASSERT(nullptr != m_layerTreeView);

    m_frame = LocalFrame::create(this, &(m_page->frameHost()), scaleFactor);
    m_frame->init();
}

void WebViewImpl::invalidateRect(const IntRect &rect)
{
    UpdateLayerTreeViewport();
}

bool WebViewImpl::KeyEventDefault(const WebKeyboardEvent &event)
{
    if (!m_frame)
        return false;

    switch (event.type)
    {
        case WebInputEvent::Char:
            if (event.windowsKeyCode == VKEY_SPACE)
            {
                int keyCode = (0 != (event.modifiers & WebInputEvent::ShiftKey) ? VKEY_PRIOR : VKEY_NEXT);
                ASSERT(false); // BKTODO: return scrollViewWithKeyboard(keyCode, event.modifiers);
            }
            break;
        case WebInputEvent::RawKeyDown:
            if (event.modifiers == WebInputEvent::ControlKey)
            {
                switch (event.windowsKeyCode)
                {
#if !OS(MACOSX)
                    case 'A':
                        ASSERT(false); // BKTODO: frame->executeCommand(WebString::fromUTF8("SelectAll"));
                        return true;
                    case VKEY_INSERT:
                    case 'C':
                        ASSERT(false); // BKTODO: frame->executeCommand(WebString::fromUTF8("Copy"));
                        return true;
#endif
                    // Match FF behavior in the sense that Ctrl+home/end are the only Ctrl
                    // key combinations which affect scrolling. Safari is buggy in the
                    // sense that it scrolls the page for all Ctrl+scrolling key
                    // combinations. For e.g. Ctrl+pgup/pgdn/up/down, etc.
                    case VKEY_HOME:
                    case VKEY_END:
                        break;
                    default:
                        return false;
                }
            }
            if (!event.isSystemKey && 0 == (event.modifiers & WebInputEvent::ShiftKey))
                return ScrollViewWithKeyboard(event.windowsKeyCode, event.modifiers);
            break;
        default:
            break;
    }
    return false;
}

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

    ResourceRequest request(u);
    m_frame->loader().load(FrameLoadRequest(nullptr, request));
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

bool WebViewImpl::MapKeyCodeForScroll(
    int keyCode,
    ScrollDirectionPhysical *scrollDirection,
    ScrollGranularity *scrollGranularity)
{
    switch (keyCode)
    {
        case VKEY_LEFT:
            *scrollDirection = ScrollLeft;
            *scrollGranularity = ScrollByLine;
            break;
        case VKEY_RIGHT:
            *scrollDirection = ScrollRight;
            *scrollGranularity = ScrollByLine;
            break;
        case VKEY_UP:
            *scrollDirection = ScrollUp;
            *scrollGranularity = ScrollByLine;
            break;
        case VKEY_DOWN:
            *scrollDirection = ScrollDown;
            *scrollGranularity = ScrollByLine;
            break;
        case VKEY_HOME:
            *scrollDirection = ScrollUp;
            *scrollGranularity = ScrollByDocument;
            break;
        case VKEY_END:
            *scrollDirection = ScrollDown;
            *scrollGranularity = ScrollByDocument;
            break;
        case VKEY_PRIOR:  // page up
            *scrollDirection = ScrollUp;
            *scrollGranularity = ScrollByPage;
            break;
        case VKEY_NEXT:  // page down
            *scrollDirection = ScrollDown;
            *scrollGranularity = ScrollByPage;
            break;
        default:
            return false;
    }

    return true;
}

float WebViewImpl::MaximumPageScaleFactor(void) const
{
    return GetPageScaleConstraintsSet().finalConstraints().maximumScale;
}

float WebViewImpl::MinimumPageScaleFactor(void) const
{
    return GetPageScaleConstraintsSet().finalConstraints().minimumScale;
}

void WebViewImpl::mouseContextMenu(const WebMouseEvent &event)
{
    if (!m_frame)
        return;

    FrameView *frameView = m_frame->view();
    if (nullptr == frameView)
        return;

    m_page->contextMenuController().clearContextMenu();

    PlatformMouseEventBuilder pme(frameView, event);
#if OS(WIN)
    frameView->setCursor(pointerCursor());
#endif

    {
        ContextMenuAllowedScope scope;
        m_frame->eventHandler().sendContextMenuEvent(pme, nullptr);
    }
    // Actually showing the context menu is handled by the ContextMenuClient
    // implementation...
}

float WebViewImpl::PageScaleFactor(void) const
{
    if (m_page)
        return m_page->frameHost().visualViewport().scale();
    return 1.0;
}

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

void WebViewImpl::ProcessKeyEvent(WebInputEvent::Type type, int code, int modifiers)
{
    WebKeyboardEvent e;
    e.timeStampSeconds = base::Time::Now().ToDoubleT();
    e.type = type;
    e.modifiers = modifiers;
    e.windowsKeyCode = e.nativeKeyCode = code;

    memset(e.text, 0, sizeof(e.text));
    e.text[0] = code;

    ProcessInput(e);
}

void WebViewImpl::ProcessMouseEvent(const BlinKit::MouseEvent &e)
{
    ProcessInput(e.Translate());
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

void WebViewImpl::registerForAnimations(WebLayer *layer)
{
    m_layerTreeView->registerForAnimations(layer);
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
    m_resizePending = true;

    ResizeViewportAnchor anchor(*view, m_page->frameHost().visualViewport());
    ResizeViewWhileAnchored(view);
    SendResizeEventAndRepaint();
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
    if (!m_frame->loader().stateMachine()->committedFirstRealDocumentLoad())
        return;
    if (!m_frame->document()->isRenderingReady())
        return;

    m_layerTreeView->setDeferCommits(false);
    m_layerTreeView->setNeedsBeginFrame();
}

void WebViewImpl::scheduleAnimation(void)
{
    m_layerTreeView->setNeedsBeginFrame();
}

bool WebViewImpl::ScrollViewWithKeyboard(int keyCode, int modifiers)
{
    ScrollDirectionPhysical scrollDirectionPhysical;
    ScrollGranularity scrollGranularity;
#if OS(MACOSX)
    // Control-Up/Down should be PageUp/Down on Mac.
    if (0 != (modifiers & WebMouseEvent::ControlKey))
    {
        if (keyCode == VKEY_UP)
            keyCode = VKEY_PRIOR;
        else if (keyCode == VKEY_DOWN)
            keyCode = VKEY_NEXT;
    }
#endif
    if (!MapKeyCodeForScroll(keyCode, &scrollDirectionPhysical, &scrollGranularity))
        return false;

    if (m_frame)
        return m_frame->eventHandler().bubblingScroll(toScrollDirection(scrollDirectionPhysical), scrollGranularity);
    return false;
}

bool WebViewImpl::SelectionBounds(IntRect &anchor, IntRect &focus) const
{
    if (!m_frame)
        return false;

    FrameSelection &selection = m_frame->selection();
    if (selection.isCaret())
    {
        anchor = focus = selection.absoluteCaretBounds();
    }
    else
    {
        const EphemeralRange selectedRange = selection.selection().toNormalizedEphemeralRange();
        if (selectedRange.isNull())
            return false;
        anchor = m_frame->editor().firstRectForRange(EphemeralRange(selectedRange.startPosition()));
        focus = m_frame->editor().firstRectForRange(EphemeralRange(selectedRange.endPosition()));
    }

    anchor = m_frame->view()->contentsToViewport(anchor);
    focus = m_frame->view()->contentsToViewport(focus);

    if (!selection.selection().isBaseFirst())
        std::swap(anchor, focus);
    return true;
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

    UpdateLayerTreeViewport();
    UpdatePageOverlays();
}

void WebViewImpl::SetFocus(bool focus)
{
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

void WebViewImpl::SetIsActive(bool active)
{
    if (m_page)
        m_page->focusController().setActive(active);
}

void WebViewImpl::SetPageScaleFactor(float scaleFactor)
{
    ASSERT(m_page);

    scaleFactor = ClampPageScaleFactorToLimits(scaleFactor);
    if (zed::almost_equals(scaleFactor, PageScaleFactor()))
        return;

    m_page->frameHost().visualViewport().setScale(scaleFactor);
}

void WebViewImpl::setRootGraphicsLayer(GraphicsLayer *layer)
{
    ASSERT(nullptr != m_layerTreeView);

    // In SPv2, we attach layers via PaintArtifactCompositor, rather than
    // supplying a root GraphicsLayer from PaintLayerCompositor.
    ASSERT(!RuntimeEnabledFeatures::slimmingPaintV2Enabled());

    VisualViewport &visualViewport = m_page->frameHost().visualViewport();
    visualViewport.attachToLayerTree(layer, graphicsLayerFactory());
    if (nullptr != layer)
    {
        m_rootGraphicsLayer = visualViewport.rootGraphicsLayer();
        m_rootLayer = m_rootGraphicsLayer->platformLayer();
        UpdateRootLayerTransform();
        m_layerTreeView->setRootLayer(*m_rootLayer);
        // We register viewport layers here since there may not be a layer
        // tree view prior to this point.
        visualViewport.registerLayersWithTreeView(m_layerTreeView);
        UpdatePageOverlays();
        // TODO(enne): Work around page visibility changes not being
        // propagated to the WebView in some circumstances.  This needs to
        // be refreshed here when setting a new root layer to avoid being
        // stuck in a presumed incorrectly invisible state.
        bool visible = page()->visibilityState() == PageVisibilityStateVisible;
        m_layerTreeView->setVisible(visible);
    }
    else
    {
        m_rootGraphicsLayer = nullptr;
        m_rootLayer = nullptr;
        // This means that we're transitioning to a new page. Suppress
        // commits until Blink generates invalidations so we don't
        // attempt to paint too early in the next page load.
        m_layerTreeView->setDeferCommits(true);
        m_layerTreeView->clearRootLayer();
        visualViewport.clearLayersForTreeView(m_layerTreeView);
    }
}

void WebViewImpl::SetScaleFactor(float scaleFactor)
{
    if (m_frame)
        m_frame->SetScaleFactor(scaleFactor);
}

void WebViewImpl::SetVisibilityState(PageVisibilityState visibilityState)
{
    ASSERT(isMainThread());

    ASSERT(m_page);
    m_page->setVisibilityState(visibilityState, false);

    m_layerTreeView->setVisible(PageVisibilityStateVisible == visibilityState);
#if 0 // BKTODO:
    if (visibilityState == WebPageVisibilityStateVisible) {
        m_scheduler->setPageInBackground(false);
    }
    else {
        m_scheduler->setPageInBackground(true);
    }
#endif
}

bool WebViewImpl::ShouldShowContextMenu(const WebContextMenuData &data)
{
    if (data.isEditable)
        return true;
    return WebContextMenuData::InputFieldTypePlainText == data.inputFieldType;
}

void WebViewImpl::showContextMenu(const WebContextMenuData &data)
{
    if (ShouldShowContextMenu(data))
        m_host.ShowContextMenu(data);
}

void WebViewImpl::transitionToCommittedForNewPage(void)
{
    ASSERT(m_frame);

    m_exposedElements.clear();
    m_frame->createView(m_size, m_baseBackgroundColor, false); // BKTODO: Is transparent needed?
    if (m_shouldAutoResize)
        ASSERT(false); // BKTODO: frame()->view()->enableAutoSizeMode(webView->minAutoSize(), webView->maxAutoSize());

#if 0 // BKTODO: Check if necessary.
    m_frame->view()->setInputEventsTransformForEmulation(m_inputEventsOffsetForEmulation, m_inputEventsScaleFactorForEmulation);
    m_frame->view()->setDisplayMode(webView->displayMode());
#endif
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
    m_layerTreeView->setBackgroundColor(BackgroundColor());
}

void WebViewImpl::UpdateLayerTreeViewport(void)
{
    if (!m_page)
        return;
    m_layerTreeView->setPageScaleFactorAndLimits(PageScaleFactor(), MinimumPageScaleFactor(), MaximumPageScaleFactor());
}

void WebViewImpl::UpdateLifecycle(void)
{
    if (!m_frame)
        return;

    UpdateLayerTreeBackgroundColor();

    PageWidgetDelegate::updateAllLifecyclePhases(*m_page, *m_frame);

    if (m_resizePending)
    {
        m_resizePending = false;
        m_host.ProcessSizeChanged(m_size);
    }

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
        m_matchesHeuristicsForGpuRasterization = true;
        m_layerTreeView->heuristicsForGpuRasterizationUpdated(m_matchesHeuristicsForGpuRasterization);
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

void WebViewImpl::UpdateRootLayerTransform(void)
{
    if (nullptr != m_rootGraphicsLayer)
    {
        TransformationMatrix transform;
        transform.translate(m_rootLayerOffset.width(), m_rootLayerOffset.height());
        transform = transform.scale(m_rootLayerScale);
        m_rootGraphicsLayer->setTransform(transform);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT bool_t BKAPI BkAddClickObserver(BkWebView view, const char *id, BkClickObserver ob, void *userData)
{
    return view->AddClickObserver(id, ob, userData);
}

BKEXPORT BkElement BKAPI BkGetElementById(BkWebView view, const char *id)
{
    return view->GetElementById(id);
}

BKEXPORT int BKAPI BkLoadUI(BkWebView view, const char *URI)
{
    return view->LoadUI(URI);
}

} // extern "C"
