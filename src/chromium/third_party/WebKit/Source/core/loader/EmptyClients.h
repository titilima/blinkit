// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: EmptyChromeClient.h
// Description: EmptyChromeClient Class
//      Author: Ziming Li
//     Created: 2019-02-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006 Eric Seidel (eric@webkit.org)
 * Copyright (C) 2008, 2009, 2010, 2011, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Copyright (C) 2012 Samsung Electronics. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef EmptyClients_h
#define EmptyClients_h

#include "core/CoreExport.h"
#include "core/editing/commands/UndoStep.h"
#include "core/loader/FrameLoaderClient.h"
#include "core/page/ChromeClient.h"
#include "core/page/ContextMenuClient.h"
#include "core/page/DragClient.h"
#include "core/page/EditorClient.h"
#include "core/page/Page.h"
#include "core/page/SpellCheckerClient.h"
#include "platform/DragImage.h"
#include "platform/geometry/FloatPoint.h"
#include "platform/geometry/IntRect.h"
#include "platform/heap/Handle.h"
#include "platform/network/ResourceError.h"
#include "platform/text/TextCheckerClient.h"
#include "public/platform/WebFocusType.h"
#include "public/platform/WebFrameScheduler.h"
#include "public/platform/WebScreenInfo.h"
#include "wtf/Forward.h"

/*
 This file holds empty Client stubs for use by WebCore.
 Viewless element needs to create a dummy Page->LocalFrame->FrameView tree for use in parsing or executing JavaScript.
 This tree depends heavily on Clients (usually provided by WebKit classes).

 This file was first created for SVGImage as it had no way to access the current Page (nor should it,
 since Images are not tied to a page).
 See http://bugs.webkit.org/show_bug.cgi?id=5971 for the original discussion about this file.

 Ideally, whenever you change a Client class, you should add a stub here.
 Brittle, yes.  Unfortunate, yes.  Hopefully temporary.
*/

namespace blink {

class CORE_EXPORT EmptyChromeClient : public ChromeClient {
public:
    static PassOwnPtrWillBeRawPtr<EmptyChromeClient> create() { return adoptPtrWillBeNoop(new EmptyChromeClient); }

    ~EmptyChromeClient() override {}
    void chromeDestroyed() override {}

    void* webView() const override { return nullptr; }
    void setWindowRect(const IntRect&) override {}
    IntRect windowRect() override { return IntRect(); }

    IntRect pageRect() override { return IntRect(); }

    void focus() override {}

    bool canTakeFocus(WebFocusType) override { return false; }
    void takeFocus(WebFocusType) override {}

    void focusedNodeChanged(Node*, Node*) override {}
    Page* createWindow(LocalFrame*, const FrameLoadRequest&, const WindowFeatures&, NavigationPolicy, ShouldSetOpener) override { return nullptr; }
    void show(NavigationPolicy) override {}

    void didOverscroll(const FloatSize&, const FloatSize&, const FloatPoint&, const FloatSize&) override {}

    bool hadFormInteraction() const override { return false; }

    void setToolbarsVisible(bool) override {}
    bool toolbarsVisible() override { return false; }

    void setStatusbarVisible(bool) override {}
    bool statusbarVisible() override { return false; }

    void setScrollbarsVisible(bool) override {}
    bool scrollbarsVisible() override { return false; }

    void setMenubarVisible(bool) override {}
    bool menubarVisible() override { return false; }

    void setResizable(bool) override {}

    bool shouldReportDetailedMessageForSource(LocalFrame&, const String&) override { return false; }
    void addMessageToConsole(LocalFrame*, MessageSource, MessageLevel, const String&, unsigned, const String&, const String&) override {}

    bool canOpenBeforeUnloadConfirmPanel() override { return false; }
    bool openBeforeUnloadConfirmPanelDelegate(LocalFrame*, const String&, bool) override { return true; }

    void closeWindowSoon() override {}

    bool openJavaScriptAlertDelegate(LocalFrame*, const String&) override { return false; }
    bool openJavaScriptConfirmDelegate(LocalFrame*, const String&) override { return false; }
    bool openJavaScriptPromptDelegate(LocalFrame*, const String&, const String&, String&) override { return false; }

    bool hasOpenedPopup() const override { return false; }
    PassRefPtrWillBeRawPtr<PopupMenu> openPopupMenu(LocalFrame&, HTMLSelectElement&) override;

    void setStatusbarText(const String&) override {}

    bool tabsToLinks() override { return false; }

    IntRect windowResizerRect() const override { return IntRect(); }

    void invalidateRect(const IntRect&) override {}
    void scheduleAnimation(Widget*) override {}

    IntRect viewportToScreen(const IntRect& r) const override { return r; }
    WebScreenInfo screenInfo() const override { return WebScreenInfo(); }
    void contentsSizeChanged(LocalFrame*, const IntSize&) const override {}

    void showMouseOverURL(const HitTestResult&) override {}

    void setToolTip(const String&, TextDirection) override {}

    void printDelegate(LocalFrame*) override {}

    void enumerateChosenDirectory(FileChooser*) override {}

    PassOwnPtrWillBeRawPtr<ColorChooser> openColorChooser(LocalFrame*, ColorChooserClient*, const Color&) override;
    PassRefPtrWillBeRawPtr<DateTimeChooser> openDateTimeChooser(DateTimeChooserClient*, const DateTimeChooserParameters&) override;
    void openTextDataListChooser(HTMLInputElement&) override;

    void openFileChooser(LocalFrame*, PassRefPtr<FileChooser>) override;

    void setCursor(const Cursor&, LocalFrame* localRoot) override {}
    Cursor lastSetCursorForTesting() const override { return pointerCursor(); }

    void attachRootGraphicsLayer(GraphicsLayer*, LocalFrame* localRoot) override {}

    void needTouchEvents(bool) override {}
    void setTouchAction(TouchAction) override {}

    void didAssociateFormControls(const WillBeHeapVector<RefPtrWillBeMember<Element>>&, LocalFrame*) override {}

    void annotatedRegionsChanged() override {}
    String acceptLanguages() override;

    void registerPopupOpeningObserver(PopupOpeningObserver*) override {}
    void unregisterPopupOpeningObserver(PopupOpeningObserver*) override {}

    PassOwnPtr<WebFrameScheduler> createFrameScheduler() override;
};

class CORE_EXPORT EmptyFrameLoaderClient : public FrameLoaderClient {
    WTF_MAKE_NONCOPYABLE(EmptyFrameLoaderClient);
    USING_FAST_MALLOC_WILL_BE_REMOVED(EmptyFrameLoaderClient);
public:
    static PassOwnPtrWillBeRawPtr<EmptyFrameLoaderClient> create() { return adoptPtrWillBeNoop(new EmptyFrameLoaderClient); }
    ~EmptyFrameLoaderClient() override {}

    bool hasWebView() const override { return true; } // mainly for assertions

    bool inShadowTree() const override { return false; }

    Frame* opener() const override { return 0; }
    void setOpener(Frame*) override {}

    void willBeDetached() override {}
    void detached(FrameDetachType) override {}
    void frameFocused() const override {}

    void dispatchWillSendRequest(DocumentLoader*, unsigned long, ResourceRequest&, const ResourceResponse&) override {}
    void dispatchDidReceiveResponse(DocumentLoader*, unsigned long, const ResourceResponse&) override {}
    void dispatchDidFinishLoading(DocumentLoader*, unsigned long) override {}
    void dispatchDidLoadResourceFromMemoryCache(const ResourceRequest&, const ResourceResponse&) override {}

    void dispatchDidHandleOnloadEvents() override {}
    void dispatchDidReceiveServerRedirectForProvisionalLoad() override {}
    void dispatchWillClose() override {}
    void dispatchDidStartProvisionalLoad(double triggeringEventTime) override {}
    void dispatchDidReceiveTitle(const String&) override {}
    void dispatchDidChangeIcons(IconType) override {}
    void dispatchDidCommitLoad(HistoryItem*, HistoryCommitType) override {}
    void dispatchDidFailProvisionalLoad(const ResourceError&, HistoryCommitType) override {}
    void dispatchDidFailLoad(const ResourceError&, HistoryCommitType) override {}
    void dispatchDidFinishDocumentLoad(bool) override {}
    void dispatchDidFinishLoad() override {}
    void dispatchDidChangeThemeColor() override {}

    NavigationPolicy decidePolicyForNavigation(const ResourceRequest&, DocumentLoader*, NavigationType, NavigationPolicy, bool) override;
    bool hasPendingNavigation() override;

    void dispatchWillSendSubmitEvent(Element*) override;
    void dispatchWillSubmitForm(Element*) override;

    void didStartLoading(LoadStartType) override {}
    void progressEstimateChanged(double) override {}
    void didStopLoading() override {}

    void loadURLExternally(const ResourceRequest&, NavigationPolicy, const String&, bool) override {}

    PassRefPtrWillBeRawPtr<DocumentLoader> createDocumentLoader(LocalFrame*, const ResourceRequest&, const SubstituteData&) override;

    String userAgent() override { return ""; }

    String doNotTrackValue() override { return String(); }

    void transitionToCommittedForNewPage() override {}

    bool navigateBackForward(int offset) const override { return false; }
    void didDisplayInsecureContent() override {}
    void didRunInsecureContent(SecurityOrigin*, const KURL&) override {}
    void didDetectXSS(const KURL&, bool) override {}
    void didDispatchPingLoader(const KURL&) override {}
    void didDisplayContentWithCertificateErrors(const KURL&, const CString&, const WebURL& mainResourceUrl, const CString& mainResourceSecurityInfo) override {}
    void didRunContentWithCertificateErrors(const KURL&, const CString&, const WebURL& mainResourceUrl, const CString& mainResourceSecurityInfo) override {}
    void selectorMatchChanged(const Vector<String>&, const Vector<String>&) override {}
    bool canCreatePluginWithoutRenderer(const String& mimeType) const override { return false; }

    ObjectContentType objectContentType(const KURL&, const String&, bool) override { return ObjectContentType(); }

    void didCreateNewDocument() override {}
    void dispatchDidClearWindowObjectInMainWorld() override {}
    void documentElementAvailable() override {}

    bool allowScriptExtension(const String& extensionName, int extensionGroup, int worldId) override { return false; }

    WebCookieJar* cookieJar() const override { return 0; }

    void didRequestAutocomplete(HTMLFormElement*) override;

    bool isControlledByServiceWorker(DocumentLoader&) override { return false; }
    int64_t serviceWorkerID(DocumentLoader&) override { return -1; }

protected:
    EmptyFrameLoaderClient() {}
};

class CORE_EXPORT EmptyTextCheckerClient : public TextCheckerClient {
    DISALLOW_NEW();
public:
    ~EmptyTextCheckerClient() { }

    void checkSpellingOfString(const String&, int*, int*) override {}
    void checkGrammarOfString(const String&, Vector<GrammarDetail>&, int*, int*) override {}
    void requestCheckingOfString(PassRefPtrWillBeRawPtr<TextCheckingRequest>) override;
};

class EmptySpellCheckerClient : public SpellCheckerClient {
    WTF_MAKE_NONCOPYABLE(EmptySpellCheckerClient); USING_FAST_MALLOC(EmptySpellCheckerClient);
public:
    EmptySpellCheckerClient() {}
    ~EmptySpellCheckerClient() override {}

    bool isContinuousSpellCheckingEnabled() override { return false; }
    void toggleContinuousSpellChecking() override {}

    TextCheckerClient& textChecker() override { return m_textCheckerClient; }

    void updateSpellingUIWithMisspelledWord(const String&) override {}
    void showSpellingUI(bool) override {}
    bool spellingUIIsShowing() override { return false; }

private:
    EmptyTextCheckerClient m_textCheckerClient;
};

class EmptyEditorClient final : public EditorClient {
    WTF_MAKE_NONCOPYABLE(EmptyEditorClient); USING_FAST_MALLOC(EmptyEditorClient);
public:
    EmptyEditorClient() {}
    ~EmptyEditorClient() override {}

    void respondToChangedContents() override {}
    void respondToChangedSelection(LocalFrame*, SelectionType) override {}

    bool canCopyCut(LocalFrame*, bool defaultValue) const override { return defaultValue; }
    bool canPaste(LocalFrame*, bool defaultValue) const override { return defaultValue; }

    bool handleKeyboardEvent() override { return false; }
};

class EmptyContextMenuClient final : public ContextMenuClient {
    WTF_MAKE_NONCOPYABLE(EmptyContextMenuClient); USING_FAST_MALLOC(EmptyContextMenuClient);
public:
    EmptyContextMenuClient() {}
    ~EmptyContextMenuClient() override {}
    void showContextMenu(const ContextMenu*) override {}
    void clearContextMenu() override {}
};

class EmptyDragClient final : public DragClient {
    WTF_MAKE_NONCOPYABLE(EmptyDragClient); USING_FAST_MALLOC(EmptyDragClient);
public:
    EmptyDragClient() {}
    ~EmptyDragClient() override {}
    DragDestinationAction actionMaskForDrag(DragData*) override { return DragDestinationActionNone; }
    void startDrag(DragImage*, const IntPoint&, const IntPoint&, DataTransfer*, LocalFrame*, bool) override {}
};

CORE_EXPORT void fillWithEmptyClients(Page::PageClients&);

} // namespace blink

#endif // EmptyClients_h
