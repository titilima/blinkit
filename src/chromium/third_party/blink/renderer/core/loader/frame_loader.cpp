// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_loader.cpp
// Description: FrameLoader Class
//      Author: Ziming Li
//     Created: 2019-09-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All rights
 * reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
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

#include "frame_loader.h"

#include "base/single_thread_task_runner.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"
#include "third_party/blink/renderer/core/loader/frame_load_request.h"
#include "third_party/blink/renderer/core/loader/navigation_scheduler.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/substitute_data.h"
#include "third_party/blink/renderer/platform/network/http_names.h"

using namespace BlinKit;

namespace blink {

FrameLoader::FrameLoader(LocalFrame *frame) : m_frame(frame)
{
}

FrameLoader::~FrameLoader(void)
{
    ASSERT(m_detached);
}

bool FrameLoader::CancelProvisionalLoaderForNewNavigation(bool cancelScheduledNavigations)
{
    bool hadPlaceholderClientDocumentLoader = m_provisionalDocumentLoader && !m_provisionalDocumentLoader->DidStart();

    // For placeholder DocumentLoaders, don't send failure callbacks
    // for a placeholder simply being replaced with a new DocumentLoader.
    if (hadPlaceholderClientDocumentLoader)
        m_provisionalDocumentLoader->SetSentDidFinishLoad();

    // This seems to correspond to step 9 of the specification:
    // "9. Abort the active document of browsingContext."
    // https://html.spec.whatwg.org/#navigate
    m_frame->GetDocument()->Abort();
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
    // document.onreadystatechange can fire in Abort(), which can:
    // 1) Detach this frame.
    // 2) Stop the provisional DocumentLoader (i.e window.stop()).
    if (!frame_->GetPage())
        return false;
#endif

    DetachDocumentLoader(m_provisionalDocumentLoader);
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
    // Detaching the provisional DocumentLoader above may leave the frame without
    // any loading DocumentLoader. It can causes the 'load' event to fire, which
    // can be used to detach this frame.
    if (!frame_->GetPage())
        return false;
#endif

    m_frame->SetIsLoading(true);

    // We need to ensure that script initiated navigations are honored.
    if (!hadPlaceholderClientDocumentLoader || cancelScheduledNavigations)
        m_frame->GetNavigationScheduler().Cancel();

    return true;
}

LocalFrameClient* FrameLoader::Client(void) const
{
    return m_frame->Client();
}

void FrameLoader::CommitNavigation(
    const ResourceRequest &request,
    const SubstituteData &substituteData,
    WebFrameLoadType loadType,
    std::unique_ptr<WebDocumentLoader::ExtraData> extraData)
{
    ASSERT(nullptr != m_frame->GetDocument());
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO: DCHECK(Client()->HasWebView());
#endif

    if (m_inStopAllLoaders || !m_frame->IsNavigationAllowed()
        || m_frame->GetDocument()->PageDismissalEventBeingDispatched() != Document::kNoDismissal)
    {
        // Any of the checks above should not be necessary.
        // Unfortunately, in the case of sync IPCs like print() there might be
        // reentrancy and, for example, frame detach happening.
        // See fast/loader/detach-while-printing.html for a repro.
        // TODO(https://crbug.com/862088): we should probably ignore print()
        // call in this case instead.
        return;
    }

    ResourceRequest resourceRequest = request;

    if (!CancelProvisionalLoaderForNewNavigation(false))
        return;

    // TODO(dgozman): get rid of provisional document loader and most of the code
    // below. We should probably call DocumentLoader::CommitNavigation directly.
    m_provisionalDocumentLoader = CreateDocumentLoader(resourceRequest, substituteData, loadType, std::move(extraData));
    m_provisionalDocumentLoader->AppendRedirect(m_provisionalDocumentLoader->Url());

#if 0 // BKTODO: Check if necessary.
    m_frame->GetFrameScheduler()->DidStartProvisionalLoad(frame_->IsMainFrame());
#endif
    Client()->DispatchDidStartProvisionalLoad(m_provisionalDocumentLoader.get(), resourceRequest);

    m_provisionalDocumentLoader->StartLoading();
}

void FrameLoader::CommitProvisionalLoad(void)
{
    if (!PrepareForCommit())
        return;

    Client()->TransitionToCommittedForNewPage();

    m_frame->GetNavigationScheduler().Cancel();
}

std::unique_ptr<DocumentLoader> FrameLoader::CreateDocumentLoader(
    const ResourceRequest &request,
    const SubstituteData &substituteData,
    WebFrameLoadType loadType,
    std::unique_ptr<WebDocumentLoader::ExtraData> extraData)
{
    std::unique_ptr<DocumentLoader> loader = Client()->CreateDocumentLoader(m_frame, request,
        substituteData.IsValid() ? substituteData : DefaultSubstituteDataForURL(request.Url()),
        std::move(extraData));
    loader->SetLoadType(loadType);
    return loader;
}

SubstituteData FrameLoader::DefaultSubstituteDataForURL(const BkURL &url)
{
    ASSERT(url.AsString() != "about:src"); // BKTODO:
    return SubstituteData();
}

void FrameLoader::Detach(void)
{
    DetachDocumentLoader(m_documentLoader);
    DetachDocumentLoader(m_provisionalDocumentLoader);

    m_detached = true;
}

std::unique_ptr<DocumentLoader> FrameLoader::DetachDocumentLoader(std::unique_ptr<DocumentLoader> &loader, bool flushMicrotaskQueue)
{
    if (!loader)
        return nullptr;

    FrameNavigationDisabler navigationDisabler(*m_frame);
    loader->DetachFromFrame(flushMicrotaskQueue);
    return std::move(loader);
}

static void DestroyDocumentLoaderTask(DocumentLoader *loader)
{
    delete loader;
}

void FrameLoader::DetachProvisionalDocumentLoader(DocumentLoader *loader)
{
    ASSERT(m_provisionalDocumentLoader.get() == loader);
    std::unique_ptr<DocumentLoader> detachedLoader = DetachDocumentLoader(m_provisionalDocumentLoader);
    DidFinishNavigation();
    if (detachedLoader)
    {
        std::function<void()> task = std::bind(DestroyDocumentLoaderTask, detachedLoader.release());
        m_frame->GetTaskRunner(TaskType::kInternalLoading)->PostTask(FROM_HERE, task);
    }
}

void FrameLoader::DidFinishNavigation(void)
{
    // We should have either finished the provisional or committed navigation if
    // this is called. Only delcare the whole frame finished if neither is in
    // progress.
    ASSERT((m_documentLoader && m_documentLoader->SentDidFinishLoad()) || !HasProvisionalNavigation());
    if (!m_documentLoader || !m_documentLoader->SentDidFinishLoad() || HasProvisionalNavigation())
        return;

    // This code in this block is meant to prepare a document for display, but
    // this code may also run on a document being unloaded. In that case, which
    // is detectable via protect_provisional_loader_, skip the display work.
    if (m_frame->IsLoading() && !m_protectProvisionalLoader)
    {
        m_frame->SetIsLoading(false);
#ifndef BLINKIT_CRAWLER_ONLY
        // Retry restoring scroll offset since finishing loading disables content
        // size clamping.
        RestoreScrollPositionAndViewState();
#endif
        if (m_documentLoader)
            m_documentLoader->SetLoadType(WebFrameLoadType::kStandard);
        m_frame->DomWindow()->FinishedLoading();
    }
}

void FrameLoader::DispatchDidClearDocumentOfWindowObject(void)
{
    ASSERT(nullptr != m_frame->GetDocument());
    if (m_stateMachine.CreatingInitialEmptyDocument())
        return;
    if (!m_frame->GetDocument()->CanExecuteScripts(kNotAboutToExecuteScript))
        return;

#if 0 // BKTODO: Check if necessary.
    if (dispatching_did_clear_window_object_in_main_world_)
        return;
    base::AutoReset<bool> in_did_clear_window_object(
        &dispatching_did_clear_window_object_in_main_world_, true);
    // We just cleared the document, not the entire window object, but for the
    // embedder that's close enough.
    Client()->DispatchDidClearWindowObjectInMainWorld();
#endif
}

void FrameLoader::DispatchUnloadEvent(void)
{
    FrameNavigationDisabler navigationDisabler(*m_frame);

    // If the frame is unloading, the provisional loader should no longer be
    // protected. It will be detached soon.
    m_protectProvisionalLoader = false;

    if (Document *document = m_frame->GetDocument())
        document->DispatchUnloadEvents();
}

void FrameLoader::FinishedParsing(void)
{
    if (m_stateMachine.CreatingInitialEmptyDocument())
        return;

    if (LocalFrameClient *client = Client())
    {
        ScriptForbiddenScope forbidScripts;
        client->DispatchDidFinishDocumentLoad();
    }

#ifndef BLINKIT_CRAWLER_ONLY
    if (frame_->View()) {
        ProcessFragment(frame_->GetDocument()->Url(), document_loader_->LoadType(),
            kNavigationToDifferentDocument);
    }
#endif

    m_frame->GetDocument()->CheckCompleted();
}

void FrameLoader::Init(void)
{
    ScriptForbiddenScope forbidScripts;

    BkURL emptyURL;
    ResourceRequest initialRequest(emptyURL);
    m_provisionalDocumentLoader = Client()->CreateDocumentLoader(m_frame, initialRequest, SubstituteData(), nullptr);
    m_provisionalDocumentLoader->StartLoading();

    m_frame->GetDocument()->CancelParsing();

    m_stateMachine.AdvanceTo(FrameLoaderStateMachine::kDisplayingInitialEmptyDocument);

    // Suppress finish notifications for initial empty documents, since they don't
    // generate start notifications.
    m_documentLoader->SetSentDidFinishLoad();
#if 0 // BKTODO: Check if necessary
    if (frame_->GetPage()->Paused())
        SetDefersLoading(true);
#endif
}

bool FrameLoader::PrepareForCommit(void)
{
    DocumentLoader *pdl = m_provisionalDocumentLoader.get();

    if (m_documentLoader)
    {
        // BKTODO: Client()->DispatchWillCommitProvisionalLoad();
        DispatchUnloadEvent();
    }

    // The previous calls to dispatchUnloadEvent() and detachChildren() can
    // execute arbitrary script via things like unload events. If the executed
    // script intiates a new load or causes the current frame to be detached, we
    // need to abandon the current load.
    if (pdl != m_provisionalDocumentLoader.get())
        return false;

    // detachFromFrame() will abort XHRs that haven't completed, which can trigger
    // event listeners for 'abort'. These event listeners might call
    // window.stop(), which will in turn detach the provisional document loader.
    // At this point, the provisional document loader should not detach, because
    // then the FrameLoader would not have any attached DocumentLoaders.
    if (m_documentLoader)
    {
        base::AutoReset<bool> inDetachDocumentLoader(&m_protectProvisionalLoader, true);
        DetachDocumentLoader(m_documentLoader, true);
    }
    // 'abort' listeners can also detach the frame.
    if (nullptr == m_frame->Client())
        return false;
    ASSERT(m_provisionalDocumentLoader.get() == pdl);

    // No more events will be dispatched so detach the Document.
    // TODO(yoav): Should we also be nullifying domWindow's document (or
    // domWindow) since the doc is now detached?
    if (Document *document = m_frame->GetDocument())
        document->Shutdown();
    m_documentLoader = std::move(m_provisionalDocumentLoader);
    if (m_documentLoader)
        m_documentLoader->MarkAsCommitted();
    return true;
}

void FrameLoader::SetReferrerForFrameRequest(FrameLoadRequest &frameRequest)
{
    Document *originDocument = frameRequest.OriginDocument();
    if (nullptr == originDocument)
        return;
    ASSERT(false); // BKTODO:
}

bool FrameLoader::ShouldPerformFragmentNavigation(bool isFormSubmission, const String &httpMethod, WebFrameLoadType loadType, const BkURL &url)
{
    if (!DeprecatedEqualIgnoringCase(httpMethod, http_names::kGET))
        return false;
    if (WebFrameLoadType::kReload == loadType)
        return false;
    if (!url.HasRef())
        return false;
    ASSERT(false); // BKTODO:
    return true;
}

void FrameLoader::StartNavigation(const FrameLoadRequest &passedRequest, WebFrameLoadType loadType)
{
    ASSERT(WebFrameLoadType::kStandard == loadType || WebFrameLoadType::kReload == loadType);

    ASSERT(nullptr != m_frame->GetDocument());

    if (m_inStopAllLoaders)
        return;

    FrameLoadRequest request(passedRequest);
    ResourceRequest &resourceRequest = request.GetResourceRequest();
    const BkURL &url = resourceRequest.Url();

    SetReferrerForFrameRequest(request);

    // TODO(dgozman): merge page dismissal check and FrameNavigationDisabler.
    if (!m_frame->IsNavigationAllowed()
        || m_frame->GetDocument()->PageDismissalEventBeingDispatched() != Document::kNoDismissal)
    {
        return;
    }

    bool sameDocumentNavigation = ShouldPerformFragmentNavigation(nullptr != request.Form(), resourceRequest.HttpMethod(), loadType, url);

    // Perform same document navigation.
    if (sameDocumentNavigation)
    {
        ASSERT(false); // BKTODO:
#if 0
        CommitSameDocumentNavigation(
            url, frame_load_type, nullptr, request.ClientRedirect(),
            origin_document,
            request.TriggeringEventInfo() != WebTriggeringEventInfo::kNotFromEvent,
            nullptr /* extra_data */);
#endif
        return;
    }

#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO: DCHECK(Client()->HasWebView());
#endif

    NavigationPolicy policy = Client()->DecidePolicyForNavigation(resourceRequest);

#ifndef BLINKIT_CRAWLER_ONLY
    // 'beforeunload' can be fired above, which can detach this frame from inside
    // the event handler.
    if (!frame_->GetPage())
        return;
#endif

    if (kNavigationPolicyIgnore == policy)
        return;

    if (kNavigationPolicyCurrentTab == policy)
    {
        CommitNavigation(resourceRequest, SubstituteData(), loadType, nullptr);
        return;
    }

    ASSERT(false); // BKTODO:
#if 0
    ASSERT(kNavigationPolicyHandledByClient == policy);
    if (!CancelProvisionalLoaderForNewNavigation(
        true /* cancel_scheduled_navigations */)) {
        return;
    }

    provisional_document_loader_ = CreateDocumentLoader(
        resource_request, SubstituteData(), request.ClientRedirect(),
        base::UnguessableToken::Create(), frame_load_type, navigation_type,
        nullptr /* navigation_params */, nullptr /* extra_data */);

    provisional_document_loader_->AppendRedirect(
        provisional_document_loader_->Url());
    frame_->GetFrameScheduler()->DidStartProvisionalLoad(frame_->IsMainFrame());

    // TODO(ananta):
    // We should get rid of the dependency on the DocumentLoader in consumers of
    // the DidStartProvisionalLoad() notification.
    Client()->DispatchDidStartProvisionalLoad(
        provisional_document_loader_, resource_request,
        std::move(navigation_initiator_handle));
    probe::didStartProvisionalLoad(frame_);
    virtual_time_pauser_.PauseVirtualTime();
#endif
    ASSERT(m_provisionalDocumentLoader);
}

void FrameLoader::StopAllLoaders(void)
{
    if (m_frame->GetDocument()->PageDismissalEventBeingDispatched() != Document::kNoDismissal)
        return;

    // If this method is called from within this method, infinite recursion can
    // occur (3442218). Avoid this.
    if (m_inStopAllLoaders)
        return;

    base::AutoReset<bool> inStopAllLoaders(&m_inStopAllLoaders, true);

    m_frame->GetDocument()->CancelParsing();
    if (m_documentLoader)
        m_documentLoader->StopLoading();
    if (!m_protectProvisionalLoader)
        DetachDocumentLoader(m_provisionalDocumentLoader);
    m_frame->GetNavigationScheduler().Cancel();
    DidFinishNavigation();
}

String FrameLoader::UserAgent(void) const
{
    return Client()->UserAgent();
}

}  // namespace blink
