// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document.cpp
// Description: Document Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2011, 2012 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) 2008, 2009, 2011, 2012 Google Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) Research In Motion Limited 2010-2011. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "document.h"

#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/renderer/core/css/selector_query.h"
#include "third_party/blink/renderer/core/dom/document_init.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/document_parser.h"
#include "third_party/blink/renderer/core/dom/element_data_cache.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/core/dom/events/event.h"
#include "third_party/blink/renderer/core/dom/node_traversal.h"
#include "third_party/blink/renderer/core/dom/live_node_list_base.h"
#include "third_party/blink/renderer/core/event_type_names.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/html/parser/html_document_parser.h"
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"
#include "third_party/blink/renderer/core/loader/navigation_scheduler.h"
#include "third_party/blink/renderer/core/loader/text_resource_decoder_builder.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"
#include "third_party/blink/renderer/platform/network/http_parsers.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace BlinKit;

namespace blink {

uint64_t Document::m_globalTreeVersion = 0;

Document::Document(const DocumentInit &initializer)
    : ContainerNode(nullptr, GetConstructionType(initializer))
    , TreeScope(*this)
    , m_domTreeVersion(++m_globalTreeVersion)
    , m_frame(initializer.GetFrame())
    , m_domWindow(nullptr != m_frame ? m_frame->DomWindow() : nullptr)
    , m_elementDataCacheClearTimer(GetTaskRunner(TaskType::kInternalUserInteraction), this, &Document::ElementDataCacheClearTimerFired)
{
    if (m_frame)
    {
#ifndef BLINKIT_CRAWLER_ONLY
        ASSERT(false); // BKTODO: DCHECK(frame_->GetPage());
        ProvideContextFeaturesToDocumentFrom(*this, *frame_->GetPage());
#endif

        m_fetcher = m_frame->Loader().GetDocumentLoader()->Fetcher();
#ifndef BLINKIT_CRAWLER_ONLY
        FrameFetchContext::ProvideDocumentToContext(fetcher_->Context(), this);

        // TODO(dcheng): Why does this need to check that DOMWindow is non-null?
        CustomElementRegistry* registry =
            frame_->DomWindow() ? frame_->DomWindow()->MaybeCustomElements()
            : nullptr;
        if (registry && registration_context_)
            registry->Entangle(registration_context_);
#endif
    }
#ifndef BLINKIT_CRAWLER_ONLY
    else if (imports_controller_) {
        fetcher_ = FrameFetchContext::CreateFetcherFromDocument(this);
    }
    else {
        fetcher_ = ResourceFetcher::Create(nullptr);
    }
#endif
    m_lifecycle.AdvanceTo(DocumentLifecycle::kInactive);
}

Document::~Document(void)
{
    FastCleanupChildren();
#ifndef BLINKIT_CRAWLER_ONLY
    DCHECK(!GetLayoutView());
#endif
}

void Document::Abort(void)
{
    CancelParsing();
    CheckCompletedInternal();
}

bool Document::CanAcceptChild(const Node &newChild, const Node *next, const Node *oldChild, ExceptionState &exceptionState) const
{
    ASSERT(!(nullptr != next && nullptr != oldChild));
    if (nullptr != oldChild && oldChild->getNodeType() == newChild.getNodeType())
        return true;

    int numDoctypes = 0;
    int numElements = 0;
    bool hasDoctypeAfterReferenceNode = false;
    bool hasElementAfterReferenceNode = false;

    // First, check how many doctypes and elements we have, not counting
    // the child we're about to remove.
    bool sawReferenceNode = false;
    for (Node &child : NodeTraversal::ChildrenOf(*this))
    {
        if (nullptr != oldChild && *oldChild == child)
        {
            sawReferenceNode = true;
            continue;
        }
        if (&child == next)
            sawReferenceNode = true;

        switch (child.getNodeType())
        {
            case kDocumentTypeNode:
                ++numDoctypes;
                hasDoctypeAfterReferenceNode = sawReferenceNode;
                break;
            case kElementNode:
                ++numElements;
                hasElementAfterReferenceNode = sawReferenceNode;
                break;
            default:
                break;
        }
    }

    // Then, see how many doctypes and elements might be added by the new child.
    if (newChild.IsDocumentFragment())
    {
        for (Node &child : NodeTraversal::ChildrenOf(ToDocumentFragment(newChild)))
        {
            switch (child.getNodeType())
            {
                case kAttributeNode:
                case kCdataSectionNode:
                case kDocumentFragmentNode:
                case kDocumentNode:
                case kTextNode:
                    exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
                        "Nodes of type '" + newChild.nodeName() +
                        "' may not be inserted inside nodes of type '#document'.");
                    return false;
                case kCommentNode:
                case kProcessingInstructionNode:
                    break;
                case kDocumentTypeNode:
                    ++numDoctypes;
                    break;
                case kElementNode:
                    ++numElements;
                    if (hasDoctypeAfterReferenceNode)
                    {
                        exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
                            "Can't insert an element before a doctype.");
                        return false;
                    }
                    break;
            }
        }
    }
    else
    {
        switch (newChild.getNodeType())
        {
            case kAttributeNode:
            case kCdataSectionNode:
            case kDocumentFragmentNode:
            case kDocumentNode:
            case kTextNode:
                exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
                    "Nodes of type '" + newChild.nodeName() +
                    "' may not be inserted inside nodes of type '#document'.");
                return false;
            case kCommentNode:
            case kProcessingInstructionNode:
                return true;
            case kDocumentTypeNode:
                ++numDoctypes;
                if (numElements > 0 && !hasElementAfterReferenceNode)
                {
                    exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
                        "Can't insert a doctype before the root element.");
                    return false;
                }
                break;
            case kElementNode:
                ++numElements;
                if (hasDoctypeAfterReferenceNode)
                {
                    exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
                        "Can't insert an element before a doctype.");
                    return false;
                }
                break;
        }
    }

    if (numElements > 1 || numDoctypes > 1)
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kHierarchyRequestError,
            String::Format("Only one %s on document allowed.", numElements > 1 ? "element" : "doctype"));
        return false;
    }

    return true;
}

void Document::CancelParsing(void)
{
    DetachParser();
    SetParsingState(kFinishedParsing);
    SetReadyState(kComplete);
    SuppressLoadEvent();
}

bool Document::CanExecuteScripts(ReasonForCallingCanExecuteScripts reason)
{
    ASSERT(nullptr != GetFrame());
    BKLOG("// BKTODO: CanExecuteScripts");
    return false;
}

void Document::CheckCompleted(void)
{
    if (CheckCompletedInternal())
        m_frame->Loader().DidFinishNavigation();
}

bool Document::CheckCompletedInternal(void)
{
    if (!ShouldComplete())
        return false;

    if (m_frame)
    {
        m_frame->Client()->RunScriptsAtDocumentIdle();

        // Injected scripts may have disconnected this frame.
        if (!m_frame)
            return false;

        // Check again, because runScriptsAtDocumentIdle() may have delayed the load
        // event.
        if (!ShouldComplete())
            return false;
    }

    // OK, completed. Fire load completion events as needed.
    SetReadyState(kComplete);
    if (LoadEventStillNeeded())
        ImplicitClose();

    // The readystatechanged or load event may have disconnected this frame.
    if (!m_frame || !m_frame->IsAttached())
        return false;
    m_frame->GetNavigationScheduler().StartTimer();

#ifndef BLINKIT_CRAWLER_ONLY
    View()->HandleLoadCompleted();
#endif

    // No need to repeat if we've already notified this load as finished.
    if (!Loader()->SentDidFinishLoad())
    {
#ifndef BLINKIT_CRAWLER_ONLY
        if (frame_->IsMainFrame())
            GetViewportData().GetViewportDescription().ReportMobilePageStats(frame_);
#endif
        Loader()->SetSentDidFinishLoad();
        m_frame->Client()->DispatchDidFinishLoad();
        if (!m_frame)
            return false;

#ifndef BLINKIT_CRAWLER_ONLY
        AnchorElementMetrics::MaybeReportViewportMetricsOnLoad(*this);
#endif
    }

    return true;
}

void Document::ChildrenChanged(const ChildrenChange &change)
{
    ContainerNode::ChildrenChanged(change);
    m_documentElement = ElementTraversal::FirstWithin(*this);
}

Node* Document::Clone(Document &factory, CloneChildrenFlag flag) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

Document* Document::ContextDocument(void) const
{
    if (m_frame)
        return const_cast<Document *>(this);
    return nullptr;
}

std::shared_ptr<DocumentParser> Document::CreateParser(void)
{
    return HTMLDocumentParser::Create(*this);
}

void Document::DetachParser(void)
{
    if (m_parser)
    {
        m_parser->Detach();
        m_parser.reset();
    }
}

void Document::DispatchDidReceiveTitle(void)
{
    m_frame->Client()->DispatchDidReceiveTitle(m_title);
}

void Document::DispatchUnloadEvents(void)
{
    if (m_parser)
        m_parser->StopParsing();

    if (kLoadEventNotRun == m_loadEventProgress)
        return;

    if (m_loadEventProgress <= kUnloadEventInProgress)
    {
#ifndef BLINKIT_CRAWLER_ONLY
        Element* current_focused_element = FocusedElement();
        if (auto* input = ToHTMLInputElementOrNull(current_focused_element))
            input->EndEditing();
#endif
        if (m_loadEventProgress < kPageHideInProgress)
        {
#ifndef BLINKIT_CRAWLER_ONLY
            m_loadEventProgress = kPageHideInProgress;
            if (LocalDOMWindow* window = domWindow()) {
                const TimeTicks pagehide_event_start = CurrentTimeTicks();
                window->DispatchEvent(
                    *PageTransitionEvent::Create(EventTypeNames::pagehide, false),
                    this);
                const TimeTicks pagehide_event_end = CurrentTimeTicks();
                DEFINE_STATIC_LOCAL(
                    CustomCountHistogram, pagehide_histogram,
                    ("DocumentEventTiming.PageHideDuration", 0, 10000000, 50));
                pagehide_histogram.CountMicroseconds(pagehide_event_end -
                    pagehide_event_start);
            }
            if (!frame_)
                return;

            mojom::PageVisibilityState visibility_state = GetPageVisibilityState();
            load_event_progress_ = kUnloadVisibilityChangeInProgress;
            if (visibility_state != mojom::PageVisibilityState::kHidden) {
                // Dispatch visibilitychange event, but don't bother doing
                // other notifications as we're about to be unloaded.
                const TimeTicks pagevisibility_hidden_event_start = CurrentTimeTicks();
                DispatchEvent(*Event::CreateBubble(EventTypeNames::visibilitychange));
                const TimeTicks pagevisibility_hidden_event_end = CurrentTimeTicks();
                DEFINE_STATIC_LOCAL(CustomCountHistogram, pagevisibility_histogram,
                    ("DocumentEventTiming.PageVibilityHiddenDuration",
                        0, 10000000, 50));
                pagevisibility_histogram.CountMicroseconds(
                    pagevisibility_hidden_event_end -
                    pagevisibility_hidden_event_start);
                DispatchEvent(
                    *Event::CreateBubble(EventTypeNames::webkitvisibilitychange));
            }
            if (!frame_)
                return;
#endif

            m_loadEventProgress = kUnloadEventInProgress;
            m_frame->DomWindow()->DispatchEvent(*Event::Create(event_type_names::kUnload), this);
        }
        m_loadEventProgress = kUnloadEventHandled;
    }

    if (!m_frame)
        return;

    // Don't remove event listeners from a transitional empty document (see
    // https://bugs.webkit.org/show_bug.cgi?id=28716 for more information).
    bool keepEventListeners = nullptr != m_frame->Loader().GetProvisionalDocumentLoader()
        && m_frame->ShouldReuseDefaultView();
    if (!keepEventListeners)
        RemoveAllEventListenersRecursively();
}

void Document::ElementDataCacheClearTimerFired(TimerBase *)
{
    m_elementDataCache.reset();
}

LocalFrame* Document::ExecutingFrame(void)
{
    if (LocalDOMWindow *window = ExecutingWindow())
        return window->GetFrame();
    return nullptr;
}

LocalDOMWindow* Document::ExecutingWindow(void) const
{
    if (LocalDOMWindow *owningWindow = domWindow())
        return owningWindow;
#ifndef BLINKIT_CRAWLER_ONLY
    if (HTMLImportsController* import = ImportsController())
        return import->Master()->domWindow();
#endif
    return nullptr;
}

void Document::FinishedParsing(void)
{
    ScriptableDocumentParser *parser = GetScriptableDocumentParser();
    ASSERT(nullptr == parser || !m_parser->IsParsing());
    ASSERT(nullptr == parser || m_readyState != kLoading);
    SetParsingState(kInDOMContentLoaded);

    // FIXME: DOMContentLoaded is dispatched synchronously, but this should be
    // dispatched in a queued task, see https://crbug.com/425790
    DispatchEvent(*Event::CreateBubble(event_type_names::kDOMContentLoaded));
    SetParsingState(kFinishedParsing);

#if 0 // BKTODO: Check this
    // Ensure Custom Element callbacks are drained before DOMContentLoaded.
    // FIXME: Remove this ad-hoc checkpoint when DOMContentLoaded is dispatched in
    // a queued task, which will do a checkpoint anyway. https://crbug.com/425790
    Microtask::PerformCheckpoint(V8PerIsolateData::MainThreadIsolate());
#endif

    m_wellFormed = nullptr != parser && parser->WellFormed();

    if (LocalFrame *frame = GetFrame())
    {
        // Guarantee at least one call to the client specifying a title. (If
        // |title_| is not empty, then the title has already been dispatched.)
        if (m_title.IsEmpty())
            DispatchDidReceiveTitle();

#ifndef BLINKIT_CRAWLER_ONLY
        ASSERT(false); // BKTODO:
        // Don't update the layout tree if we haven't requested the main resource
        // yet to avoid adding extra latency. Note that the first layout tree update
        // can be expensive since it triggers the parsing of the default stylesheets
        // which are compiled-in.
        const bool mainResourceWasAlreadyRequested = frame->Loader().StateMachine()->CommittedFirstRealDocumentLoad();

        // FrameLoader::finishedParsing() might end up calling
        // Document::implicitClose() if all resource loads are
        // complete. HTMLObjectElements can start loading their resources from post
        // attach callbacks triggered by recalcStyle().  This means if we parse out
        // an <object> tag and then reach the end of the document without updating
        // styles, we might not have yet started the resource load and might fire
        // the window load event too early.  To avoid this we force the styles to be
        // up to date before calling FrameLoader::finishedParsing().  See
        // https://bugs.webkit.org/show_bug.cgi?id=36864 starting around comment 35.
        if (mainResourceWasAlreadyRequested)
            UpdateStyleAndLayoutTree();

        BeginLifecycleUpdatesIfRenderingReady();
#endif

        frame->Loader().FinishedParsing();

#if 0 // BKTODO: Check if necesary.
        frame->GetIdlenessDetector()->DomContentLoadedEventFired();
#endif
    }

    // Schedule dropping of the ElementDataCache. We keep it alive for a while
    // after parsing finishes so that dynamically inserted content can also
    // benefit from sharing optimizations.  Note that we don't refresh the timer
    // on cache access since that could lead to huge caches being kept alive
    // indefinitely by something innocuous like JS setting .innerHTML repeatedly
    // on a timer.
    m_elementDataCacheClearTimer.StartOneShot(TimeDelta::FromSeconds(10), FROM_HERE);

#if 0 // BKTODO: Check if necesary.
    // Parser should have picked up all preloads by now
    fetcher_->ClearPreloads(ResourceFetcher::kClearSpeculativeMarkupPreloads);
    if (!frame_ || frame_->GetSettings()->GetSavePreviousDocumentResources() ==
        SavePreviousDocumentResources::kUntilOnDOMContentLoaded) {
        fetcher_->ClearResourcesFromPreviousFetcher();
    }
#endif
}

Node::ConstructionType Document::GetConstructionType(const DocumentInit &init)
{
    return init.GetFrame()->Client()->IsCrawler() ? kCreateCrawlerDocument : kCreateDocument;
}

ScriptableDocumentParser* Document::GetScriptableDocumentParser(void) const
{
    if (DocumentParser *parser = Parser())
        return parser->AsScriptableDocumentParser();
    return nullptr;
}

SelectorQueryCache& Document::GetSelectorQueryCache(void)
{
    if (!m_selectorQueryCache)
        m_selectorQueryCache = std::make_unique<SelectorQueryCache>();
    return *m_selectorQueryCache;
}

std::shared_ptr<base::SingleThreadTaskRunner> Document::GetTaskRunner(TaskType type)
{
    ASSERT(IsMainThread());
    return Platform::Current()->CurrentThread()->GetTaskRunner();
}

void Document::ImplicitClose(void)
{
    ASSERT(!InStyleRecalc());
    ASSERT(m_parser);

    m_loadEventProgress = kLoadEventInProgress;

    // We have to clear the parser, in case someone document.write()s from the
    // onLoad event handler, as in Radar 3206524.
    DetachParser();

    // JS running below could remove the frame or destroy the LayoutView so we
    // call those two functions repeatedly and don't save them on the stack.

    if (LocalDOMWindow *domWindow = this->domWindow())
        domWindow->DocumentWasClosed();

    LocalFrame *frame = GetFrame();
    if (nullptr == frame)
    {
        m_loadEventProgress = kLoadEventCompleted;
        return;
    }

    frame->Client()->DispatchDidHandleOnloadEvents();

#if 0 // BKTODO: Check if necessary
    // Make sure both the initial layout and reflow happen after the onload
    // fires. This will improve onload scores, and other browsers do it.
    // If they wanna cheat, we can too. -dwh

    if (GetFrame()->GetNavigationScheduler().LocationChangePending() &&
        ElapsedTime() < kCLayoutScheduleThreshold) {
        // Just bail out. Before or during the onload we were shifted to another
        // page.  The old i-Bench suite does this. When this happens don't bother
        // painting or laying out.
        m_loadEventProgress = kLoadEventCompleted;
        return;
    }
#endif

#ifndef BLINKIT_CRAWLER_ONLY
    // We used to force a synchronous display and flush here.  This really isn't
    // necessary and can in fact be actively harmful if pages are loading at a
    // rate of > 60fps
    // (if your platform is syncing flushes and limiting them to 60fps).
    if (!LocalOwner() || (LocalOwner()->GetLayoutObject() &&
        !LocalOwner()->GetLayoutObject()->NeedsLayout())) {
        UpdateStyleAndLayoutTree();

        // Always do a layout after loading if needed.
        if (View() && GetLayoutView() &&
            (!GetLayoutView()->FirstChild() || GetLayoutView()->NeedsLayout()))
            View()->UpdateLayout();
    }

    if (View())
        View()->ScrollAndFocusFragmentAnchor();
#endif

    m_loadEventProgress = kLoadEventCompleted;
}

std::shared_ptr<DocumentParser> Document::ImplicitOpen(void)
{
    RemoveChildren();
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(!m_focusedElement);
#endif

    SetCompatibilityMode(kNoQuirksMode);

    DetachParser();
    m_parser = CreateParser();

    SetParsingState(kParsing);
    SetReadyState(kLoading);
    if (m_loadEventProgress != kLoadEventInProgress && PageDismissalEventBeingDispatched() == kNoDismissal)
        m_loadEventProgress = kLoadEventNotRun;

    return m_parser;
}

void Document::Initialize(void)
{
    assert(m_lifecycle.GetState() == DocumentLifecycle::kInactive);

#ifndef BLINKIT_CRAWLER_ONLY
    if (!ForCrawler())
    {
        layout_view_ = new LayoutView(this);
        SetLayoutObject(layout_view_);

        layout_view_->SetIsInWindow(true);
        layout_view_->SetStyle(StyleResolver::StyleForViewport(*this));
        layout_view_->Compositor()->SetNeedsCompositingUpdate(
            kCompositingUpdateAfterCompositingInputChange);

        {
            ReattachLegacyLayoutObjectList legacy_layout_objects(*this);
            AttachContext context;
            ContainerNode::AttachLayoutTree(context);
            legacy_layout_objects.ForceLegacyLayoutIfNeeded();
        }

        // The TextAutosizer can't update layout view info while the Document is
        // detached, so update now in case anything changed.
        if (TextAutosizer* autosizer = GetTextAutosizer())
            autosizer->UpdatePageInfo();
    }
#endif

    m_frame->DocumentAttached();
    m_lifecycle.AdvanceTo(DocumentLifecycle::kStyleClean);

#ifndef BLINKIT_CRAWLER_ONLY
    if (View())
        View()->DidAttachDocument();

    // Observer(s) should not be initialized until the document is initialized /
    // attached to a frame. Otherwise ContextLifecycleObserver::contextDestroyed
    // wouldn't be fired.
    network_state_observer_ = new NetworkStateObserver(*this);
#endif
}

DocumentLoader* Document::Loader(void) const
{
    if (!m_frame)
        return nullptr;

    ASSERT(m_frame->GetDocument() == this);
    return m_frame->Loader().GetDocumentLoader();
}

void Document::MaybeHandleHttpRefresh(const String &content, HttpRefreshType refreshType)
{
    if (!m_frame)
        return;

    double delay;
    String refresh_url_string;
    if (!ParseHTTPRefresh(content,
        refreshType == kHttpRefreshFromMetaTag
        ? IsHTMLSpace<UChar>
        : nullptr,
        delay, refresh_url_string))
    {
        return;
    }
    ASSERT(false); // BKTODO:
}

String Document::nodeName(void) const
{
    return "#document";
}

std::shared_ptr<DocumentParser> Document::OpenForNavigation(const AtomicString &mimeType, const AtomicString &encoding)
{
    std::shared_ptr<DocumentParser> parser = ImplicitOpen();
    if (parser->NeedsDecoder())
        parser->SetDecoder(BuildTextResourceDecoderFor(this, mimeType, encoding));
    return parser;
}

Document::PageDismissalType Document::PageDismissalEventBeingDispatched(void) const
{
    switch (m_loadEventProgress)
    {
        case kBeforeUnloadEventInProgress:
            return kBeforeUnloadDismissal;
        case kPageHideInProgress:
            return kPageHideDismissal;
        case kUnloadVisibilityChangeInProgress:
            return kUnloadVisibilityChangeDismissal;
        case kUnloadEventInProgress:
            return kUnloadDismissal;

        case kLoadEventNotRun:
        case kLoadEventInProgress:
        case kLoadEventCompleted:
        case kBeforeUnloadEventCompleted:
        case kUnloadEventHandled:
            return kNoDismissal;
    }
    NOTREACHED();
    return kNoDismissal;
}

void Document::ParserInsertedHtmlElement(Element &element)
{
    // When parsing a fragment, its dummy document has a null parser.
    if (!m_parser)
        return;

    m_parser->DocumentElementAvailable();
    if (LocalFrame *frame = GetFrame())
    {
        BKLOG("// BKTODO: frame->Loader().DispatchDocumentElementAvailable();");
        BKLOG("// BKTODO: frame->Loader().RunScriptsAtDocumentElementAvailable();");
    }
}

void Document::SetCompatibilityMode(CompatibilityMode mode)
{
    if (m_compatibilityModeLocked || mode == m_compatibilityMode)
        return;

    m_compatibilityMode = mode;
    if (m_selectorQueryCache)
        m_selectorQueryCache->Invalidate();
}

void Document::SetDoctype(DocumentType *docType)
{
    // This should never be called more than once.
    ASSERT(!m_docType || nullptr == docType);
    m_docType = docType;
    if (m_docType)
    {
#if 0 // BKTODO: Check if necessary
        AdoptIfNeeded(*doc_type_);
#endif
#ifndef BLINKIT_CRAWLER_ONLY
        if (doc_type_->publicId().StartsWithIgnoringASCIICase(
            "-//wapforum//dtd xhtml mobile 1.")) {
            is_mobile_document_ = true;
            style_engine_->ViewportRulesChanged();
        }
#endif
    }
}

void Document::SetEncodingData(const DocumentEncodingData &newData)
{
    // It's possible for the encoding of the document to change while we're
    // decoding data. That can only occur while we're processing the <head>
    // portion of the document. There isn't much user-visible content in the
    // <head>, but there is the <title> element. This function detects that
    // situation and re-decodes the document's title so that the user doesn't see
    // an incorrectly decoded title in the title bar.
    do {
        if (!m_titleElement)
            break;
        ASSERT(false); // BKTODO:
#if 0
        if (Encoding() == newData.Encoding())
            break;
        if (ElementTraversal::FirstWithin(*m_titleElement))
            break;
        if (Encoding() != Latin1Encoding())
            break;
        if (!m_titleElement->textContent().ContainsOnlyLatin1())
            break;

        CString original_bytes = title_element_->textContent().Latin1();
        std::unique_ptr<TextCodec> codec = NewTextCodec(new_data.Encoding());
        String correctly_decoded_title =
            codec->Decode(original_bytes.data(), original_bytes.length(),
                WTF::FlushBehavior::kDataEOF);
        title_element_->setTextContent(correctly_decoded_title);
#endif
    } while (false);

    ASSERT(newData.Encoding().IsValid());
    m_encodingData = newData;

#ifndef BLINKIT_CRAWLER_ONLY
    if (ForCrawler())
        return;

    // FIXME: Should be removed as part of
    // https://code.google.com/p/chromium/issues/detail?id=319643
    bool should_use_visual_ordering = encoding_data_.Encoding().UsesVisualOrdering();
    if (should_use_visual_ordering != visually_ordered_) {
        visually_ordered_ = should_use_visual_ordering;
        SetNeedsStyleRecalc(kSubtreeStyleChange,
            StyleChangeReasonForTracing::Create(
                StyleChangeReason::kVisuallyOrdered));
    }
#endif
}

void Document::SetParsingState(ParsingState parsingState)
{
    m_parsingState = parsingState;

    if (Parsing() && !m_elementDataCache)
        m_elementDataCache = ElementDataCache::Create();
}

void Document::SetReadyState(DocumentReadyState readyState)
{
    if (readyState != m_readyState)
    {
        m_readyState = readyState;
        DispatchEvent(*Event::Create(event_type_names::kReadystatechange));
    }
}

bool Document::ShouldComplete(void)
{
    if (kFinishedParsing != m_parsingState)
        return false;
#ifndef BLINKIT_CRAWLER_ONLY
    if (!HaveImportsLoaded())
        return false;
#endif
    if (0 != m_fetcher->BlockingRequestCount())
        return false;
    if (IsDelayingLoadEvent())
        return false;
    if (kLoadEventInProgress == m_loadEventProgress)
        return false;
    return true;
}

template <unsigned type>
bool ShouldInvalidateNodeListCachesForAttr(const LiveNodeListRegistry &nodeLists, const QualifiedName &attrName)
{
    auto invalidationType = static_cast<NodeListInvalidationType>(type);
    if (nodeLists.ContainsInvalidationType(invalidationType)
        && LiveNodeListBase::ShouldInvalidateTypeOnAttributeChange(invalidationType, attrName))
    {
        return true;
    }
    return ShouldInvalidateNodeListCachesForAttr<type + 1>(nodeLists, attrName);
}

template <>
bool ShouldInvalidateNodeListCachesForAttr<kNumNodeListInvalidationTypes>(const LiveNodeListRegistry &, const QualifiedName &)
{
    return false;
}

bool Document::ShouldInvalidateNodeListCaches(const QualifiedName *attrName) const
{
    if (nullptr != attrName)
    {
        return ShouldInvalidateNodeListCachesForAttr<kDoNotInvalidateOnAttributeChanges + 1>(m_nodeLists, *attrName);
    }

    // If the invalidation is not for an attribute, invalidation is needed if
    // there is any node list present (with any invalidation type).
    return !m_nodeLists.IsEmpty();
}

void Document::Shutdown(void)
{
    if (!IsActive())
        return;

#ifndef BLINKIT_CRAWLER_ONLY
    GetViewportData().Shutdown();
#endif

    // Frame navigation can cause a new Document to be attached. Don't allow that,
    // since that will cause a situation where LocalFrame still has a Document
    // attached after this finishes!  Normally, it shouldn't actually be possible
    // to trigger navigation here.  However, plugins (see below) can cause lots of
    // crazy things to happen, since plugin detach involves nested run loops.
    FrameNavigationDisabler navigationDisabler(*m_frame);
    // Don't allow script to run in the middle of detachLayoutTree() because a
    // detaching Document is not in a consistent state.
    ScriptForbiddenScope forbidScript;

    m_lifecycle.AdvanceTo(DocumentLifecycle::kStopping);
#ifndef BLINKIT_CRAWLER_ONLY
    View()->Dispose();
    // TODO(crbug.com/729196): Trace why LocalFrameView::DetachFromLayout crashes.
    CHECK(!View()->IsAttached());
#endif

#ifndef BLINKIT_CRAWLER_ONLY
    markers_->PrepareForDestruction();

    if (GetPage())
        GetPage()->DocumentDetached(this);

    if (frame_->Client()->GetSharedWorkerRepositoryClient())
        frame_->Client()->GetSharedWorkerRepositoryClient()->DocumentDetached(this);

    // FIXME: consider using PausableObject.
    if (scripted_animation_controller_)
        scripted_animation_controller_->ClearDocumentPointer();
    scripted_animation_controller_.Clear();

    scripted_idle_task_controller_.Clear();

    if (layout_view_)
        layout_view_->SetIsInWindow(false);

    if (RegistrationContext())
        RegistrationContext()->DocumentWasDetached();

    MutationObserver::CleanSlotChangeList(*this);

    hover_element_ = nullptr;
    active_element_ = nullptr;
    autofocus_element_ = nullptr;

    if (focused_element_.Get()) {
        Element* old_focused_element = focused_element_;
        focused_element_ = nullptr;
        if (GetPage())
            GetPage()->GetChromeClient().FocusedNodeChanged(old_focused_element,
                nullptr);
    }
    sequential_focus_navigation_starting_point_ = nullptr;

    layout_view_ = nullptr;
    ContainerNode::DetachLayoutTree();
    // TODO(crbug.com/729196): Trace why LocalFrameView::DetachFromLayout crashes.
    CHECK(!View()->IsAttached());

    GetStyleEngine().DidDetach();

    frame_->GetEventHandlerRegistry().DocumentDetached(*this);
#endif

    // Signal destruction to mutation observers.
    DocumentShutdownNotifier::NotifyContextDestroyed();
    SynchronousMutationNotifier::NotifyContextDestroyed();

    // If this Document is associated with a live DocumentLoader, the
    // DocumentLoader will take care of clearing the FetchContext. Deferring
    // to the DocumentLoader when possible also prevents prematurely clearing
    // the context in the case where multiple Documents end up associated with
    // a single DocumentLoader (e.g., navigating to a javascript: url).
    if (nullptr == Loader())
        m_fetcher->ClearContext();
#ifndef BLINKIT_CRAWLER_ONLY
    // If this document is the master for an HTMLImportsController, sever that
    // relationship. This ensures that we don't leave import loads in flight,
    // thinking they should have access to a valid frame when they don't.
    if (imports_controller_) {
        imports_controller_->Dispose();
        ClearImportsController();
    }

    if (media_query_matcher_)
        media_query_matcher_->DocumentDetached();
#endif

    m_lifecycle.AdvanceTo(DocumentLifecycle::kStopped);
#ifndef BLINKIT_CRAWLER_ONLY
    // TODO(crbug.com/729196): Trace why LocalFrameView::DetachFromLayout crashes.
    CHECK(!View()->IsAttached());
#endif

    // TODO(haraken): Call contextDestroyed() before we start any disruptive
    // operations.
    // TODO(haraken): Currently we call notifyContextDestroyed() only in
    // Document::detachLayoutTree(), which means that we don't call
    // notifyContextDestroyed() for a document that doesn't get detached.
    // If such a document has any observer, the observer won't get
    // a contextDestroyed() notification. This can happen for a document
    // created by DOMImplementation::createDocument().
    ExecutionContext::NotifyContextDestroyed();
#ifndef BLINKIT_CRAWLER_ONLY
    // TODO(crbug.com/729196): Trace why LocalFrameView::DetachFromLayout crashes.
    CHECK(!View()->IsAttached());
#endif

    // This is required, as our LocalFrame might delete itself as soon as it
    // detaches us. However, this violates Node::detachLayoutTree() semantics, as
    // it's never possible to re-attach. Eventually Document::detachLayoutTree()
    // should be renamed, or this setting of the frame to 0 could be made
    // explicit in each of the callers of Document::detachLayoutTree().
    m_frame = nullptr;
}

void Document::SuppressLoadEvent(void)
{
    if (!LoadEventFinished())
        m_loadEventProgress = kLoadEventCompleted;
}

BkURL Document::ValidBaseElementURL(void) const
{
    ASSERT(false); // BKTODO:
    return BkURL();
}

void Document::WillInsertBody(void)
{
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
    if (auto* loader = Loader())
        loader->Fetcher()->LoosenLoadThrottlingPolicy();

    // If we get to the <body> try to resume commits since we should have content
    // to paint now.
    // TODO(esprehn): Is this really optimal? We might start producing frames
    // for very little content, should we wait for some heuristic like
    // isVisuallyNonEmpty() ?
    BeginLifecycleUpdatesIfRenderingReady();
#endif
}

}  // namespace blink
