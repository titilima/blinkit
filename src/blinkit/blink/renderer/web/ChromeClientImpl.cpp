// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ChromeClientImpl.cpp
// Description: ChromeClientImpl Class
//      Author: Ziming Li
//     Created: 2021-08-08
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "web/ChromeClientImpl.h"

#include "blinkit/ui/web_view_impl.h"
#include "bindings/core/v8/ScriptController.h"
#include "core/HTMLNames.h"
// BKTODO: #include "core/dom/AXObjectCache.h"
#include "core/dom/Document.h"
// BKTODO: #include "core/dom/Fullscreen.h"
#include "core/dom/Node.h"
#include "core/events/UIEventWithKeyState.h"
// BKTODO: #include "core/frame/Console.h"
#include "core/frame/FrameHost.h"
#include "core/frame/FrameView.h"
#include "core/frame/Settings.h"
#include "core/html/HTMLInputElement.h"
#include "core/html/forms/ColorChooser.h"
#include "core/html/forms/ColorChooserClient.h"
#include "core/html/forms/DateTimeChooser.h"
#include "core/layout/HitTestResult.h"
#include "core/layout/LayoutPart.h"
#include "core/layout/compositing/CompositedSelection.h"
#include "core/loader/DocumentLoader.h"
#include "core/loader/FrameLoadRequest.h"
#include "core/page/Page.h"
#include "core/page/PopupOpeningObserver.h"
// BKTODO: #include "modules/accessibility/AXObject.h"
#include "platform/Cursor.h"
#include "platform/FileChooser.h"
#include "platform/KeyboardCodes.h"
#include "platform/RuntimeEnabledFeatures.h"
// BKTODO: #include "platform/exported/WrappedResourceRequest.h"
#include "platform/geometry/IntRect.h"
#include "platform/graphics/GraphicsLayer.h"
// BKTODO: #include "platform/weborigin/SecurityOrigin.h"
#include "public/platform/Platform.h"
// BKTODO: #include "public/platform/WebCursorInfo.h"
#include "public/platform/WebFrameScheduler.h"
#include "public/platform/WebURLRequest.h"
#include "public/platform/WebViewScheduler.h"
#if 0 // BKTODO:
#include "public/web/WebAutofillClient.h"
#include "public/web/WebColorChooser.h"
#include "public/web/WebColorSuggestion.h"
#include "public/web/WebConsoleMessage.h"
#include "public/web/WebFrameClient.h"
#include "public/web/WebInputElement.h"
#include "public/web/WebInputEvent.h"
#include "public/web/WebKit.h"
#include "public/web/WebNode.h"
#include "public/web/WebPageImportanceSignals.h"
#include "public/web/WebPlugin.h"
#include "public/web/WebPopupMenuInfo.h"
#include "public/web/WebSelection.h"
#include "public/web/WebSettings.h"
#include "public/web/WebTextDirection.h"
#include "public/web/WebTouchAction.h"
#include "public/web/WebUserGestureIndicator.h"
#include "public/web/WebUserGestureToken.h"
#include "public/web/WebViewClient.h"
#include "public/web/WebWindowFeatures.h"
#include "web/ColorChooserPopupUIController.h"
#include "web/ColorChooserUIController.h"
#include "web/DateTimeChooserImpl.h"
#include "web/ExternalDateTimeChooser.h"
#include "web/ExternalPopupMenu.h"
#include "web/PopupMenuImpl.h"
#include "web/WebFileChooserCompletionImpl.h"
#include "web/WebFrameWidgetImpl.h"
#include "web/WebInputEventConversion.h"
#include "web/WebLocalFrameImpl.h"
#include "web/WebPluginContainerImpl.h"
#include "web/WebSettingsImpl.h"
#endif
#include "wtf/text/CString.h"
#include "wtf/text/CharacterNames.h"
#include "wtf/text/StringBuilder.h"
#include "wtf/text/StringConcatenate.h"

namespace blink {

class WebCompositorAnimationTimeline;

#if 0 // BKTODO:
// Converts a AXObjectCache::AXNotification to a WebAXEvent
static WebAXEvent toWebAXEvent(AXObjectCache::AXNotification notification)
{
    // These enums have the same values; enforced in AssertMatchingEnums.cpp.
    return static_cast<WebAXEvent>(notification);
}
#endif

ChromeClientImpl::ChromeClientImpl(WebViewImpl* webView)
    : m_webView(webView)
    , m_cursorOverridden(false)
    , m_didRequestNonEmptyToolTip(false)
{
}

ChromeClientImpl::~ChromeClientImpl()
{
}

std::unique_ptr<ChromeClientImpl> ChromeClientImpl::create(WebViewImpl* webView)
{
    return zed::wrap_unique(new ChromeClientImpl(webView));
}

void* ChromeClientImpl::webView() const
{
    return static_cast<void*>(m_webView);
}

void ChromeClientImpl::chromeDestroyed()
{
    // Our lifetime is bound to the WebViewImpl.
}

void ChromeClientImpl::setWindowRect(const IntRect& r)
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->setWindowRect(r);
#endif
}

IntRect ChromeClientImpl::windowRect()
{
    ASSERT(false); // BKTODO:
    return IntRect();
#if 0
    WebRect rect;
    if (m_webView->client()) {
        rect = m_webView->client()->rootWindowRect();
    } else {
        // These numbers will be fairly wrong. The window's x/y coordinates will
        // be the top left corner of the screen and the size will be the content
        // size instead of the window size.
        rect.width = m_webView->size().width;
        rect.height = m_webView->size().height;
    }
    return IntRect(rect);
#endif
}

IntRect ChromeClientImpl::pageRect()
{
    // We hide the details of the window's border thickness from the web page by
    // simple re-using the window position here.  So, from the point-of-view of
    // the web page, the window has no border.
    return windowRect();
}

void ChromeClientImpl::focus()
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->didFocus();
#endif
}

bool ChromeClientImpl::canTakeFocus(WebFocusType)
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    // For now the browser can always take focus if we're not running layout
    // tests.
    return !layoutTestMode();
#endif
}

void ChromeClientImpl::takeFocus(WebFocusType type)
{
    ASSERT(false); // BKTODO:
#if 0
    if (!m_webView->client())
        return;
    if (type == WebFocusTypeBackward)
        m_webView->client()->focusPrevious();
    else
        m_webView->client()->focusNext();
#endif
}

void ChromeClientImpl::focusedNodeChanged(Node* fromNode, Node* toNode)
{
    ASSERT(false); // BKTODO:
#if 0
    if (!m_webView->client())
        return;

    m_webView->client()->focusedNodeChanged(WebNode(fromNode), WebNode(toNode));

    WebURL focusURL;
    if (toNode && toNode->isElementNode() && toElement(toNode)->isLiveLink() && toNode->shouldHaveFocusAppearance())
        focusURL = toElement(toNode)->hrefURL();
    m_webView->client()->setKeyboardFocusURL(focusURL);
#endif
}

bool ChromeClientImpl::hadFormInteraction() const
{
    ASSERT(false); // BKTODO: return m_webView->pageImportanceSignals() && m_webView->pageImportanceSignals()->hadFormInteraction();
    return false;
}

namespace {

#if 0 // BKTODO:
void updatePolicyForEvent(const WebInputEvent* inputEvent, NavigationPolicy* policy)
{
    if (!inputEvent)
        return;

    unsigned short buttonNumber = 0;
    if (inputEvent->type == WebInputEvent::MouseUp) {

        const WebMouseEvent* mouseEvent = static_cast<const WebMouseEvent*>(inputEvent);

        switch (mouseEvent->button) {
        case WebMouseEvent::ButtonLeft:
            buttonNumber = 0;
            break;
        case WebMouseEvent::ButtonMiddle:
            buttonNumber = 1;
            break;
        case WebMouseEvent::ButtonRight:
            buttonNumber = 2;
            break;
        default:
            return;
        }
    } else if ((WebInputEvent::isKeyboardEventType(inputEvent->type) && static_cast<const WebKeyboardEvent*>(inputEvent)->windowsKeyCode == VKEY_RETURN) || WebInputEvent::isGestureEventType(inputEvent->type)) {
        // Keyboard and gesture events can simulate mouse events.
        buttonNumber = 0;
    } else {
        return;
    }

    bool ctrl = inputEvent->modifiers & WebInputEvent::ControlKey;
    bool shift = inputEvent->modifiers & WebInputEvent::ShiftKey;
    bool alt = inputEvent->modifiers & WebInputEvent::AltKey;
    bool meta = inputEvent->modifiers & WebInputEvent::MetaKey;

    NavigationPolicy userPolicy = *policy;
    navigationPolicyFromMouseEvent(buttonNumber, ctrl, shift, alt, meta, &userPolicy);

    // When the input event suggests a download, but the navigation was initiated
    // by script, we should not override it.
    if (userPolicy == NavigationPolicyDownload && *policy != NavigationPolicyIgnore)
        return;

    // User and app agree that we want a new window; let the app override the decorations.
    if (userPolicy == NavigationPolicyNewWindow && *policy == NavigationPolicyNewPopup)
        return;
    *policy = userPolicy;
}

WebNavigationPolicy getNavigationPolicy(const WindowFeatures& features)
{
    // If our default configuration was modified by a script or wasn't
    // created by a user gesture, then show as a popup. Else, let this
    // new window be opened as a toplevel window.
    bool asPopup = !features.toolBarVisible
        || !features.statusBarVisible
        || !features.scrollbarsVisible
        || !features.menuBarVisible
        || !features.resizable;

    NavigationPolicy policy = NavigationPolicyNewForegroundTab;
    if (asPopup)
        policy = NavigationPolicyNewPopup;
    updatePolicyForEvent(WebViewImpl::currentInputEvent(), &policy);

    return static_cast<WebNavigationPolicy>(policy);
}

WebNavigationPolicy effectiveNavigationPolicy(NavigationPolicy navigationPolicy, const WindowFeatures& features)
{
    WebNavigationPolicy policy = static_cast<WebNavigationPolicy>(navigationPolicy);
    if (policy == WebNavigationPolicyIgnore)
        return getNavigationPolicy(features);
    if (policy == WebNavigationPolicyNewBackgroundTab && getNavigationPolicy(features) != WebNavigationPolicyNewBackgroundTab && !UIEventWithKeyState::newTabModifierSetFromIsolatedWorld())
        return WebNavigationPolicyNewForegroundTab;

    return policy;
}
#endif

} // namespace

#if 0 // BKTODO:
Page* ChromeClientImpl::createWindow(LocalFrame* frame, const FrameLoadRequest& r, const WindowFeatures& features,
    NavigationPolicy navigationPolicy, ShouldSetOpener shouldSetOpener)
{
    if (!m_webView->client())
        return nullptr;

    WebNavigationPolicy policy = effectiveNavigationPolicy(navigationPolicy, features);
    ASSERT(frame->document());
    Fullscreen::fullyExitFullscreen(*frame->document());

    WebViewImpl* newView = toWebViewImpl(
        m_webView->client()->createView(WebLocalFrameImpl::fromFrame(frame), WrappedResourceRequest(r.resourceRequest()), features, r.frameName(), policy, shouldSetOpener == NeverSetOpener));
    if (!newView)
        return nullptr;
    return newView->page();
}
#endif

void ChromeClientImpl::didOverscroll(const FloatSize& unusedDelta, const FloatSize& accumulatedRootOverScroll, const FloatPoint& position, const FloatSize& velocity)
{
    ASSERT(false); // BKTODO:
#if 0
    if (!m_webView->client())
        return;

    m_webView->client()->didOverscroll(unusedDelta, accumulatedRootOverScroll, position, velocity);
#endif
}

#if 0 // BKTODO:
void ChromeClientImpl::show(NavigationPolicy navigationPolicy)
{
    if (m_webView->client())
        m_webView->client()->show(effectiveNavigationPolicy(navigationPolicy, m_windowFeatures));
}
#endif

void ChromeClientImpl::setToolbarsVisible(bool value)
{
    ASSERT(false); // BKTODO: m_windowFeatures.toolBarVisible = value;
}

bool ChromeClientImpl::toolbarsVisible()
{
    ASSERT(false); // BKTODO: return m_windowFeatures.toolBarVisible;
    return false;
}

void ChromeClientImpl::setStatusbarVisible(bool value)
{
    ASSERT(false); // BKTODO: m_windowFeatures.statusBarVisible = value;
}

bool ChromeClientImpl::statusbarVisible()
{
    ASSERT(false); // BKTODO: return m_windowFeatures.statusBarVisible;
    return false;
}

void ChromeClientImpl::setScrollbarsVisible(bool value)
{
    ASSERT(false); // BKTODO:
#if 0
    m_windowFeatures.scrollbarsVisible = value;
    if (WebLocalFrameImpl* webFrame = toWebLocalFrameImpl(m_webView->mainFrame()))
        webFrame->setCanHaveScrollbars(value);
#endif
}

bool ChromeClientImpl::scrollbarsVisible()
{
    ASSERT(false); // BKTODO: return m_windowFeatures.scrollbarsVisible;
    return false;
}

void ChromeClientImpl::setMenubarVisible(bool value)
{
    ASSERT(false); // BKTODO: m_windowFeatures.menuBarVisible = value;
}

bool ChromeClientImpl::menubarVisible()
{
    ASSERT(false); // BKTODO: return m_windowFeatures.menuBarVisible;
    return false;
}

void ChromeClientImpl::setResizable(bool value)
{
    ASSERT(false); // BKTODO: m_windowFeatures.resizable = value;
}

bool ChromeClientImpl::shouldReportDetailedMessageForSource(LocalFrame& localFrame, const String& url)
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(localFrame.localFrameRoot());
    return webframe && webframe->client() && webframe->client()->shouldReportDetailedMessageForSource(url);
#endif
}

#if 0 // BKTODO:
void ChromeClientImpl::addMessageToConsole(LocalFrame* localFrame, MessageSource source, MessageLevel level, const String& message, unsigned lineNumber, const String& sourceID, const String& stackTrace)
{
    WebLocalFrameImpl* frame = WebLocalFrameImpl::fromFrame(localFrame);
    if (frame && frame->client()) {
        frame->client()->didAddMessageToConsole(
            WebConsoleMessage(static_cast<WebConsoleMessage::Level>(level), message),
            sourceID,
            lineNumber,
            stackTrace);
    }
}
#endif

bool ChromeClientImpl::canOpenBeforeUnloadConfirmPanel()
{
    ASSERT(false); // BKTODO: return !!m_webView->client();
    return false;
}

bool ChromeClientImpl::openBeforeUnloadConfirmPanelDelegate(LocalFrame* frame, const String& message, bool isReload)
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    notifyPopupOpeningObservers();
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(frame);
    return webframe->client() && webframe->client()->runModalBeforeUnloadDialog(isReload, message);
#endif
}

void ChromeClientImpl::closeWindowSoon()
{
    ASSERT(false); // BKTODO:
#if 0
    // Make sure this Page can no longer be found by JS.
    m_webView->page()->willBeClosed();

    // Make sure that all loading is stopped.  Ensures that JS stops executing!
    m_webView->mainFrame()->stopLoading();

    if (m_webView->client())
        m_webView->client()->closeWidgetSoon();
#endif
}

// Although a LocalFrame is passed in, we don't actually use it, since we
// already know our own m_webView.
bool ChromeClientImpl::openJavaScriptAlertDelegate(LocalFrame* frame, const String& message)
{
    ASSERT(false); // BKTODO:
#if 0
    notifyPopupOpeningObservers();
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(frame);
    if (webframe->client()) {
        if (WebUserGestureIndicator::isProcessingUserGesture())
            WebUserGestureIndicator::currentUserGestureToken().setJavascriptPrompt();
        webframe->client()->runModalAlertDialog(message);
        return true;
    }
#endif
    return false;
}

// See comments for openJavaScriptAlertDelegate().
bool ChromeClientImpl::openJavaScriptConfirmDelegate(LocalFrame* frame, const String& message)
{
    ASSERT(false); // BKTODO:
#if 0
    notifyPopupOpeningObservers();
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(frame);
    if (webframe->client()) {
        if (WebUserGestureIndicator::isProcessingUserGesture())
            WebUserGestureIndicator::currentUserGestureToken().setJavascriptPrompt();
        return webframe->client()->runModalConfirmDialog(message);
    }
#endif
    return false;
}

// See comments for openJavaScriptAlertDelegate().
bool ChromeClientImpl::openJavaScriptPromptDelegate(LocalFrame* frame, const String& message, const String& defaultValue, String& result)
{
    ASSERT(false); // BKTODO:
#if 0
    notifyPopupOpeningObservers();
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(frame);
    if (webframe->client()) {
        if (WebUserGestureIndicator::isProcessingUserGesture())
            WebUserGestureIndicator::currentUserGestureToken().setJavascriptPrompt();
        WebString actualValue;
        bool ok = webframe->client()->runModalPromptDialog(message, defaultValue, &actualValue);
        if (ok)
            result = actualValue;
        return ok;
    }
#endif
    return false;
}

void ChromeClientImpl::setStatusbarText(const String& message)
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->setStatusText(message);
#endif
}

bool ChromeClientImpl::tabsToLinks()
{
    ASSERT(false); // BKTODO: return m_webView->tabsToLinks();
    return false;
}

IntRect ChromeClientImpl::windowResizerRect() const
{
#if 0 // BKTODO: May be useful, leave it here.
    if (m_webView->client())
        return m_webView->client()->windowResizerRect();
#endif
    return IntRect();
}

void ChromeClientImpl::invalidateRect(const IntRect& updateRect)
{
    if (!updateRect.isEmpty())
        m_webView->invalidateRect(updateRect);
}

void ChromeClientImpl::scheduleAnimation(Widget* widget)
{
    ASSERT(widget->isFrameView());
    m_webView->scheduleAnimation();
}

IntRect ChromeClientImpl::viewportToScreen(const IntRect& rectInViewport) const
{
#if 0 // BKTODO:
    WebRect screenRect(rectInViewport);

    if (m_webView->client()) {
        m_webView->client()->convertViewportToWindow(&screenRect);
        WebRect windowRect = m_webView->client()->windowRect();
        screenRect.x += windowRect.x;
        screenRect.y += windowRect.y;
    }
    return screenRect;
#else
    return rectInViewport;
#endif
}

#if 0 // BKTODO:
WebScreenInfo ChromeClientImpl::screenInfo() const
{
    return m_webView->client() ? m_webView->client()->screenInfo() : WebScreenInfo();
}
#endif

void ChromeClientImpl::contentsSizeChanged(LocalFrame* frame, const IntSize& size) const
{
    m_webView->didChangeContentsSize();
    // BKTODO: frame->didChangeContentsSize(size);
}

void ChromeClientImpl::pageScaleFactorChanged() const
{
    ASSERT(false); // BKTODO: m_webView->pageScaleFactorChanged();
}

float ChromeClientImpl::clampPageScaleFactorToLimits(float scale) const
{
    ASSERT(false); // BKTODO: return m_webView->clampPageScaleFactorToLimits(scale);
    return 1;
}

void ChromeClientImpl::layoutUpdated(LocalFrame* frame) const
{
    m_webView->layoutUpdated(frame);
}

void ChromeClientImpl::showMouseOverURL(const HitTestResult& result)
{
    ASSERT(false); // BKTODO:
#if 0
    if (!m_webView->client())
        return;

    WebURL url;
    // Find out if the mouse is over a link, and if so, let our UI know...
    if (result.isLiveLink() && !result.absoluteLinkURL().string().isEmpty()) {
        url = result.absoluteLinkURL();
    } else if (result.innerNode()
        && (isHTMLObjectElement(*result.innerNode())
            || isHTMLEmbedElement(*result.innerNode()))) {
        LayoutObject* object = result.innerNode()->layoutObject();
        if (object && object->isLayoutPart()) {
            Widget* widget = toLayoutPart(object)->widget();
            if (widget && widget->isPluginContainer()) {
                WebPluginContainerImpl* plugin = toWebPluginContainerImpl(widget);
                url = plugin->plugin()->linkAtPosition(result.roundedPointInInnerNodeFrame());
            }
        }
    }

    m_webView->client()->setMouseOverURL(url);
#endif
}

void ChromeClientImpl::setToolTip(const String& tooltipText, TextDirection dir)
{
    ASSERT(false); // BKTODO:
#if 0
    if (!m_webView->client())
        return;
    if (!tooltipText.isEmpty()) {
        m_webView->client()->setToolTipText(tooltipText, toWebTextDirection(dir));
        m_didRequestNonEmptyToolTip = true;
    } else if (m_didRequestNonEmptyToolTip) {
        // WebViewClient::setToolTipText will send an IPC message.  We'd like to
        // reduce the number of setToolTipText calls.
        m_webView->client()->setToolTipText(tooltipText, toWebTextDirection(dir));
        m_didRequestNonEmptyToolTip = false;
    }
#endif
}

void ChromeClientImpl::dispatchViewportPropertiesDidChange(const ViewportDescription& description) const
{
    m_webView->updatePageDefinedViewportConstraints(description);
}

void ChromeClientImpl::printDelegate(LocalFrame* frame)
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->printPage(WebLocalFrameImpl::fromFrame(frame));
#endif
}

PassOwnPtrWillBeRawPtr<ColorChooser> ChromeClientImpl::openColorChooser(LocalFrame* frame, ColorChooserClient* chooserClient, const Color&)
{
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    notifyPopupOpeningObservers();
    OwnPtrWillBeRawPtr<ColorChooserUIController> controller = nullptr;
    if (RuntimeEnabledFeatures::pagePopupEnabled())
        controller = ColorChooserPopupUIController::create(frame, this, chooserClient);
    else
        controller = ColorChooserUIController::create(frame, chooserClient);
    controller->openUI();
    return controller.release();
#endif
}

PassRefPtrWillBeRawPtr<DateTimeChooser> ChromeClientImpl::openDateTimeChooser(DateTimeChooserClient* pickerClient, const DateTimeChooserParameters& parameters)
{
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    notifyPopupOpeningObservers();
#if ENABLE(INPUT_MULTIPLE_FIELDS_UI)
    return DateTimeChooserImpl::create(this, pickerClient, parameters);
#else
    return ExternalDateTimeChooser::create(this, m_webView->client(), pickerClient, parameters);
#endif
#endif
}

void ChromeClientImpl::openFileChooser(LocalFrame* frame, PassRefPtr<FileChooser> fileChooser)
{
    ASSERT(false); // BKTODO:
#if 0
    notifyPopupOpeningObservers();
    WebViewClient* client = m_webView->client();
    if (!client)
        return;

    WebFileChooserParams params;
    params.multiSelect = fileChooser->settings().allowsMultipleFiles;
    params.directory = fileChooser->settings().allowsDirectoryUpload;
    params.acceptTypes = fileChooser->settings().acceptTypes();
    params.selectedFiles = fileChooser->settings().selectedFiles;
    if (params.selectedFiles.size() > 0)
        params.initialValue = params.selectedFiles[0];
    params.useMediaCapture = fileChooser->settings().useMediaCapture;
    params.needLocalPath = fileChooser->settings().allowsDirectoryUpload;
    params.requestor = frame->document()->url();

    WebFileChooserCompletionImpl* chooserCompletion = new WebFileChooserCompletionImpl(fileChooser);
    if (client->runFileChooser(params, chooserCompletion))
        return;
    // Choosing failed, so do callback with an empty list.
    chooserCompletion->didChooseFile(WebVector<WebString>());
#endif
}

void ChromeClientImpl::enumerateChosenDirectory(FileChooser* fileChooser)
{
    ASSERT(false); // BKTODO:
#if 0
    WebViewClient* client = m_webView->client();
    if (!client)
        return;

    WebFileChooserCompletionImpl* chooserCompletion = new WebFileChooserCompletionImpl(fileChooser);

    ASSERT(fileChooser);
    ASSERT(fileChooser->settings().selectedFiles.size());

    // If the enumeration can't happen, call the callback with an empty list.
    if (!client->enumerateChosenDirectory(fileChooser->settings().selectedFiles[0], chooserCompletion))
        chooserCompletion->didChooseFile(WebVector<WebString>());
#endif
}

#if 0 // BKTODO:
Cursor ChromeClientImpl::lastSetCursorForTesting() const
{
    return m_lastSetMouseCursorForTesting;
}
#endif

void ChromeClientImpl::setCursor(const Cursor& cursor, LocalFrame* localRoot)
{
    ASSERT(false); // BKTODO:
#if 0
    m_lastSetMouseCursorForTesting = cursor;
    setCursor(WebCursorInfo(cursor), localRoot);
#endif
}

void ChromeClientImpl::setCursor(const WebCursorInfo& cursor, LocalFrame* localRoot)
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_cursorOverridden)
        return;

#if OS(MACOSX)
    // On Mac the mousemove event propagates to both the popup and main window.
    // If a popup is open we don't want the main window to change the cursor.
    if (m_webView->hasOpenedPopup())
        return;
#endif
    if (!m_webView->client())
        return;
    // TODO(kenrb, dcheng): For top-level frames we still use the WebView as
    // a WebWidget. This special case will be removed when top-level frames
    // get WebFrameWidgets.
    if (localRoot->isMainFrame()) {
        m_webView->client()->didChangeCursor(cursor);
    } else {
        WebLocalFrameImpl* webFrame = WebLocalFrameImpl::fromFrame(localRoot);
        ASSERT(webFrame);
        ASSERT(webFrame->frameWidget());
        if (toWebFrameWidgetImpl(webFrame->frameWidget())->client())
            toWebFrameWidgetImpl(webFrame->frameWidget())->client()->didChangeCursor(cursor);
    }
#endif
}

void ChromeClientImpl::setCursorForPlugin(const WebCursorInfo& cursor, LocalFrame* localRoot)
{
    setCursor(cursor, localRoot);
}

void ChromeClientImpl::setCursorOverridden(bool overridden)
{
    m_cursorOverridden = overridden;
}

#if 0 // BKTODO:
void ChromeClientImpl::postAccessibilityNotification(AXObject* obj, AXObjectCache::AXNotification notification)
{
    // Alert assistive technology about the accessibility object notification.
    if (!obj || !obj->document())
        return;

    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(obj->document()->axObjectCacheOwner().frame());
    if (webframe && webframe->client())
        webframe->client()->postAccessibilityEvent(WebAXObject(obj), toWebAXEvent(notification));
}
#endif

String ChromeClientImpl::acceptLanguages()
{
    ASSERT(false); // BKTODO: return m_webView->client()->acceptLanguages();
    return String();
}

GraphicsLayerFactory* ChromeClientImpl::graphicsLayerFactory() const
{
    ASSERT(false); // BKTODO: return m_webView->graphicsLayerFactory();
    return nullptr;
}

void ChromeClientImpl::attachRootGraphicsLayer(GraphicsLayer* rootLayer, LocalFrame* localRoot)
{
    ASSERT(false); // BKTODO:
#if 0
    // FIXME: For top-level frames we still use the WebView as a WebWidget. This
    // special case will be removed when top-level frames get WebFrameWidgets.
    if (localRoot->isMainFrame()) {
        m_webView->setRootGraphicsLayer(rootLayer);
    } else {
        WebLocalFrameImpl* webFrame = WebLocalFrameImpl::fromFrame(localRoot);
        // FIXME: The following conditional is only needed for staging until the
        // Chromium patch lands that instantiates a WebFrameWidget.
        if (!webFrame->frameWidget()) {
            m_webView->setRootGraphicsLayer(rootLayer);
            return;
        }
        ASSERT(webFrame);
        ASSERT(webFrame->frameWidget());
        toWebFrameWidgetImpl(webFrame->frameWidget())->setRootGraphicsLayer(rootLayer);
    }
#endif
}

void ChromeClientImpl::didPaint(const PaintArtifact& paintArtifact)
{
    ASSERT(false); // BKTODO:
#if 0
    // TODO(jbroman): This doesn't handle OOPIF correctly. We probably need a
    // branch for WebFrameWidget, like attachRootGraphicsLayer.
    m_webView->paintArtifactCompositor().update(paintArtifact);
#endif
}

void ChromeClientImpl::attachCompositorAnimationTimeline(WebCompositorAnimationTimeline* compositorTimeline, LocalFrame* localRoot)
{
    ASSERT(false); // BKTODO:
#if 0
    // FIXME: For top-level frames we still use the WebView as a WebWidget. This
    // special case will be removed when top-level frames get WebFrameWidgets.
    if (localRoot->isMainFrame()) {
        m_webView->attachCompositorAnimationTimeline(compositorTimeline);
    } else {
        WebLocalFrameImpl* webFrame = WebLocalFrameImpl::fromFrame(localRoot);
        // FIXME: The following conditional is only needed for staging until the
        // Chromium patch lands that instantiates a WebFrameWidget.
        if (!webFrame->frameWidget()) {
            m_webView->attachCompositorAnimationTimeline(compositorTimeline);
            return;
        }
        ASSERT(webFrame);
        ASSERT(webFrame->frameWidget());
        toWebFrameWidgetImpl(webFrame->frameWidget())->attachCompositorAnimationTimeline(compositorTimeline);
    }
#endif
}

void ChromeClientImpl::detachCompositorAnimationTimeline(WebCompositorAnimationTimeline* compositorTimeline, LocalFrame* localRoot)
{
    ASSERT(false); // BKTODO:
#if 0
    // FIXME: For top-level frames we still use the WebView as a WebWidget. This
    // special case will be removed when top-level frames get WebFrameWidgets.
    if (localRoot->isMainFrame()) {
        m_webView->detachCompositorAnimationTimeline(compositorTimeline);
    } else {
        WebLocalFrameImpl* webFrame = WebLocalFrameImpl::fromFrame(localRoot);
        // FIXME: The following conditional is only needed for staging until the
        // Chromium patch lands that instantiates a WebFrameWidget.
        if (!webFrame->frameWidget()) {
            m_webView->detachCompositorAnimationTimeline(compositorTimeline);
            return;
        }
        ASSERT(webFrame);
        ASSERT(webFrame->frameWidget());
        toWebFrameWidgetImpl(webFrame->frameWidget())->detachCompositorAnimationTimeline(compositorTimeline);
    }
#endif
}

void ChromeClientImpl::enterFullScreenForElement(Element* element)
{
    ASSERT(false); // BKTODO: m_webView->enterFullScreenForElement(element);
}

void ChromeClientImpl::exitFullScreenForElement(Element* element)
{
    ASSERT(false); // BKTODO: m_webView->exitFullScreenForElement(element);
}

void ChromeClientImpl::clearCompositedSelection()
{
    ASSERT(false); // BKTODO: m_webView->clearCompositedSelection();
}

void ChromeClientImpl::updateCompositedSelection(const CompositedSelection& selection)
{
    ASSERT(false); // BKTODO: m_webView->updateCompositedSelection(WebSelection(selection));
}

bool ChromeClientImpl::hasOpenedPopup() const
{
    ASSERT(false); // BKTODO: return m_webView->hasOpenedPopup();
    return false;
}

PassRefPtrWillBeRawPtr<PopupMenu> ChromeClientImpl::openPopupMenu(LocalFrame& frame, HTMLSelectElement& select)
{
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    notifyPopupOpeningObservers();
    if (WebViewImpl::useExternalPopupMenus())
        return adoptRefWillBeNoop(new ExternalPopupMenu(frame, select, *m_webView));

    ASSERT(RuntimeEnabledFeatures::pagePopupEnabled());
    return PopupMenuImpl::create(this, select);
#endif
}

PagePopup* ChromeClientImpl::openPagePopup(PagePopupClient* client)
{
    ASSERT(false); // BKTODO: return m_webView->openPagePopup(client);
    return nullptr;
}

void ChromeClientImpl::closePagePopup(PagePopup* popup)
{
    ASSERT(false); // BKTODO: m_webView->closePagePopup(popup);
}

DOMWindow* ChromeClientImpl::pagePopupWindowForTesting() const
{
    ASSERT(false); // BKTODO: return m_webView->pagePopupWindow();
    return nullptr;
}

bool ChromeClientImpl::shouldOpenModalDialogDuringPageDismissal(const DialogType& dialogType, const String& dialogMessage, Document::PageDismissalType dismissalType) const
{
    ASSERT(false); // BKTODO:
#if 0
    const char* kDialogs[] = {"alert", "confirm", "prompt"};
    int dialog = static_cast<int>(dialogType);
    ASSERT_WITH_SECURITY_IMPLICATION(0 <= dialog);
    ASSERT_WITH_SECURITY_IMPLICATION(dialog < static_cast<int>(WTF_ARRAY_LENGTH(kDialogs)));

    const char* kDismissals[] = {"beforeunload", "pagehide", "unload"};
    int dismissal = static_cast<int>(dismissalType) - 1; // Exclude NoDismissal.
    ASSERT_WITH_SECURITY_IMPLICATION(0 <= dismissal);
    ASSERT_WITH_SECURITY_IMPLICATION(dismissal < static_cast<int>(WTF_ARRAY_LENGTH(kDismissals)));

    Platform::current()->histogramEnumeration("Renderer.ModalDialogsDuringPageDismissal", dismissal * WTF_ARRAY_LENGTH(kDialogs) + dialog, WTF_ARRAY_LENGTH(kDialogs) * WTF_ARRAY_LENGTH(kDismissals));

    String message = String("Blocked ") + kDialogs[dialog] + "('" + dialogMessage + "') during " + kDismissals[dismissal] + ".";
    m_webView->mainFrame()->addMessageToConsole(WebConsoleMessage(WebConsoleMessage::LevelError, message));
#endif

    return false;
}

void ChromeClientImpl::needTouchEvents(bool needsTouchEvents)
{
    // BKTODO: m_webView->hasTouchEventHandlers(needsTouchEvents);
}

void ChromeClientImpl::setTouchAction(TouchAction touchAction)
{
    ASSERT(false); // BKTODO:
#if 0
    if (WebViewClient* client = m_webView->client())
        client->setTouchAction(static_cast<WebTouchAction>(touchAction));
#endif
}

bool ChromeClientImpl::requestPointerLock()
{
    ASSERT(false); // BKTODO: return m_webView->requestPointerLock();
    return false;
}

void ChromeClientImpl::requestPointerUnlock()
{
    ASSERT(false); // BKTODO: return m_webView->requestPointerUnlock();
}

void ChromeClientImpl::annotatedRegionsChanged()
{
    ASSERT(false); // BKTODO:
#if 0
    if (WebViewClient* client = m_webView->client())
        client->draggableRegionsChanged();
#endif
}

void ChromeClientImpl::didAssociateFormControls(const WillBeHeapVector<RefPtrWillBeMember<Element>>& elements, LocalFrame* frame)
{
    ASSERT(false); // BKTODO:
#if 0
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(frame);
    if (webframe->autofillClient())
        webframe->autofillClient()->didAssociateFormControls(elements);
#endif
}

void ChromeClientImpl::didCancelCompositionOnSelectionChange()
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->didCancelCompositionOnSelectionChange();
#endif
}

void ChromeClientImpl::willSetInputMethodState()
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->resetInputMethod();
#endif
}

void ChromeClientImpl::didUpdateTextOfFocusedElementByNonUserInput()
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->didUpdateTextOfFocusedElementByNonUserInput();
#endif
}

void ChromeClientImpl::showImeIfNeeded()
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->showImeIfNeeded();
#endif
}

void ChromeClientImpl::showUnhandledTapUIIfNeeded(IntPoint tappedPositionInViewport, Node* tappedNode, bool pageChanged)
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->showUnhandledTapUIIfNeeded(WebPoint(tappedPositionInViewport), WebNode(tappedNode), pageChanged);
#endif
}

void ChromeClientImpl::onMouseDown(Node* mouseDownNode)
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->onMouseDown(WebNode(mouseDownNode));
#endif
}

void ChromeClientImpl::handleKeyboardEventOnTextField(HTMLInputElement& inputElement, KeyboardEvent& event)
{
    ASSERT(false); // BKTODO:
#if 0
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(inputElement.document().frame());
    if (webframe->autofillClient())
        webframe->autofillClient()->textFieldDidReceiveKeyDown(WebInputElement(&inputElement), WebKeyboardEventBuilder(event));
#endif
}

void ChromeClientImpl::didChangeValueInTextField(HTMLFormControlElement& element)
{
    ASSERT(false); // BKTODO:
#if 0
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(element.document().frame());
    if (webframe->autofillClient())
        webframe->autofillClient()->textFieldDidChange(WebFormControlElement(&element));

    m_webView->pageImportanceSignals()->setHadFormInteraction();
#endif
}

void ChromeClientImpl::didEndEditingOnTextField(HTMLInputElement& inputElement)
{
    ASSERT(false); // BKTODO:
#if 0
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(inputElement.document().frame());
    if (webframe->autofillClient())
        webframe->autofillClient()->textFieldDidEndEditing(WebInputElement(&inputElement));
#endif
}

void ChromeClientImpl::openTextDataListChooser(HTMLInputElement& input)
{
    ASSERT(false); // BKTODO:
#if 0
    notifyPopupOpeningObservers();
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(input.document().frame());
    if (webframe->autofillClient())
        webframe->autofillClient()->openTextDataListChooser(WebInputElement(&input));
#endif
}

void ChromeClientImpl::textFieldDataListChanged(HTMLInputElement& input)
{
    ASSERT(false); // BKTODO:
#if 0
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(input.document().frame());
    if (webframe->autofillClient())
        webframe->autofillClient()->dataListOptionsChanged(WebInputElement(&input));
#endif
}

void ChromeClientImpl::ajaxSucceeded(LocalFrame* frame)
{
    ASSERT(false); // BKTODO:
#if 0
    WebLocalFrameImpl* webframe = WebLocalFrameImpl::fromFrame(frame);
    if (webframe->autofillClient())
        webframe->autofillClient()->ajaxSucceeded();
#endif
}

void ChromeClientImpl::registerViewportLayers() const
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->rootGraphicsLayer() && m_webView->layerTreeView())
        m_webView->page()->frameHost().visualViewport().registerLayersWithTreeView(m_webView->layerTreeView());
#endif
}

void ChromeClientImpl::didUpdateTopControls() const
{
    ASSERT(false); // BKTODO: m_webView->didUpdateTopControls();
}

void ChromeClientImpl::registerPopupOpeningObserver(PopupOpeningObserver* observer)
{
    ASSERT(observer);
    m_popupOpeningObservers.append(observer);
}

void ChromeClientImpl::unregisterPopupOpeningObserver(PopupOpeningObserver* observer)
{
    size_t index = m_popupOpeningObservers.find(observer);
    ASSERT(index != kNotFound);
    m_popupOpeningObservers.remove(index);
}

void ChromeClientImpl::notifyPopupOpeningObservers() const
{
    const Vector<PopupOpeningObserver*> observers(m_popupOpeningObservers);
    for (const auto& observer : observers)
        observer->willOpenPopup();
}

FloatSize ChromeClientImpl::elasticOverscroll() const
{
    ASSERT(false); // BKTODO: return m_webView->elasticOverscroll();
    return FloatSize();
}

void ChromeClientImpl::didObserveNonGetFetchFromScript() const
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->pageImportanceSignals())
        m_webView->pageImportanceSignals()->setIssuedNonGetFetchFromScript();
#endif
}

PassOwnPtr<WebFrameScheduler> ChromeClientImpl::createFrameScheduler()
{
    ASSERT(false); // BKTODO: return m_webView->scheduler()->createFrameScheduler().release();
    return nullptr;
}

} // namespace blink
