// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_loader.cpp
// Description: ResourceLoader Class
//      Author: Ziming Li
//     Created: 2019-12-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2010, 2011 Apple Inc. All rights reserved.
 *           (C) 2007 Graham Dennis (graham.dennis@gmail.com)
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

#include "resource_loader.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/public/platform/web_url_loader.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_context.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader_options.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_response.h"

namespace blink {

ResourceLoader::ResourceLoader(ResourceFetcher *fetcher, Resource *resource, uint32_t inflightKeepaliveBytes)
    : m_fetcher(fetcher), m_resource(resource)
{
    ASSERT(m_fetcher);
    ASSERT(m_resource);
}

ResourceLoader::~ResourceLoader(void) = default;

void ResourceLoader::ActivateCacheAwareLoadingIfNeeded(const ResourceRequest & request)
{
    ASSERT(!m_isCacheAwareLoadingActivated);

    if (m_resource->Options().cache_aware_loading_enabled != kIsCacheAwareLoadingEnabled)
        return;

    // Synchronous requests are not supported.
    if (m_resource->Options().synchronous_policy == kRequestSynchronously)
        return;

    // Don't activate on Resource revalidation.
    if (m_resource->IsCacheValidator())
        return;

    m_isCacheAwareLoadingActivated = true;
}

void ResourceLoader::Cancel(void)
{
    ASSERT(false); // BKTODO:
}

FetchContext& ResourceLoader::Context(void) const
{
    return m_fetcher->Context();
}

ResourceLoader* ResourceLoader::Create(ResourceFetcher *fetcher, Resource *resource, uint32_t inflightKeepaliveBytes)
{
    std::unique_ptr<ResourceLoader> loader = base::WrapUnique(new ResourceLoader(fetcher, resource, inflightKeepaliveBytes));
    resource->SetLoader(loader);
    return resource->Loader();
}

void ResourceLoader::DidFail(const ResourceError &error)
{
    HandleError(error);
}

void ResourceLoader::DidFinishLoading(void)
{
#if 0 // BKTODO:
    resource_->SetEncodedDataLength(encoded_data_length);
    resource_->SetEncodedBodyLength(encoded_body_length);
    resource_->SetDecodedBodyLength(decoded_body_length);
#endif

    ASSERT(!m_isDownloadingToBlob); // BKTODO:
#if 0
    if (is_downloading_to_blob_ && !blob_finished_ && blob_response_started_) {
        load_did_finish_before_blob_ =
            DeferedFinishLoadingInfo{ finish_time, should_report_corb_blocking };
        return;
    }
#endif

#if 0 // BKTODO:
    Release(ResourceLoadScheduler::ReleaseOption::kReleaseAndSchedule,
        ResourceLoadScheduler::TrafficReportHints(encoded_data_length,
            decoded_body_length));
#endif
    m_loader.reset();
#if 0 // BKTODO:
    code_cache_request_.reset();
#endif

    m_fetcher->HandleLoaderFinish(m_resource.Get(), ResourceFetcher::kDidFinishLoading);
#if 0 // BKTODO:
    m_fetcher->HandleLoaderFinish(
        resource_.Get(), finish_time, ResourceFetcher::kDidFinishLoading,
        inflight_keepalive_bytes_, should_report_corb_blocking,
        cors_preflight_timing_info);
#endif
}

void ResourceLoader::DidReceiveData(const char *data, int length)
{
    ASSERT(length >= 0);

    Context().DispatchDidReceiveData(m_resource->Identifier(), data, length);
    m_resource->AppendData(data, length);
}

void ResourceLoader::DidReceiveResponse(const ResourceResponse &response)
{
    if (Context().IsDetached())
    {
        ASSERT(false); // BKTODO:
#if 0
        // If the fetch context is already detached, we don't need further signals,
        // so let's cancel the request.
        HandleError(ResourceError::CancelledError(web_url_response.Url()));
#endif
        return;
    }

    ResourceType resourceType = m_resource->GetType();

    const ResourceRequest &initialRequest = m_resource->GetResourceRequest();
    const ResourceLoaderOptions &options = m_resource->Options();

    Context().DispatchDidReceiveResponse(m_resource->Identifier(), response, m_resource);

    m_resource->ResponseReceived(response);

    ASSERT(!ShouldFetchCodeCache()); // BKTODO:
#if 0
    // Send the cached code after we notify that the response is received.
    // Resource expects that we receive the response first before the
    // corresponding cached code.
    if (code_cache_request_)
        code_cache_request_->DidReceiveResponse(response.ResponseTime(), this);
#endif

    if (nullptr == m_resource->Loader())
        return;

    ASSERT(response.HttpStatusCode() < 400); // BKTODO:
#if 0
    if (response_to_pass.HttpStatusCode() >= 400 &&
        !resource_->ShouldIgnoreHTTPStatusCodeErrors())
        HandleError(ResourceError::CancelledError(response_to_pass.Url()));
#endif
}

void ResourceLoader::HandleError(const ResourceError &error)
{
    m_loader.reset();
    ASSERT(!ShouldFetchCodeCache()); // BKTODO:
#if 0
    code_cache_request_.reset();
#endif

    m_fetcher->HandleLoaderError(m_resource.Get(), error);
}

void ResourceLoader::ScheduleCancel(void)
{
    ASSERT(false); // BKTODO:
}

void ResourceLoader::SetDefersLoading(bool defers)
{
    ASSERT(m_loader);
    ASSERT(!defers); // BKTODO:
}

bool ResourceLoader::ShouldFetchCodeCache(void)
{
    if (!RuntimeEnabledFeatures::IsolatedCodeCacheEnabled())
        return false;

    ASSERT(false); // BKTODO:
    return false;
}

void ResourceLoader::Start(void)
{
    const ResourceRequest &request = m_resource->GetResourceRequest();
    ActivateCacheAwareLoadingIfNeeded(request);
    m_loader = Platform::Current()->CreateURLLoader(Context().GetLoadingTaskRunner());
    StartWith(request);
}

void ResourceLoader::StartWith(const ResourceRequest &request)
{
    ASSERT(m_loader);

    if (m_resource->Options().synchronous_policy == kRequestSynchronously && Context().DefersLoading())
    {
        Cancel();
        return;
    }

    m_isDownloadingToBlob = request.DownloadToBlob();

    SetDefersLoading(Context().DefersLoading());

    if (ShouldFetchCodeCache())
    {
        ASSERT(false); // BKTODO:
#if 0
        code_cache_request_ = std::make_unique<CodeCacheRequest>(
            Context().CreateCodeCacheLoader(), request.Url(),
            Context().DefersLoading());
#endif
    }

    if (m_isCacheAwareLoadingActivated)
    {
        ASSERT(false); // BKTODO:
#if 0
        // Override cache policy for cache-aware loading. If this request fails, a
        // reload with original request will be triggered in DidFail().
        ResourceRequest cache_aware_request(request);
        cache_aware_request.SetCacheMode(
            mojom::FetchCacheMode::kUnspecifiedOnlyIfCachedStrict);
        loader_->LoadAsynchronously(WrappedResourceRequest(cache_aware_request),
            this);
        if (code_cache_request_) {
            // Sets defers loading and initiates a fetch from code cache.
            code_cache_request_->FetchFromCodeCache(loader_.get(), this);
        }
#endif
        return;
    }

    if (m_resource->Options().synchronous_policy == kRequestSynchronously)
    {
        ASSERT(false); // BKTODO: RequestSynchronously(request);        
    }
    else
    {
        m_loader->LoadAsynchronously(request, this);

        ASSERT(!ShouldFetchCodeCache()); // BKTODO:
#if 0
        if (code_cache_request_) {
            // Sets defers loading and initiates a fetch from code cache.
            code_cache_request_->FetchFromCodeCache(loader_.get(), this);
        }
#endif
    }
}

} // namespace blink
