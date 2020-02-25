// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_loader.h
// Description: ResourceLoader Class
//      Author: Ziming Li
//     Created: 2019-12-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2005, 2006, 2011 Apple Inc. All rights reserved.
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

#ifndef BLINKIT_BLINK_RESOURCE_LOADER_H
#define BLINKIT_BLINK_RESOURCE_LOADER_H

#pragma once

#include "third_party/blink/public/platform/web_url_loader_client.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class FetchContext;
class Resource;
class ResourceFetcher;
class ResourceRequest;
class WebURLLoader;

class ResourceLoader final : public GarbageCollectedFinalized<ResourceLoader>
                           , public WebURLLoaderClient
{
public:
    static ResourceLoader* Create(ResourceFetcher *fetcher, std::shared_ptr<Resource> &resource, uint32_t inflightKeepaliveBytes = 0);
    ~ResourceLoader(void) override;

    void Start(void);
    void Cancel(void);
    void ScheduleCancel(void);
    void HandleError(const ResourceError &error);

    void SetDefersLoading(bool defers);

    // Called before start() to activate cache-aware loading if enabled in
    // |m_resource->options()| and applicable.
    void ActivateCacheAwareLoadingIfNeeded(const ResourceRequest &request);
private:
    ResourceLoader(ResourceFetcher *fetcher, std::shared_ptr<Resource> &resource, uint32_t inflightKeepaliveBytes);

    FetchContext& Context(void) const;
    bool ShouldFetchCodeCache(void);
    void StartWith(const ResourceRequest &request);

    // WebURLLoaderClient
    void DidReceiveResponse(const ResourceResponse &response) override;
    void DidReceiveData(const char *data, int length) override;
    void DidFinishLoading(void) override;
    void DidFail(const ResourceError &error) override;

    std::unique_ptr<WebURLLoader> m_loader;
    Member<ResourceFetcher> m_fetcher;
    std::shared_ptr<Resource> m_resource;

    bool m_isCacheAwareLoadingActivated = false;
    bool m_isDownloadingToBlob = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_RESOURCE_LOADER_H
