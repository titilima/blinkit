// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: FrameLoader.cpp
// Description: FrameLoader Class
//      Author: Ziming Li
//     Created: 2019-02-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 * Copyright (C) 2008 Alp Toker <alp@atoker.com>
 * Copyright (C) Research In Motion Limited 2009. All rights reserved.
 * Copyright (C) 2011 Kris Jordan <krisjordan@gmail.com>
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/loader/FrameLoader.h"

#include "bindings/core/v8/ScriptController.h"
#include "core/HTMLNames.h"
#include "core/dom/Document.h"
#include "core/dom/Element.h"
#include "core/dom/ViewportDescription.h"
#include "core/editing/Editor.h"
#include "core/editing/commands/UndoStack.h"
#include "core/events/GestureEvent.h"
#include "core/events/KeyboardEvent.h"
#include "core/events/MouseEvent.h"
#include "core/events/PageTransitionEvent.h"
#include "core/fetch/ResourceFetcher.h"
#include "core/fetch/ResourceLoader.h"
#include "core/frame/FrameHost.h"
#include "core/frame/FrameView.h"
#include "core/frame/LocalDOMWindow.h"
#include "core/frame/LocalFrame.h"
#include "core/frame/Settings.h"
#include "core/frame/VisualViewport.h"
#include "core/html/HTMLFormElement.h"
#include "core/html/parser/HTMLParserIdioms.h"
#include "core/input/EventHandler.h"
#include "core/inspector/ConsoleMessage.h"
#include "core/inspector/InspectorInstrumentation.h"
#include "core/loader/DocumentLoadTiming.h"
#include "core/loader/DocumentLoader.h"
#include "core/loader/FormSubmission.h"
#include "core/loader/FrameLoadRequest.h"
#include "core/loader/FrameLoaderClient.h"
#include "core/loader/LinkLoader.h"
#include "core/loader/MixedContentChecker.h"
#include "core/loader/NavigationScheduler.h"
#include "core/loader/NetworkHintsInterface.h"
#include "core/page/ChromeClient.h"
#include "core/page/CreateWindow.h"
#include "core/page/Page.h"
#include "core/page/WindowFeatures.h"
#include "core/page/scrolling/ScrollingCoordinator.h"
#include "core/svg/graphics/SVGImage.h"
#include "core/xml/parser/XMLDocumentParser.h"
#include "platform/Logging.h"
#include "platform/PluginScriptForbiddenScope.h"
#include "platform/UserGestureIndicator.h"
#include "platform/network/HTTPParsers.h"
#include "platform/network/ResourceRequest.h"
#include "platform/scroll/ScrollAnimatorBase.h"
#include "platform/weborigin/SecurityOrigin.h"
#include "platform/weborigin/SecurityPolicy.h"
#include "public/platform/WebURLRequest.h"
#include "wtf/TemporaryChange.h"
#include "wtf/text/CString.h"
#include "wtf/text/WTFString.h"

using blink::WebURLRequest;

namespace blink {

using namespace HTMLNames;

bool isBackForwardLoadType(FrameLoadType type)
{
    return type == FrameLoadTypeBackForward || type == FrameLoadTypeInitialHistoryLoad;
}

ResourceRequest FrameLoader::resourceRequestForReload(FrameLoadType frameLoadType,
    const KURL& overrideURL, ClientRedirectPolicy clientRedirectPolicy)
{
    return ResourceRequest();
}

FrameLoader::FrameLoader(LocalFrame* frame)
    : m_frame(frame)
    , m_loadType(FrameLoadTypeStandard)
    , m_inStopAllLoaders(false)
    , m_checkTimer(this, &FrameLoader::checkTimerFired)
    , m_didAccessInitialDocument(false)
    , m_didAccessInitialDocumentTimer(this, &FrameLoader::didAccessInitialDocumentTimerFired)
    , m_dispatchingDidClearWindowObjectInMainWorld(false)
{
}

FrameLoader::~FrameLoader()
{
    // Nothing
}

DEFINE_TRACE(FrameLoader)
{
    visitor->trace(m_frame);
    visitor->trace(m_documentLoader);
    visitor->trace(m_provisionalDocumentLoader);
    visitor->trace(m_deferredHistoryLoad);
}

void FrameLoader::init()
{
    // init() may dispatch JS events, so protect a reference to m_frame.
    RefPtrWillBeRawPtr<LocalFrame> protect(m_frame.get());
    ResourceRequest initialRequest(KURL(ParsedURLString, emptyString()));
    initialRequest.setRequestContext(WebURLRequest::RequestContextInternal);
    initialRequest.setFrameType(m_frame->isMainFrame() ? WebURLRequest::FrameTypeTopLevel : WebURLRequest::FrameTypeNested);
    m_provisionalDocumentLoader = client()->createDocumentLoader(m_frame, initialRequest, SubstituteData());
    m_provisionalDocumentLoader->startLoadingMainResource();
    m_frame->document()->cancelParsing();
    m_stateMachine.advanceTo(FrameLoaderStateMachine::DisplayingInitialEmptyDocument);
}

FrameLoaderClient* FrameLoader::client() const
{
    return static_cast<FrameLoaderClient*>(m_frame->client());
}

void FrameLoader::setDefersLoading(bool defers)
{
    if (m_documentLoader)
        m_documentLoader->setDefersLoading(defers);
    if (m_provisionalDocumentLoader)
        m_provisionalDocumentLoader->setDefersLoading(defers);

    if (Document* document = m_frame->document()) {
        if (defers)
            document->suspendScheduledTasks();
        else
            document->resumeScheduledTasks();
    }

    if (!defers) {
        if (m_deferredHistoryLoad) {
            load(FrameLoadRequest(nullptr, m_deferredHistoryLoad->m_request),
                m_deferredHistoryLoad->m_loadType, m_deferredHistoryLoad->m_item.get(),
                m_deferredHistoryLoad->m_historyLoadType);
            m_deferredHistoryLoad.clear();
        }
        m_frame->navigationScheduler().startTimer();
        scheduleCheckCompleted();
    }
}

void FrameLoader::saveScrollState()
{
    // Nothing to do.
}

void FrameLoader::dispatchUnloadEvent()
{
    saveScrollState();

#ifdef BLINKIT_CRAWLER_ONLY
    if (m_frame->document())
        m_frame->document()->dispatchUnloadEvents();
#else
    if (m_frame->document() && !SVGImage::isInSVGImage(m_frame->document()))
        m_frame->document()->dispatchUnloadEvents();

    if (Page* page = m_frame->page())
        page->undoStack().didUnloadFrame(*m_frame);
#endif
}

void FrameLoader::didExplicitOpen()
{
    // Calling document.open counts as committing the first real document load.
    if (!m_stateMachine.committedFirstRealDocumentLoad())
        m_stateMachine.advanceTo(FrameLoaderStateMachine::CommittedFirstRealLoad);

    // Prevent window.open(url) -- eg window.open("about:blank") -- from blowing away results
    // from a subsequent window.document.open / window.document.write call.
    // Canceling redirection here works for all cases because document.open
    // implicitly precedes document.write.
    m_frame->navigationScheduler().cancel();
}

void FrameLoader::clear()
{
    // clear() is called during (Local)Frame detachment or when
    // reusing a FrameLoader by putting a new Document within it
    // (DocumentLoader::ensureWriter().)
    if (m_stateMachine.creatingInitialEmptyDocument())
        return;

#ifndef BLINKIT_CRAWLER_ONLY
    if (!m_frame->IsCrawlerFrame())
    {
        m_frame->editor().clear();
        m_frame->document()->removeFocusedElementOfSubtree(m_frame->document());
        m_frame->selection().prepareForDestruction();
        m_frame->eventHandler().clear();
        if (m_frame->view())
            m_frame->view()->clear();
    }
#endif

    m_frame->script().enableEval();

    m_frame->navigationScheduler().cancel();

    m_checkTimer.stop();

    if (m_stateMachine.isDisplayingInitialEmptyDocument())
        m_stateMachine.advanceTo(FrameLoaderStateMachine::CommittedFirstRealLoad);
}

void FrameLoader::receivedMainResourceRedirect(const KURL& newURL)
{
    client()->dispatchDidReceiveServerRedirectForProvisionalLoad();
}

void FrameLoader::setHistoryItemStateForCommit(HistoryCommitType historyCommitType, HistoryNavigationType navigationType)
{
    // Nothing to do.
}

static HistoryCommitType loadTypeToCommitType(FrameLoadType type)
{
    switch (type) {
    case FrameLoadTypeStandard:
        return StandardCommit;
    case FrameLoadTypeInitialInChildFrame:
    case FrameLoadTypeInitialHistoryLoad:
        return InitialCommitInChildFrame;
    case FrameLoadTypeBackForward:
        return BackForwardCommit;
    default:
        break;
    }
    return HistoryInertCommit;
}

void FrameLoader::receivedFirstData()
{
    if (m_stateMachine.creatingInitialEmptyDocument())
        return;

    if (!m_stateMachine.committedMultipleRealLoads() && m_loadType == FrameLoadTypeStandard)
        m_stateMachine.advanceTo(FrameLoaderStateMachine::CommittedMultipleRealLoads);

    client()->dispatchDidCommitLoad(nullptr, StandardCommit);

#ifndef BLINKIT_CRAWLER_ONLY
    if (!m_frame->IsCrawlerFrame())
        m_frame->page()->didCommitLoad(m_frame);
#endif
    dispatchDidClearDocumentOfWindowObject();
}

void FrameLoader::didBeginDocument(bool dispatch)
{
    ASSERT(m_frame);
    ASSERT(m_frame->document());
    ASSERT(m_frame->document()->fetcher());
    m_frame->document()->setReadyState(Document::Loading);

    if (dispatch)
        dispatchDidClearDocumentOfWindowObject();

    Settings* settings = m_frame->document()->settings();
    if (settings) {
        m_frame->document()->fetcher()->setImagesEnabled(settings->imagesEnabled());
        m_frame->document()->fetcher()->setAutoLoadImages(settings->loadsImagesAutomatically());
    }

    if (m_documentLoader) {
        String headerContentLanguage = m_documentLoader->response().httpHeaderField(HTTPNames::Content_Language);
        if (!headerContentLanguage.isEmpty()) {
            size_t commaIndex = headerContentLanguage.find(',');
            headerContentLanguage.truncate(commaIndex); // kNotFound == -1 == don't truncate
            headerContentLanguage = headerContentLanguage.stripWhiteSpace(isHTMLSpace<UChar>);
            if (!headerContentLanguage.isEmpty())
                m_frame->document()->setContentLanguage(AtomicString(headerContentLanguage));
        }
    }

    client()->didCreateNewDocument();
}

void FrameLoader::finishedParsing()
{
    if (m_stateMachine.creatingInitialEmptyDocument())
        return;

    FrameView *view = nullptr;
#ifndef BLINKIT_CRAWLER_ONLY
    if (!m_frame->IsCrawlerFrame())
        view = m_frame->view();
#endif

    // This can be called from the LocalFrame's destructor, in which case we shouldn't protect ourselves
    // because doing so will cause us to re-enter the destructor when protector goes out of scope.
    // Null-checking the FrameView indicates whether or not we're in the destructor.
    RefPtrWillBeRawPtr<LocalFrame> protect(nullptr != view ? m_frame : nullptr);

    if (client())
        client()->dispatchDidFinishDocumentLoad(m_documentLoader ? m_documentLoader->isCommittedButEmpty() : true);

    checkCompleted();

#ifndef BLINKIT_CRAWLER_ONLY
    if (view)
    {
        // Check if the scrollbars are really needed for the content.
        // If not, remove them, relayout, and repaint.
        view->restoreScrollbar();
        processFragment(m_frame->document()->url(), NavigationToDifferentDocument);
    }
#endif
}

static bool allDescendantsAreComplete(Frame* frame)
{
    return true;
}

bool FrameLoader::allAncestorsAreComplete() const
{
    return true;
}

static bool shouldComplete(Document* document)
{
    if (!document->frame())
        return false;
    if (document->parsing() || document->isInDOMContentLoaded())
        return false;
    if (!document->haveImportsLoaded())
        return false;
    if (document->fetcher()->requestCount())
        return false;
    if (document->isDelayingLoadEvent())
        return false;
    return allDescendantsAreComplete(document->frame());
}

static bool shouldSendFinishNotification(LocalFrame* frame)
{
    // Don't send stop notifications for inital empty documents, since they don't generate start notifications.
    if (!frame->loader().stateMachine()->committedFirstRealDocumentLoad())
        return false;

    // Don't send didFinishLoad more than once per DocumentLoader.
    if (frame->loader().documentLoader()->sentDidFinishLoad())
        return false;

    // We might have declined to run the load event due to an imminent content-initiated navigation.
    if (!frame->document()->loadEventFinished())
        return false;

    // An event might have restarted a child frame.
    if (!allDescendantsAreComplete(frame))
        return false;
    return true;
}

static bool shouldSendCompleteNotification(LocalFrame* frame)
{
    // FIXME: We might have already sent stop notifications and be re-completing.
    if (!frame->isLoading())
        return false;
    // Only send didStopLoading() if there are no navigations in progress at all,
    // whether committed, provisional, or pending.
    return frame->loader().documentLoader()->sentDidFinishLoad() && !frame->loader().provisionalDocumentLoader() && !frame->loader().client()->hasPendingNavigation();
}

void FrameLoader::checkCompleted()
{
    RefPtrWillBeRawPtr<LocalFrame> protect(m_frame.get());
    if (!shouldComplete(m_frame->document()))
        return;

    // OK, completed.
    m_frame->document()->setReadyState(Document::Complete);
    if (m_frame->document()->loadEventStillNeeded())
        m_frame->document()->implicitClose();

    m_frame->navigationScheduler().startTimer();

#ifndef BLINKIT_CRAWLER_ONLY
    if (m_frame->view())
        m_frame->view()->handleLoadCompleted();
#endif

    // The readystatechanged or load event may have disconnected this frame.
    if (!m_frame->client())
        return;

    if (shouldSendFinishNotification(m_frame)) {
        m_documentLoader->setSentDidFinishLoad();
        client()->dispatchDidFinishLoad();
        // Finishing the load can detach the frame when running layout tests.
        if (!m_frame->client())
            return;
    }

    if (shouldSendCompleteNotification(m_frame)) {
        // Retry restoring scroll offset since finishing loading disables content
        // size clamping.
        restoreScrollPositionAndViewState();

        m_loadType = FrameLoadTypeStandard;
        m_frame->localDOMWindow()->finishedLoading();
    }
}

void FrameLoader::checkTimerFired(Timer<FrameLoader>*)
{
    if (Page* page = m_frame->page()) {
        if (page->defersLoading())
            return;
    }
    checkCompleted();
}

void FrameLoader::scheduleCheckCompleted()
{
    if (!m_checkTimer.isActive())
        m_checkTimer.startOneShot(0, BLINK_FROM_HERE);
}

Frame* FrameLoader::opener()
{
    return client() ? client()->opener() : 0;
}

void FrameLoader::setOpener(LocalFrame* opener)
{
    // If the frame is already detached, the opener has already been cleared.
    if (client())
        client()->setOpener(opener);
}

bool FrameLoader::allowPlugins(ReasonForCallingAllowPlugins reason)
{
    // With Oilpan, a FrameLoader might be accessed after the
    // FrameHost has been detached. FrameClient will not be
    // accessible, so bail early.
    if (!client())
        return false;
    Settings* settings = m_frame->settings();
    bool allowed = client()->allowPlugins(settings && settings->pluginsEnabled());
    if (!allowed && reason == AboutToInstantiatePlugin)
        client()->didNotAllowPlugins();
    return allowed;
}

void FrameLoader::detachDocumentLoader(RefPtrWillBeMember<DocumentLoader>& loader)
{
    if (!loader)
        return;

    loader->detachFromFrame();
    loader = nullptr;
}

void FrameLoader::setReferrerForFrameRequest(ResourceRequest& request, ShouldSendReferrer shouldSendReferrer, Document* originDocument)
{
    if (!originDocument)
        return;
    // Anchor elements with the 'referrerpolicy' attribute will have
    // already set the referrer on the request.
    if (request.didSetHTTPReferrer())
        return;
    if (shouldSendReferrer == NeverSendReferrer)
        return;

    // Always use the initiating document to generate the referrer.
    // We need to generateReferrer(), because we haven't enforced ReferrerPolicy or https->http
    // referrer suppression yet.
    Referrer referrer = SecurityPolicy::generateReferrer(originDocument->referrerPolicy(), request.url(), originDocument->outgoingReferrer());

    request.setHTTPReferrer(referrer);
    RefPtr<SecurityOrigin> referrerOrigin = SecurityOrigin::createFromString(referrer.referrer);
    request.addHTTPOriginIfNeeded(referrerOrigin);
}

bool FrameLoader::prepareRequestForThisFrame(FrameLoadRequest& request)
{
    // If no origin Document* was specified, skip remaining security checks and assume the caller has fully initialized the FrameLoadRequest.
    if (!request.originDocument())
        return true;

    KURL url = request.resourceRequest().url();
    if (m_frame->script().executeScriptIfJavaScriptURL(url))
        return false;

    if (!request.originDocument()->securityOrigin()->canDisplay(url)) {
        reportLocalLoadFailed(m_frame, url.elidedString());
        return false;
    }

    if (!request.form() && request.frameName().isEmpty())
        request.setFrameName(m_frame->document()->baseTarget());
    return true;
}

static NavigationType determineNavigationType(FrameLoadType frameLoadType, bool isFormSubmission, bool haveEvent)
{
    bool isReload = frameLoadType == FrameLoadTypeReload || frameLoadType == FrameLoadTypeReloadFromOrigin;
    bool isBackForward = isBackForwardLoadType(frameLoadType);
    if (isFormSubmission)
        return (isReload || isBackForward) ? NavigationTypeFormResubmitted : NavigationTypeFormSubmitted;
    if (haveEvent)
        return NavigationTypeLinkClicked;
    if (isReload)
        return NavigationTypeReload;
    if (isBackForward)
        return NavigationTypeBackForward;
    return NavigationTypeOther;
}

static WebURLRequest::RequestContext determineRequestContextFromNavigationType(const NavigationType navigationType)
{
    switch (navigationType) {
    case NavigationTypeLinkClicked:
        return WebURLRequest::RequestContextHyperlink;

    case NavigationTypeOther:
        return WebURLRequest::RequestContextLocation;

    case NavigationTypeFormResubmitted:
    case NavigationTypeFormSubmitted:
        return WebURLRequest::RequestContextForm;

    case NavigationTypeBackForward:
    case NavigationTypeReload:
        return WebURLRequest::RequestContextInternal;
    }
    ASSERT_NOT_REACHED();
    return WebURLRequest::RequestContextHyperlink;
}

void FrameLoader::load(const FrameLoadRequest& passedRequest, FrameLoadType frameLoadType,
    HistoryItem* historyItem, HistoryLoadType historyLoadType)
{
    ASSERT(m_frame->document());

    RefPtrWillBeRawPtr<LocalFrame> protect(m_frame.get());

    if (m_inStopAllLoaders)
        return;

    FrameLoadRequest request(passedRequest);
    request.resourceRequest().setHasUserGesture(UserGestureIndicator::processingUserGesture());

    if (!prepareRequestForThisFrame(request))
        return;

    setReferrerForFrameRequest(request.resourceRequest(), request.shouldSendReferrer(), request.originDocument());
    startLoad(request, FrameLoadTypeStandard, NavigationPolicyCurrentTab);
}

SubstituteData FrameLoader::defaultSubstituteDataForURL(const KURL& url)
{
    return SubstituteData();
}

void FrameLoader::reportLocalLoadFailed(LocalFrame* frame, const String& url)
{
    ASSERT(!url.isEmpty());
    if (!frame)
        return;

    frame->document()->addConsoleMessage(ConsoleMessage::create(SecurityMessageSource, ErrorMessageLevel, "Not allowed to load local resource: " + url));
}

void FrameLoader::stopAllLoaders()
{
    if (m_frame->document()->pageDismissalEventBeingDispatched() != Document::NoDismissal)
        return;

    // If this method is called from within this method, infinite recursion can occur (3442218). Avoid this.
    if (m_inStopAllLoaders)
        return;

    // Calling stopLoading() on the provisional document loader can blow away
    // the frame from underneath.
    RefPtrWillBeRawPtr<LocalFrame> protect(m_frame.get());

    m_inStopAllLoaders = true;

    m_frame->document()->suppressLoadEvent();
    if (m_provisionalDocumentLoader)
        m_provisionalDocumentLoader->stopLoading();
    if (m_documentLoader)
        m_documentLoader->stopLoading();
    m_frame->document()->cancelParsing();

    detachDocumentLoader(m_provisionalDocumentLoader);

    m_checkTimer.stop();
    m_frame->navigationScheduler().cancel();

    m_inStopAllLoaders = false;
}

void FrameLoader::didAccessInitialDocument()
{
    // We only need to notify the client once, and only for the main frame.
    if (isLoadingMainFrame() && !m_didAccessInitialDocument) {
        m_didAccessInitialDocument = true;
        // Notify asynchronously, since this is called within a JavaScript security check.
        m_didAccessInitialDocumentTimer.startOneShot(0, BLINK_FROM_HERE);
    }
}

void FrameLoader::didAccessInitialDocumentTimerFired(Timer<FrameLoader>*)
{
    if (client())
        client()->didAccessInitialDocument();
}

void FrameLoader::notifyIfInitialDocumentAccessed()
{
    if (m_didAccessInitialDocumentTimer.isActive()) {
        m_didAccessInitialDocumentTimer.stop();
        didAccessInitialDocumentTimerFired(0);
    }
}

bool FrameLoader::prepareForCommit()
{
    PluginScriptForbiddenScope forbidPluginDestructorScripting;
    RefPtrWillBeRawPtr<DocumentLoader> pdl = m_provisionalDocumentLoader;

    if (m_documentLoader) {
        client()->dispatchWillClose();
        dispatchUnloadEvent();
    }
    // The previous calls to dispatchUnloadEvent() and detachChildren() can
    // execute arbitrary script via things like unload events. If the executed
    // script intiates a new load or causes the current frame to be detached,
    // we need to abandon the current load.
    if (pdl != m_provisionalDocumentLoader)
        return false;
    if (m_documentLoader) {
        FrameNavigationDisabler navigationDisabler(*m_frame);
        detachDocumentLoader(m_documentLoader);
    }
    // detachFromFrame() will abort XHRs that haven't completed, which can
    // trigger event listeners for 'abort'. These event listeners might detach
    // the frame.
    // TODO(dcheng): Investigate if this can be moved above the check that
    // m_provisionalDocumentLoader hasn't changed.
    if (!m_frame->client())
        return false;
    // No more events will be dispatched so detach the Document.
    // TODO(yoav): Should we also be nullifying domWindow's document (or domWindow) since the doc is now detached?
    if (m_frame->document())
        m_frame->document()->detach();
    m_documentLoader = m_provisionalDocumentLoader.release();

    return true;
}

void FrameLoader::commitProvisionalLoad()
{
    ASSERT(m_frame->IsCrawlerFrame() || client()->hasWebView());
    RefPtrWillBeRawPtr<LocalFrame> protect(m_frame.get());

    if (!prepareForCommit())
        return;

#ifndef BLINKIT_CRAWLER_ONLY
    if (isLoadingMainFrame() && !m_frame->IsCrawlerFrame())
        m_frame->page()->chromeClient().needTouchEvents(false);
#endif

    client()->transitionToCommittedForNewPage();
    m_frame->navigationScheduler().cancel();
#ifndef BLINKIT_CRAWLER_ONLY
    if (!m_frame->IsCrawlerFrame())
        m_frame->editor().clearLastEditCommand();
#endif

    // If we are still in the process of initializing an empty document then
    // its frame is not in a consistent state for rendering, so avoid setJSStatusBarText
    // since it may cause clients to attempt to render the frame.
    if (!m_stateMachine.creatingInitialEmptyDocument()) {
        DOMWindow* window = m_frame->domWindow();
        window->setStatus(String());
        window->setDefaultStatus(String());
    }
}

bool FrameLoader::isLoadingMainFrame() const
{
    return m_frame->isMainFrame();
}

FrameLoadType FrameLoader::loadType() const
{
    return m_loadType;
}

void FrameLoader::restoreScrollPositionAndViewState()
{
    // Nothing to do.
}

String FrameLoader::userAgent() const
{
    return client()->userAgent();
}

void FrameLoader::detach()
{
#if !ENABLE(OILPAN)
    // The caller must protect a reference to m_frame.
    ASSERT(m_frame->refCount() > 1);
#endif
    detachDocumentLoader(m_documentLoader);
    detachDocumentLoader(m_provisionalDocumentLoader);
}

void FrameLoader::receivedMainResourceError(DocumentLoader* loader, const ResourceError& error)
{
    // Retain because the stop may release the last reference to it.
    RefPtrWillBeRawPtr<LocalFrame> protect(m_frame.get());
    RefPtrWillBeRawPtr<DocumentLoader> protectDocumentLoader(loader);

    HistoryCommitType historyCommitType = loadTypeToCommitType(m_loadType);
    if (loader == m_provisionalDocumentLoader) {
        client()->dispatchDidFailProvisionalLoad(error, historyCommitType);
        if (loader != m_provisionalDocumentLoader)
            return;
        detachDocumentLoader(m_provisionalDocumentLoader);
    } else {
        ASSERT(loader == m_documentLoader);
        if (m_frame->document()->parser())
            m_frame->document()->parser()->stopParsing();
        m_documentLoader->setSentDidFinishLoad();
        if (!m_provisionalDocumentLoader && m_frame->isLoading())
            client()->dispatchDidFailLoad(error, historyCommitType);
    }
    checkCompleted();
}

void FrameLoader::processFragment(const KURL& url, LoadStartType loadStartType)
{
#ifndef BLINKIT_CRAWLER_ONLY
    FrameView* view = m_frame->view();
    if (!view)
        return;

    // If scroll position is restored from history fragment then we should not override it unless
    // this is a same document reload.
    bool shouldScrollToFragment = (loadStartType == NavigationWithinSameDocument && !isBackForwardLoadType(m_loadType))
        || (documentLoader() && !documentLoader()->initialScrollState().didRestoreFromHistory);

    view->processUrlFragment(url, shouldScrollToFragment ?
        FrameView::UrlFragmentScroll : FrameView::UrlFragmentDontScroll);
#endif
}

bool FrameLoader::shouldClose(bool isReload)
{
#ifdef BLINKIT_CRAWLER_ONLY
    return true;
#else
    if (m_frame->IsCrawlerFrame())
        return true;

    Page *page = m_frame->page();
    if (!page || !page->chromeClient().canOpenBeforeUnloadConfirmPanel())
        return true;

    NavigationDisablerForBeforeUnload navigationDisabler;
    bool didAllowNavigation = false;
    return m_frame->document()->dispatchBeforeUnloadEvent(page->chromeClient(), isReload, didAllowNavigation);
#endif
}

bool FrameLoader::shouldContinueForNavigationPolicy(const ResourceRequest& request, const SubstituteData& substituteData,
    DocumentLoader* loader, ContentSecurityPolicyDisposition shouldCheckMainWorldContentSecurityPolicy,
    NavigationType type, NavigationPolicy policy, bool replacesCurrentHistoryItem)
{
    // Don't ask if we are loading an empty URL.
    if (request.url().isEmpty() || substituteData.isValid())
        return true;

    policy = client()->decidePolicyForNavigation(request, loader, type, policy, replacesCurrentHistoryItem);
    if (policy == NavigationPolicyCurrentTab)
        return true;
    if (policy == NavigationPolicyIgnore)
        return false;
    if (policy == NavigationPolicyHandledByClient) {
        // Mark the frame as loading since the embedder is handling the navigation.
        return false;
    }
    if (!LocalDOMWindow::allowPopUp(*m_frame) && !UserGestureIndicator::processingUserGesture())
        return false;
    client()->loadURLExternally(request, policy, String(), replacesCurrentHistoryItem);
    return false;
}

void FrameLoader::startLoad(FrameLoadRequest& frameLoadRequest, FrameLoadType type, NavigationPolicy navigationPolicy)
{
    ASSERT(m_frame->IsCrawlerFrame() || client()->hasWebView());
    // Lots of things in this function can detach the LocalFrame (stopAllLoaders, beforeunload handlers, etc.), so protect the frame.
    RefPtrWillBeRawPtr<LocalFrame> protect(m_frame.get());
    if (m_frame->document()->pageDismissalEventBeingDispatched() != Document::NoDismissal)
        return;

    NavigationType navigationType = determineNavigationType(type, frameLoadRequest.resourceRequest().httpBody() || frameLoadRequest.form(), frameLoadRequest.triggeringEvent());
    frameLoadRequest.resourceRequest().setRequestContext(determineRequestContextFromNavigationType(navigationType));
    frameLoadRequest.resourceRequest().setFrameType(m_frame->isMainFrame() ? WebURLRequest::FrameTypeTopLevel : WebURLRequest::FrameTypeNested);
    ResourceRequest& request = frameLoadRequest.resourceRequest();
    if (!shouldContinueForNavigationPolicy(request, frameLoadRequest.substituteData(), nullptr, frameLoadRequest.shouldCheckMainWorldContentSecurityPolicy(), navigationType, navigationPolicy, type == FrameLoadTypeReplaceCurrentItem))
        return;
    if (!shouldClose(navigationType == NavigationTypeReload))
        return;

    m_frame->document()->cancelParsing();

    if (m_provisionalDocumentLoader) {
        m_provisionalDocumentLoader->stopLoading();
        detachDocumentLoader(m_provisionalDocumentLoader);
    }

#ifndef BLINKIT_CRAWLER_ONLY
    // beforeunload fired above, and detaching a DocumentLoader can fire
    // events, which can detach this frame.
    if (!m_frame->IsCrawlerFrame() && !m_frame->host())
        return;
#endif

    m_provisionalDocumentLoader = client()->createDocumentLoader(m_frame, request, frameLoadRequest.substituteData().isValid() ? frameLoadRequest.substituteData() : defaultSubstituteDataForURL(request.url()));
    m_provisionalDocumentLoader->setNavigationType(navigationType);
    m_provisionalDocumentLoader->setReplacesCurrentHistoryItem(type == FrameLoadTypeReplaceCurrentItem);
    m_provisionalDocumentLoader->setIsClientRedirect(frameLoadRequest.clientRedirect() == ClientRedirect);

    m_frame->navigationScheduler().cancel();
    m_checkTimer.stop();

    m_loadType = type;

    if (frameLoadRequest.form())
        client()->dispatchWillSubmitForm(frameLoadRequest.form());

    if (m_provisionalDocumentLoader->isClientRedirect())
        m_provisionalDocumentLoader->appendRedirect(m_frame->document()->url());
    m_provisionalDocumentLoader->appendRedirect(m_provisionalDocumentLoader->request().url());
    double triggeringEventTime = frameLoadRequest.triggeringEvent() ? frameLoadRequest.triggeringEvent()->platformTimeStamp() : 0;
    client()->dispatchDidStartProvisionalLoad(triggeringEventTime);
    ASSERT(m_provisionalDocumentLoader);
    m_provisionalDocumentLoader->startLoadingMainResource();
}

void FrameLoader::applyUserAgent(ResourceRequest& request)
{
    String userAgent = this->userAgent();
    ASSERT(!userAgent.isNull());
    request.setHTTPUserAgent(AtomicString(userAgent));
}

bool FrameLoader::shouldTreatURLAsSameAsCurrent(const KURL& url) const
{
    return false;
}

bool FrameLoader::shouldTreatURLAsSrcdocDocument(const KURL& url) const
{
    return false;
}

void FrameLoader::dispatchDocumentElementAvailable()
{
    client()->documentElementAvailable();
}

void FrameLoader::dispatchDidClearDocumentOfWindowObject()
{
    if (!m_frame->script().canExecuteScripts(NotAboutToExecuteScript))
        return;

    if (m_dispatchingDidClearWindowObjectInMainWorld)
        return;
    TemporaryChange<bool>
        inDidClearWindowObject(m_dispatchingDidClearWindowObjectInMainWorld, true);
    // We just cleared the document, not the entire window object, but for the
    // embedder that's close enough.
    client()->dispatchDidClearWindowObjectInMainWorld();
}

void FrameLoader::dispatchDidClearWindowObjectInMainWorld()
{
    if (!m_frame->script().canExecuteScripts(NotAboutToExecuteScript))
        return;

    if (m_dispatchingDidClearWindowObjectInMainWorld)
        return;
    TemporaryChange<bool>
        inDidClearWindowObject(m_dispatchingDidClearWindowObjectInMainWorld, true);
    client()->dispatchDidClearWindowObjectInMainWorld();
}

bool FrameLoader::shouldEnforceStrictMixedContentChecking() const
{
    return false;
}

} // namespace blink
