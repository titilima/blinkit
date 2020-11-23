// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: raw_resource.cpp
// Description: RawResource Class
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "raw_resource.h"

#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"
#include "third_party/blink/renderer/platform/loader/fetch/source_keyed_cached_metadata_handler.h"

namespace blink {

Resource* RawResource::RawResourceFactory::Create(const ResourceRequest &request, const ResourceLoaderOptions &options) const
{
    return new RawResource(request, m_type, options);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RawResource::RawResource(const ResourceRequest &resourceRequest, ResourceType type, const ResourceLoaderOptions &options)
    : Resource(resourceRequest, type, options)
{
}

void RawResource::DidAddClient(ResourceClient *c)
{
    // ASSERT()/RevalidationStartForbiddenScope are for
    // https://crbug.com/640960#c24.
    ASSERT(!IsCacheValidator());
    if (!HasClient(c))
        return;
    ASSERT(c->IsRawResourceClient());
    RevalidationStartForbiddenScope revalidationStartForbiddenScope(this);
    RawResourceClient *client = static_cast<RawResourceClient *>(c);
    for (const auto &redirect : RedirectChain())
    {
        ResourceRequest request(redirect.m_request);
        ASSERT(false); // BKTODO:
#if 0
        client->RedirectReceived(this, request, redirect.redirect_response_);
#endif
        if (!HasClient(c))
            return;
    }

    if (!GetResponse().IsNull())
    {
        ASSERT(false); // BKTODO:
#if 0
        client->ResponseReceived(this, GetResponse(),
            std::move(data_consumer_handle_));
#endif
    }
    if (!HasClient(c))
        return;
    Resource::DidAddClient(c);
}

RawResource* RawResource::FetchMainResource(
    FetchParameters &params,
    ResourceFetcher *fetcher,
    RawResourceClient *client,
    const SubstituteData &substituteData)
{
    RawResourceFactory factory(ResourceType::kMainResource);
    Resource *resource = fetcher->RequestResource(params, factory, client, substituteData);
    return ToRawResource(resource);
}

SourceKeyedCachedMetadataHandler* RawResource::InlineScriptCacheHandler(void)
{
    ASSERT(ResourceType::kMainResource == GetType());
    return static_cast<SourceKeyedCachedMetadataHandler *>(Resource::CacheHandler());
}

void RawResource::ResponseReceived(const ResourceResponse &response)
{
    Resource::ResponseReceived(response);

    std::vector<RawResourceClient *> clients;
    for (ResourceClient *c : Clients())
        clients.push_back(static_cast<RawResourceClient *>(c));
    for (RawResourceClient *c : clients)
        c->ResponseReceived(this, GetResponse());
}

}  // namespace blink
