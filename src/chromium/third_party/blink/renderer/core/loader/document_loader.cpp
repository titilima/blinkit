// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_loader.cpp
// Description: DocumentLoader Class
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#include "document_loader.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/document_init.h"
#include "third_party/blink/renderer/core/dom/document_parser.h"
#include "third_party/blink/renderer/core/dom/scriptable_document_parser.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/core/loader/frame_fetch_context.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_parameters.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader_options.h"
#include "third_party/blink/renderer/platform/network/http_names.h"
#include "third_party/blink/renderer/platform/network/mime/mime_type_registry.h"
#include "third_party/blink/renderer/platform/weborigin/kurl.h"

namespace blink {

DocumentLoader::DocumentLoader(LocalFrame *frame, const ResourceRequest &request, const SubstituteData &substituteData)
    : m_frame(frame)
    , m_fetcher(FrameFetchContext::CreateFetcherFromDocumentLoader(this))
    , m_originalRequest(request), m_currentRequest(request)
    , m_substituteData(substituteData)
{
}

DocumentLoader::~DocumentLoader(void)
{
    ASSERT(!m_frame);
    ASSERT(nullptr == GetResource());
    ASSERT(kSentDidFinishLoad == m_state);
}

void DocumentLoader::CommitData(const char *bytes, size_t length)
{
    CommitNavigation(m_response.MimeType());
    ASSERT(m_state >= kCommitted);

    // This can happen if document.close() is called by an event handler while
    // there's still pending incoming data.
    if (!m_frame || !m_frame->GetDocument()->Parsing())
        return;

    if (0 != length)
        m_dataReceived = true;

    m_parser->AppendBytes(bytes, length);
}

void DocumentLoader::CommitNavigation(const AtomicString &mimeType, const GURL &overridingURL)
{
    if (kProvisional != m_state)
        return;

    GetFrameLoader().CommitProvisionalLoad();
    if (!m_frame)
        return;

    WebGlobalObjectReusePolicy reusePolicy = m_frame->ShouldReuseDefaultView()
        ? WebGlobalObjectReusePolicy::kUseExisting
        : WebGlobalObjectReusePolicy::kCreateNew;
    const AtomicString &encoding = GetResponse().TextEncodingName();
    InstallNewDocument(Url(), reusePolicy, mimeType, encoding, overridingURL);
    m_parser->SetDocumentWasLoadedAsPartOfNavigation();
    if (m_currentRequest.WasDiscarded())
        m_frame->GetDocument()->SetWasDiscarded(true);

    m_frame->GetDocument()->MaybeHandleHttpRefresh(m_response.HttpHeaderField(http_names::kRefresh),
        Document::kHttpRefreshFromHeader);
}

void DocumentLoader::DataReceived(Resource *resource, const char *data, size_t length)
{
    ASSERT(nullptr != data);
    ASSERT(length > 0);
    ASSERT(GetResource() == resource);
    ASSERT(!m_response.IsNull());
#if 0 // BKTODO: Check if necessary
    DCHECK(!frame_->GetPage()->Paused());
#endif

    if (m_inDataReceived)
    {
        ASSERT(false); // BKTODO:
#if 0
        // If this function is reentered, defer processing of the additional data to
        // the top-level invocation. Reentrant calls can occur because of web
        // platform (mis-)features that require running a nested run loop:
        // - alert(), confirm(), prompt()
        // - Detach of plugin elements.
        // - Synchronous XMLHTTPRequest
        data_buffer_->Append(data, length);
#endif
        return;
    }

    base::AutoReset<bool> reentrancyProtector(&m_inDataReceived, true);
    ProcessData(data, length);
    ProcessDataBuffer();
}

void DocumentLoader::DetachFromFrame(bool flushMicrotaskQueue)
{
    ASSERT(m_frame);
    StopLoading();
    m_fetcher->ClearContext();
    if (flushMicrotaskQueue)
    {
        // Flush microtask queue so that they all run on pre-navigation context.
        // TODO(dcheng): This is a temporary hack that should be removed. This is
        // only here because it's currently not possible to drop the microtasks
        // queued for a Document when the Document is navigated away; instead, the
        // entire microtask queue needs to be flushed. Unfortunately, running the
        // microtasks any later results in violating internal invariants, since
        // Blink does not expect the DocumentLoader for a not-yet-detached Document
        // to be null. It is also not possible to flush microtasks any earlier,
        // since flushing microtasks can only be done after any other JS (which can
        // queue additional microtasks) has run. Once it is possible to associate
        // microtasks with a v8::Context, remove this hack.
        // BKTODO: Microtask::PerformCheckpoint(V8PerIsolateData::MainThreadIsolate());
    }
    ScriptForbiddenScope forbidScripts;

    // If that load cancellation triggered another detach, leave.
    // (fast/frames/detach-frame-nested-no-crash.html is an example of this.)
    if (!m_frame)
        return;

#if 0 // BKTODO: Check the code below.
    WeakIdentifierMap<DocumentLoader>::NotifyObjectDestroyed(this);
#endif
    ClearResource();
    m_frame = nullptr;
}

void DocumentLoader::DidCommitNavigation(WebGlobalObjectReusePolicy reusePolicy)
{
    FrameLoaderStateMachine *stateMachine = GetFrameLoader().StateMachine();
    if (stateMachine->CreatingInitialEmptyDocument())
        return;

    if (!stateMachine->CommittedMultipleRealLoads() && WebFrameLoadType::kStandard == m_loadType)
        stateMachine->AdvanceTo(FrameLoaderStateMachine::kCommittedMultipleRealLoads);

#if 0 // BKTODO: Check if necessary
    m_frame->ResumeSubresourceLoading();
    GetLocalFrameClient().DispatchDidCommitLoad(history_item_.Get(), commit_type,
        global_object_reuse_policy);

    // Links with media values need more information (like viewport information).
    // This happens after the first chunk is parsed in HTMLDocumentParser.
    DispatchLinkHeaderPreloads(nullptr, LinkLoader::kOnlyLoadNonMedia);
#endif

#ifndef BLINKIT_CRAWLER_ONLY
    if (Page *page = m_frame->GetPage())
        ASSERT(false); // BKTODO: m_frame->GetPage()->DidCommitLoad(frame_);
#endif
}

void DocumentLoader::DidInstallNewDocument(Document *document)
{
    document->SetReadyState(Document::kLoading);

    assert(nullptr != document->GetFrame());

    String headerContentLanguage = m_response.HttpHeaderField(http_names::kContentLanguage);
    if (!headerContentLanguage.IsEmpty())
    {
        size_t commaIndex = headerContentLanguage.find(',');
        // kNotFound == -1 == don't truncate
        headerContentLanguage.Truncate(commaIndex);
        headerContentLanguage = headerContentLanguage.StripWhiteSpace(IsHTMLSpace<UChar>);
        if (!headerContentLanguage.IsEmpty())
            document->SetContentLanguage(AtomicString(headerContentLanguage));
    }

    GetLocalFrameClient().DidCreateNewDocument();
}

void DocumentLoader::FinishedLoading(void)
{
    // If this is an empty document, it will not have actually been created yet.
    // Force a commit so that the Document actually gets created.
    if (kProvisional == m_state)
        CommitData(nullptr, 0);

    if (!m_frame)
        return;

    if (m_parser)
    {
        m_parser->Finish();
        m_parser.reset();
    }
    ClearResource();
}

FrameLoader& DocumentLoader::GetFrameLoader(void) const
{
    ASSERT(m_frame);
    return m_frame->Loader();
}

LocalFrameClient& DocumentLoader::GetLocalFrameClient(void) const
{
    ASSERT(m_frame);
    LocalFrameClient *client = m_frame->Client();
    // LocalFrame clears its |m_client| only after detaching all DocumentLoaders
    // (i.e. calls DetachFromFrame() which clears |m_frame|) owned by the
    // LocalFrame's FrameLoader. So, if |m_frame| is non nullptr, |client| is
    // also non nullptr.
    ASSERT(nullptr != client);
    return *client;
}

void DocumentLoader::InstallNewDocument(
    const GURL &URL,
    WebGlobalObjectReusePolicy reusePolicy,
    const AtomicString &mimeType,
    const AtomicString &encoding,
    const GURL &overridingURL)
{
    ASSERT(nullptr == m_frame->GetDocument() || !m_frame->GetDocument()->IsActive());

    FrameLoaderStateMachine *stateMachine = GetFrameLoader().StateMachine();
    if (stateMachine->IsDisplayingInitialEmptyDocument())
        stateMachine->AdvanceTo(FrameLoaderStateMachine::kCommittedFirstRealLoad);

    if (WebGlobalObjectReusePolicy::kUseExisting != reusePolicy)
    {
        std::unique_ptr<LocalDOMWindow> window = LocalDOMWindow::Create(*m_frame);
        m_frame->SetDOMWindow(window);
    }

    WillCommitNavigation();

    DocumentInit init = DocumentInit::Create().WithDocumentLoader(this).WithURL(URL);
    Document *document = m_frame->DomWindow()->InstallNewDocument(init);
    if (!overridingURL.is_empty())
        ASSERT(false); // BKTODO: document->SetBaseURLOverride(overridingURL);
    DidInstallNewDocument(document);

    DidCommitNavigation(reusePolicy);

    m_parser = document->OpenForNavigation(mimeType, encoding);

    // If this is a scriptable parser and there is a resource, register the
    // resource's cache handler with the parser.
    ScriptableDocumentParser *scriptableParser = m_parser->AsScriptableDocumentParser();
    if (nullptr != scriptableParser)
    {
        if (Resource *resource = GetResource())
            scriptableParser->SetInlineScriptCacheHandler(ToRawResource(resource)->InlineScriptCacheHandler());
    }

    GetFrameLoader().DispatchDidClearDocumentOfWindowObject();
}

void DocumentLoader::LoadFailed(const ResourceError &error)
{
    // BKTODO: m_fetcher->ClearResourcesFromPreviousFetcher();

    switch (m_state)
    {
        case kNotStarted:
            [[fallthrough]];
        case kProvisional:
            m_state = kSentDidFinishLoad;
            GetLocalFrameClient().DispatchDidFailProvisionalLoad(error);
            if (m_frame)
                GetFrameLoader().DetachProvisionalDocumentLoader(this);
            break;
        case kCommitted:
            if (DocumentParser *documentParser = m_frame->GetDocument()->Parser())
                documentParser->StopParsing();
            m_state = kSentDidFinishLoad;
            GetLocalFrameClient().DispatchDidFailLoad(error);
            GetFrameLoader().DidFinishNavigation();
            break;
        case kSentDidFinishLoad:
            NOTREACHED();
            break;
    }
    ASSERT(kSentDidFinishLoad == m_state);
}

void DocumentLoader::MarkAsCommitted(void)
{
    ASSERT(m_state < kCommitted);
    m_state = kCommitted;
}

bool DocumentLoader::MaybeLoadEmpty(void)
{
    bool shouldLoadEmpty = !m_substituteData.IsValid() && m_currentRequest.Url().is_empty();
    if (!shouldLoadEmpty)
        return false;

    if (m_currentRequest.Url().is_empty() &&
        !GetFrameLoader().StateMachine()->CreatingInitialEmptyDocument())
    {
        m_currentRequest.SetURL(BlankURL());
    }
    m_response = ResourceResponse(m_currentRequest.Url());
    m_response.SetMimeType("text/html");
    m_response.SetTextEncodingName("utf-8");
    FinishedLoading();
    return true;
}

void DocumentLoader::NotifyFinished(Resource *resource)
{
    ASSERT(GetResource() == resource);
    ASSERT(GetResource() != nullptr);

    if (!resource->ErrorOccurred() && !resource->WasCanceled())
    {
#if 0 // BKTODO:
        FinishedLoading(resource->LoadFinishTime());
#else
        FinishedLoading();
#endif
        return;
    }

    LoadFailed(resource->GetResourceError());
    ClearResource();
}

void DocumentLoader::ProcessData(const char *data, size_t length)
{
    CommitData(data, length);
}

void DocumentLoader::ProcessDataBuffer(void)
{
    // BKTODO: May be useless.
}

void DocumentLoader::ResponseReceived(Resource *resource, const ResourceResponse &response)
{
    ASSERT(GetResource() == resource);
    ASSERT(m_frame);

    m_response = response;

    if (!ShouldContinueForResponse())
        m_fetcher->StopFetching();
}

bool DocumentLoader::ShouldContinueForResponse(void) const
{
    if (m_substituteData.IsValid())
        return true;

    int statusCode = m_response.HttpStatusCode();
    if (204 == statusCode || 205 == statusCode)
    {
        // The server does not want us to replace the page contents.
        return false;
    }

    ASSERT(m_response.HttpHeaderField(http_names::kContentDisposition).IsEmpty()); // BKTODO:
#if 0
    if (IsContentDispositionAttachment(m_response.HttpHeaderField(http_names::kContentDisposition)))
    {
        // The server wants us to download instead of replacing the page contents.
        // Downloading is handled by the embedder, but we still get the initial
        // response so that we can ignore it and clean up properly.
        return false;
    }
#endif
    return MIMETypeRegistry::IsSupportedMIMEType(m_response.MimeType());
}

void DocumentLoader::StartLoading(void)
{
    ASSERT(nullptr == GetResource());
    ASSERT(kNotStarted == m_state);
    m_state = kProvisional;

    if (MaybeLoadEmpty())
        return;

    ResourceLoaderOptions options;
    // BKTODO: options.data_buffering_policy = kDoNotBufferData;
    // BKTODO: options.initiator_info.name = FetchInitiatorTypeNames::document;
    FetchParameters fetchParams(m_currentRequest, options);
    std::shared_ptr<ResourceFetcher> fetcher = Fetcher();
    RawResource::FetchMainResource(fetchParams, fetcher.get(), this, m_substituteData);
    // A bunch of headers are set when the underlying resource load begins, and
    // request_ needs to include those. Even when using a cached resource, we may
    // make some modification to the request, e.g. adding the referer header.
    Resource *resource = GetResource();
    m_currentRequest = resource->IsLoading()
        ? resource->GetResourceRequest()
        : fetchParams.GetResourceRequest();
}

void DocumentLoader::StopLoading(void)
{
    m_fetcher->StopFetching();
    if (m_frame && !SentDidFinishLoad())
        LoadFailed(ResourceError(BK_ERR_CANCELLED, Url()));
}

void DocumentLoader::WillCommitNavigation(void)
{
    // Currently nothing to do.
}

}  // namespace blink
