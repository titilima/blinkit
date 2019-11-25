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
#include "third_party/blink/renderer/core/loader/frame_fetch_context.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"
#include "third_party/blink/renderer/platform/network/http_names.h"

using namespace BlinKit;

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
    assert(!m_frame);
    assert(nullptr == GetResource());
    assert(kSentDidFinishLoad == m_state);
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

void DocumentLoader::CommitNavigation(const AtomicString &mimeType, const BkURL &overridingURL)
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

void DocumentLoader::DidCommitNavigation(WebGlobalObjectReusePolicy reusePolicy)
{
    FrameLoaderStateMachine *stateMachine = GetFrameLoader().StateMachine();
    if (stateMachine->CreatingInitialEmptyDocument())
        return;

    assert(false); // BKTODO:
}

void DocumentLoader::DidInstallNewDocument(Document *document)
{
    document->SetReadyState(Document::kLoading);

    assert(nullptr != document->GetFrame());

    String headerContentLanguage = m_response.HttpHeaderField(http_names::kContentLanguage);
    if (!headerContentLanguage.IsEmpty())
    {
        assert(false); // BKTODO:
#if 0
        size_t comma_index = header_content_language.find(',');
        // kNotFound == -1 == don't truncate
        header_content_language.Truncate(comma_index);
        header_content_language =
            header_content_language.StripWhiteSpace(IsHTMLSpace<UChar>);
        if (!header_content_language.IsEmpty())
            document->SetContentLanguage(AtomicString(header_content_language));
#endif
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
    assert(m_frame);
    return m_frame->Loader();
}

LocalFrameClient& DocumentLoader::GetLocalFrameClient(void) const
{
    assert(m_frame);
    LocalFrameClient *client = m_frame->Client();
    // LocalFrame clears its |m_client| only after detaching all DocumentLoaders
    // (i.e. calls DetachFromFrame() which clears |m_frame|) owned by the
    // LocalFrame's FrameLoader. So, if |m_frame| is non nullptr, |client| is
    // also non nullptr.
    assert(nullptr != client);
    return *client;
}

void DocumentLoader::InstallNewDocument(
    const BkURL &URL,
    WebGlobalObjectReusePolicy reusePolicy,
    const AtomicString &mimeType,
    const AtomicString &encoding,
    const BkURL &overridingURL)
{
    assert(nullptr == m_frame->GetDocument() || !m_frame->GetDocument()->IsActive());

    FrameLoaderStateMachine *stateMachine = GetFrameLoader().StateMachine();
    if (stateMachine->IsDisplayingInitialEmptyDocument())
        stateMachine->AdvanceTo(FrameLoaderStateMachine::kCommittedFirstRealLoad);

    if (WebGlobalObjectReusePolicy::kUseExisting != reusePolicy)
        m_frame->SetDOMWindow(LocalDOMWindow::Create(*m_frame));

    WillCommitNavigation();

    DocumentInit init = DocumentInit::Create().WithDocumentLoader(this).WithURL(URL);
    Document *document = m_frame->DomWindow()->InstallNewDocument(init);
    if (!overridingURL.IsEmpty())
        assert(false); // BKTODO: document->SetBaseURLOverride(overridingURL);
    DidInstallNewDocument(document);

    DidCommitNavigation(reusePolicy);

    m_parser = document->OpenForNavigation(mimeType, encoding);

    // If this is a scriptable parser and there is a resource, register the
    // resource's cache handler with the parser.
    ScriptableDocumentParser *scriptableParser = m_parser->AsScriptableDocumentParser();
    if (nullptr != scriptableParser)
    {
        if (Resource *resource = GetResource())
        {
            assert(false); // BKTODO:
#if 0
            scriptableParser->SetInlineScriptCacheHandler(ToRawResource(resource)->InlineScriptCacheHandler());
#endif
        }
    }

    GetFrameLoader().DispatchDidClearDocumentOfWindowObject();
}

void DocumentLoader::MarkAsCommitted(void)
{
    assert(m_state < kCommitted);
    m_state = kCommitted;
}

bool DocumentLoader::MaybeLoadEmpty(void)
{
    bool shouldLoadEmpty = !m_substituteData.IsValid() && m_currentRequest.Url().IsEmpty();
    if (!shouldLoadEmpty)
        return false;

    if (m_currentRequest.Url().IsEmpty() &&
        !GetFrameLoader().StateMachine()->CreatingInitialEmptyDocument())
    {
        m_currentRequest.SetURL(BkURL::Blank());
    }
    m_response = ResourceResponse(m_currentRequest.Url());
    m_response.SetMimeType("text/html");
    m_response.SetTextEncodingName("utf-8");
    FinishedLoading();
    return true;
}

void DocumentLoader::StartLoading(void)
{
    assert(nullptr == GetResource());
    assert(kNotStarted == m_state);
    m_state = kProvisional;

    if (MaybeLoadEmpty())
        return;

    assert(false); // BKTODO:
}

void DocumentLoader::WillCommitNavigation(void)
{
    // Currently nothing to do.
}

}  // namespace blink
