// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_fetch_context.cpp
// Description: FrameFetchContext Class
//      Author: Ziming Li
//     Created: 2019-10-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
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

#include "frame_fetch_context.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"
#include "third_party/blink/renderer/core/script/fetch_client_settings_object_impl.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"

namespace blink {

struct FrameFetchContext::FrozenState final : GarbageCollectedFinalized<FrozenState>
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(DocumentLoader *loader, Document *document)
{
    if (nullptr != document)
        return document->GetTaskRunner(TaskType::kNetworking);
    return loader->GetFrame()->GetTaskRunner(TaskType::kNetworking);
}

FrameFetchContext::FrameFetchContext(DocumentLoader *loader, Document *document)
    : BaseFetchContext(GetTaskRunner(loader, document))
    , m_documentLoader(loader), m_document(document)
{
    if (m_document)
        m_fetchClientSettingsObject = std::make_unique<FetchClientSettingsObjectImpl>(*m_document);
    ASSERT(nullptr != GetFrame());
}

FrameFetchContext::~FrameFetchContext(void) = default;

std::shared_ptr<ResourceFetcher> FrameFetchContext::CreateFetcher(DocumentLoader *loader, Document *document)
{
    std::unique_ptr<FetchContext> context(new FrameFetchContext(loader, document));
    return ResourceFetcher::Create(context);
}

FetchContext* FrameFetchContext::Detach(void)
{
    if (IsDetached())
        return this;

    BKLOG("// BKTODO: Initialize FrozenState stuff.");
#if 0 // BKTODO:
    if (m_document)
    {
        frozen_state_ = new FrozenState(
            Url(), GetParentSecurityOrigin(), GetAddressSpace(),
            GetContentSecurityPolicy(), GetSiteForCookies(),
            GetClientHintsPreferences(), GetDevicePixelRatio(), GetUserAgent(),
            IsMainFrame(), IsSVGImageChromeClient());
        fetch_client_settings_object_ =
            document_->CreateFetchClientSettingsObjectSnapshot();
    }
    else
    {
        // Some getters are unavailable in this case.
        frozen_state_ = new FrozenState(
            NullURL(), GetParentSecurityOrigin(), GetAddressSpace(),
            GetContentSecurityPolicy(), GetSiteForCookies(),
            GetClientHintsPreferences(), GetDevicePixelRatio(), GetUserAgent(),
            IsMainFrame(), IsSVGImageChromeClient());
        fetch_client_settings_object_ = new FetchClientSettingsObjectSnapshot(
            NullURL(), nullptr, kReferrerPolicyDefault, String());
    }
#endif

    // This is needed to break a reference cycle in which off-heap
    // ComputedStyle is involved. See https://crbug.com/383860 for details.
    m_document = nullptr;

    return this;
}

void FrameFetchContext::DidLoadResource(Resource *resource)
{
    if (!m_document)
        return;

    if (resource->IsLoadEventBlockingResourceType())
        m_document->CheckCompleted();
}

void FrameFetchContext::DispatchDidReceiveResponse(
    unsigned long identifier,
    const ResourceResponse &response,
    Resource *resource)
{
#if 0 // BKTODO: Check if necessary.
    if (IsDetached())
        return;

    DCHECK(resource);

    MaybeRecordCTPolicyComplianceUseCounter(GetFrame(), resource->GetType(),
        response.GetCTPolicyCompliance(),
        MasterDocumentLoader());

    MixedContentChecker::CheckMixedPrivatePublic(GetFrame(),
        response.RemoteIPAddress());
    LinkLoader::CanLoadResources resource_loading_policy =
        response_type == ResourceResponseType::kFromMemoryCache
        ? LinkLoader::kDoNotLoadResources
        : LinkLoader::kLoadResourcesAndPreconnect;
    if (document_loader_ &&
        document_loader_ == document_loader_->GetFrame()
        ->Loader()
        .GetProvisionalDocumentLoader()) {
        // When response is received with a provisional docloader, the resource
        // haven't committed yet, and we cannot load resources, only preconnect.
        resource_loading_policy = LinkLoader::kDoNotLoadResources;
    }
    // Client hints preferences should be persisted only from responses that were
    // served by the same host as the host of the document-level origin.
    KURL frame_url = Url();
    if (frame_url == NullURL())
        frame_url = document_loader_->Url();

    LinkLoader::LoadLinksFromHeader(
        response.HttpHeaderField(HTTPNames::Link), response.Url(), *GetFrame(),
        document_, NetworkHintsInterfaceImpl(), resource_loading_policy,
        LinkLoader::kLoadAll, nullptr);

    GetLocalFrameClient()->DispatchDidReceiveResponse(response);
#endif
}

LocalFrame* FrameFetchContext::GetFrame(void) const
{
    ASSERT(!IsDetached());

    if (!m_documentLoader)
    {
        ASSERT(false); // BKTODO:
#if 0
        return FrameOfImportsController();
#endif
    }

    LocalFrame *frame = m_documentLoader->GetFrame();
    ASSERT(nullptr != frame);
    return frame;
}

LocalFrameClient* FrameFetchContext::GetLocalFrameClient(void) const
{
    return GetFrame()->Client();;
}

String FrameFetchContext::GetUserAgent(void) const
{
    if (IsDetached())
        ASSERT(false); // BKTODO: return frozen_state_->user_agent;
    return GetFrame()->Loader().UserAgent();
}

void FrameFetchContext::PrepareRequest(ResourceRequest &request, RedirectType redirectType)
{
    SetFirstPartyCookie(request);

    String userAgent = GetUserAgent();
    request.SetHTTPUserAgent(AtomicString(userAgent));

    if (IsDetached())
        return;
    GetLocalFrameClient()->DispatchWillSendRequest(request);
}

void FrameFetchContext::ProvideDocumentToContext(FetchContext &context, Document *document)
{
    ASSERT(document);
    ASSERT(context.IsFrameFetchContext());
    static_cast<FrameFetchContext &>(context).m_document = document;
    static_cast<FrameFetchContext &>(context).m_fetchClientSettingsObject = std::make_unique<FetchClientSettingsObjectImpl>(*document);
}

void FrameFetchContext::RecordDataUriWithOctothorpe(void)
{
    ASSERT(false); // BKTODO:
}

void FrameFetchContext::SetFirstPartyCookie(ResourceRequest &request)
{
#ifndef BLINKIT_CRAWLER_ONLY
    if (GetLocalFrameClient()->IsCrawler())
        return;
#endif
    // Set the first party for cookies url if it has not been set yet (new
    // requests). This value will be updated during redirects, consistent with
    // https://tools.ietf.org/html/draft-ietf-httpbis-cookie-same-site-00#section-2.1.1?
    if (request.SiteForCookies().is_empty())
        request.SetSiteForCookies(request.Url());
}

bool FrameFetchContext::ShouldLoadNewResource(ResourceType type) const
{
    if (!m_documentLoader)
        return true;

    if (IsDetached())
        return false;

    FrameLoader &loader = m_documentLoader->GetFrame()->Loader();
    if (type == ResourceType::kMainResource)
        return loader.GetProvisionalDocumentLoader() == m_documentLoader;
    return loader.GetDocumentLoader() == m_documentLoader;
}

}  // namespace blink
