// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_fetcher.cpp
// Description: ResourceFetcher Class
//      Author: Ziming Li
//     Created: 2019-10-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller (mueller@kde.org)
    Copyright (C) 2002 Waldo Bastian (bastian@kde.org)
    Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
    rights reserved.
    Copyright (C) 2009 Torch Mobile Inc. http://www.torchmobile.com/

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

    This class provides all functionality needed for loading images, style
    sheets and html pages from the web. It has a memory cache for these objects.
*/

#include "resource_fetcher.h"

#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_context.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_client.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader.h"
#include "third_party/blink/renderer/platform/loader/fetch/unique_identifier.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace BlinKit;

namespace blink {

ResourceFetcher::ResourceFetcher(std::unique_ptr<FetchContext> &context)
    : m_context(std::move(context))
    , m_imageFetched(false)
{
}

ResourceFetcher::~ResourceFetcher(void) = default;

int ResourceFetcher::BlockingRequestCount(void) const
{
    return m_loaders.size();
}

void ResourceFetcher::ClearContext(void)
{
    ASSERT(m_resourcesFromPreviousFetcher.empty());
    // BKTODO: m_scheduler->Shutdown();
    // BKTODO: ClearPreloads(ResourceFetcher::kClearAllPreloads);
    FetchContext *detachedContext = Context().Detach();
    if (detachedContext != m_context.get())
        m_context.reset(detachedContext);

    // Make sure the only requests still going are keepalive requests.
    // Callers of ClearContext() should be calling StopFetching() prior
    // to this, but it's possible for additional requests to start during
    // StopFetching() (e.g., fallback fonts that only trigger when the
    // first choice font failed to load).
    StopFetching();

    if (!m_loaders.empty() || !m_nonBlockingLoaders.empty())
    {
        ASSERT(false); // BKTODO:
#if 0
        // There are some keepalive requests.
        // The use of WrapPersistent creates a reference cycle intentionally,
        // to keep the ResourceFetcher and ResourceLoaders alive until the requests
        // complete or the timer fires.
        keepalive_loaders_task_handle_ = PostDelayedCancellableTask(
            *Context().GetLoadingTaskRunner(), FROM_HERE,
            WTF::Bind(&ResourceFetcher::StopFetchingIncludingKeepaliveLoaders,
                WrapPersistent(this)),
            kKeepaliveLoadersTimeout);
#endif
    }
}

static ResourceLoadPriority TypeToPriority(ResourceType type)
{
    switch (type)
    {
        case ResourceType::kMainResource:
        case ResourceType::kCSSStyleSheet:
        case ResourceType::kFont:
            // Also parser-blocking scripts (set explicitly in loadPriority)
            return ResourceLoadPriority::kVeryHigh;
        case ResourceType::kRaw:
        case ResourceType::kImportResource:
        case ResourceType::kScript:
            // Also visible resources/images (set explicitly in loadPriority)
            return ResourceLoadPriority::kHigh;
        case ResourceType::kManifest:
            // Also late-body scripts discovered by the preload scanner (set
            // explicitly in loadPriority)
            return ResourceLoadPriority::kMedium;
        case ResourceType::kImage:
        case ResourceType::kTextTrack:
        case ResourceType::kAudio:
        case ResourceType::kVideo:
            // Also async scripts (set explicitly in loadPriority)
            return ResourceLoadPriority::kLow;
        case ResourceType::kLinkPrefetch:
            return ResourceLoadPriority::kVeryLow;
        default:
            break;
    }

    NOTREACHED();
    return ResourceLoadPriority::kUnresolved;
}

ResourceLoadPriority ResourceFetcher::ComputeLoadPriority(
    ResourceType type,
    const ResourceRequest &resourceRequest,
    ResourcePriority::VisibilityStatus visibility,
    FetchParameters::DeferOption deferOption,
    FetchParameters::SpeculativePreloadType speculativePreloadType,
    bool isLinkPreload)
{
    ResourceLoadPriority priority = TypeToPriority(type);

    // Visible resources (images in practice) get a boost to High priority.
    if (ResourcePriority::kVisible == visibility)
        priority = ResourceLoadPriority::kHigh;

    // Resources before the first image are considered "early" in the document and
    // resources after the first image are "late" in the document.  Important to
    // note that this is based on when the preload scanner discovers a resource
    // for the most part so the main parser may not have reached the image element
    // yet.
    if (ResourceType::kImage == type && !isLinkPreload)
        m_imageFetched = true;

    // A preloaded font should not take precedence over critical CSS or
    // parser-blocking scripts.
    if (ResourceType::kFont == type && isLinkPreload)
        priority = ResourceLoadPriority::kHigh;

    if (FetchParameters::kIdleLoad == deferOption)
    {
        priority = ResourceLoadPriority::kVeryLow;
    }
    else if (ResourceType::kScript == type)
    {
        // Special handling for scripts.
        // Default/Parser-Blocking/Preload early in document: High (set in
        // typeToPriority)
        // Async/Defer: Low Priority (applies to both preload and parser-inserted)
        // Preload late in document: Medium
        if (deferOption == FetchParameters::kLazyLoad)
        {
            priority = ResourceLoadPriority::kLow;
        }
        else if (FetchParameters::SpeculativePreloadType::kInDocument == speculativePreloadType && m_imageFetched)
        {
            // Speculative preload is used as a signal for scripts at the bottom of
            // the document.
            priority = ResourceLoadPriority::kMedium;
        }
    }
    else if (FetchParameters::kLazyLoad == deferOption)
    {
        priority = ResourceLoadPriority::kVeryLow;
    }

    // A manually set priority acts as a floor. This is used to ensure that
    // synchronous requests are always given the highest possible priority, as
    // well as to ensure that there isn't priority churn if images move in and out
    // of the viewport, or are displayed more than once, both in and out of the
    // viewport.
    return std::max(priority, resourceRequest.Priority());
}

FetchContext& ResourceFetcher::Context(void) const
{
    if (m_context)
        return *m_context;
    ASSERT(false); // BKTODO:
    return *m_context;
}

std::shared_ptr<Resource> ResourceFetcher::CreateResourceForLoading(
    const FetchParameters &params,
    const ResourceFactory &factory)
{
#if 0 // BKTODO:
    DCHECK(!IsMainThread() || params.IsStaleRevalidation() ||
        !GetMemoryCache()->ResourceForURL(params.GetResourceRequest().Url(),
            cache_identifier));
#endif

    std::shared_ptr<Resource> resource = factory.Create(params.GetResourceRequest(), params.Options(),
        params.DecoderOptions());
    resource->SetLinkPreload(params.IsLinkPreload());

#ifndef BLINKIT_CRAWLER_ONLY
    AddToMemoryCacheIfNeeded(params, resource);
#endif
    return resource;
}

void ResourceFetcher::HandleLoadCompletion(Resource *resource)
{
    Context().DidLoadResource(resource);

#if 0 // BKTODO:
    resource->ReloadIfLoFiOrPlaceholderImage(this, Resource::kReloadIfNeeded);
#endif
}

void ResourceFetcher::HandleLoaderError(Resource *resource, const ResourceError &error)
{
    ASSERT(nullptr != resource);

#if 0 // BKTODO:
    DCHECK_LE(inflight_keepalive_bytes, inflight_keepalive_bytes_);
    inflight_keepalive_bytes_ -= inflight_keepalive_bytes;
#endif

    RemoveResourceLoader(resource->Loader());

    Context().DispatchDidFail(resource->Url(), resource->Identifier(), error);

#if 0 // BKTODO:
    if (error.IsCancellation())
        RemovePreload(resource);
#endif
    resource->FinishAsError(error, Context().GetLoadingTaskRunner().get());

    HandleLoadCompletion(resource);
}

void ResourceFetcher::HandleLoaderFinish(Resource *resource, LoaderFinishType type)
{
    ASSERT(nullptr != resource);

#if 0 // BKTODO:
    DCHECK_LE(inflight_keepalive_bytes, inflight_keepalive_bytes_);
    inflight_keepalive_bytes_ -= inflight_keepalive_bytes;
#endif

    ResourceLoader *loader = resource->Loader();
    if (type == kDidFinishFirstPartInMultipart)
    {
        ASSERT(false); // BKTODO:
#if 0
        // When loading a multipart resource, make the loader non-block when
        // finishing loading the first part.
        MoveResourceLoaderToNonBlocking(loader);
#endif
    }
    else
    {
        RemoveResourceLoader(loader);
        ASSERT(std::end(m_nonBlockingLoaders) == m_nonBlockingLoaders.find(loader));
    }
    ASSERT(std::end(m_loaders) == m_loaders.find(loader));

#if 0 // BKTODO:
    const int64_t encoded_data_length = resource->GetResponse().EncodedDataLength();
#endif

    Context().DispatchDidFinishLoading(resource->Identifier());

    if (kDidFinishLoading == type)
    {
        resource->Finish(Context().GetLoadingTaskRunner().get());
#if 0 // BKTODO:
        resource->Finish(finish_time, Context().GetLoadingTaskRunner().get());
#endif

#if 0 // BKTODO:
        // Since this resource came from the network stack we only schedule a stale
        // while revalidate request if the network asked us to. If we called
        // ShouldRevalidateStaleResponse here then the resource would be checking
        // the freshness based on current time. It is possible that the resource
        // is fresh at the time of the network stack handling but not at the time
        // handling here and we should not be forcing a revalidation in that case.
        // eg. network stack returning a resource with max-age=0.
        if (resource->GetResourceRequest().AllowsStaleResponse() &&
            resource->StaleRevalidationRequested()) {
            ScheduleStaleRevalidate(resource);
        }
#endif
    }

    HandleLoadCompletion(resource);
}

void ResourceFetcher::InsertAsPreloadIfNecessary(Resource *resource, const FetchParameters &params, ResourceType type)
{
    if (!params.IsSpeculativePreload() && !params.IsLinkPreload())
        return;
    ASSERT(false); // BKTODO:
}

static BkURL RemoveFragmentIdentifierIfNeeded(const BkURL &originalUrl)
{
    if (!originalUrl.HasRef() || !originalUrl.SchemeIsHTTPOrHTTPS())
        return originalUrl;
    BkURL url = originalUrl;
    ASSERT(false); // BKTODO: url.RemoveFragmentIdentifier();
    return url;
}

std::optional<ResourceRequestBlockedReason> ResourceFetcher::PrepareRequest(
    FetchParameters &params,
    const ResourceFactory &factory,
    const SubstituteData &substituteData,
    unsigned long identifier)
{
    ResourceRequest &resourceRequest = params.MutableResourceRequest();
    ResourceType resourceType = factory.GetType();
    const ResourceLoaderOptions &options = params.Options();

    ASSERT(options.synchronous_policy == kRequestAsynchronously || ResourceType::kRaw == resourceType);

    params.OverrideContentType(factory.ContentType());

    if (!params.Url().IsValid())
        return ResourceRequestBlockedReason::kOther;

    resourceRequest.SetPriority(ComputeLoadPriority(resourceType, params.GetResourceRequest(),
        ResourcePriority::kNotVisible, params.Defer(), params.GetSpeculativePreloadType(),
        params.IsLinkPreload()));
#if 0 // BKTODO: May be useless
    if (resource_type == ResourceType::kLinkPrefetch)
        resource_request.SetHTTPHeaderField(HTTPNames::Purpose, "prefetch");
#endif

    FetchResourceType fetchType = (ResourceType::kMainResource == resourceType)
        ? kFetchMainResource
        : kFetchSubresource;
    Context().AddAdditionalRequestHeaders(resourceRequest, fetchType);

    BkURL url = RemoveFragmentIdentifierIfNeeded(params.Url());
    std::optional<ResourceRequestBlockedReason> blockedReason = Context().CanRequest(resourceType, resourceRequest,
        url, options, resourceRequest.GetRedirectStatus());
    if (blockedReason)
        return blockedReason;

    // For initial requests, call prepareRequest() here before revalidation
    // policy is determined.
    Context().PrepareRequest(resourceRequest, FetchContext::RedirectType::kNotForRedirect);

    if (!params.Url().IsValid())
        return ResourceRequestBlockedReason::kOther;

    return std::nullopt;
}

void ResourceFetcher::RemoveResourceLoader(ResourceLoader *loader)
{
    ASSERT(nullptr != loader);
    auto it = m_loaders.find(loader);
    if (std::end(m_loaders) != it)
    {
        m_loaders.erase(it);
    }
    else
    {
        it = m_nonBlockingLoaders.find(loader);
        if (std::end(m_nonBlockingLoaders) != it)
            m_nonBlockingLoaders.erase(it);
        else
            NOTREACHED();
    }

#if 0 // BKTODO:
    if (loaders_.IsEmpty() && non_blocking_loaders_.IsEmpty())
        keepalive_loaders_task_handle_.Cancel();
#endif
}

std::shared_ptr<Resource> ResourceFetcher::RequestResource(
    FetchParameters &params,
    const ResourceFactory &factory,
    ResourceClient *client,
    const SubstituteData &substituteData)
{
    unsigned long identifier = CreateUniqueIdentifier();
    ResourceRequest &resourceRequest = params.MutableResourceRequest();

    // TODO(crbug.com/123004): Remove once we have enough stats on data URIs that
    // contain fragments ('#' characters).
    //
    // TODO(crbug.com/796173): This call happens before commit for iframes that
    // have data URI sources, which causes UKM to miss the metric recording.
    if (m_context)
    {
        const BkURL &url = params.Url();
        if (url.HasRef() && url.SchemeIsData())
            m_context->RecordDataUriWithOctothorpe();
    }

    std::optional<ResourceRequestBlockedReason> blockedReason = PrepareRequest(params, factory, substituteData, identifier);
    if (blockedReason)
        return ResourceForBlockedRequest(params, factory, blockedReason.value(), client);

    ResourceType resourceType = factory.GetType();

    std::shared_ptr<Resource> resource;
    RevalidationPolicy policy = kLoad;

#ifdef BLINKIT_CRAWLER_ONLY
    resource = CreateResourceForLoading(params, factory);
#else
    bool is_data_url = resource_request.Url().ProtocolIsData();
    bool is_static_data = is_data_url || substitute_data.IsValid() || archive_;
    bool is_stale_revalidation = params.IsStaleRevalidation();
    if (!is_stale_revalidation && is_static_data) {
        resource = ResourceForStaticData(params, factory, substitute_data);
        if (resource) {
            policy =
                DetermineRevalidationPolicy(resource_type, params, *resource, true);
        }
        else if (!is_data_url && archive_) {
            // Abort the request if the archive doesn't contain the resource, except
            // in the case of data URLs which might have resources such as fonts that
            // need to be decoded only on demand. These data URLs are allowed to be
            // processed using the normal ResourceFetcher machinery.
            return ResourceForBlockedRequest(
                params, factory, ResourceRequestBlockedReason::kOther, client);
        }
    }

    if (!is_stale_revalidation && !resource) {
        resource = MatchPreload(params, resource_type);
        if (resource) {
            policy = kUse;
            // If |params| is for a blocking resource and a preloaded resource is
            // found, we may need to make it block the onload event.
            MakePreloadedResourceBlockOnloadIfNeeded(resource, params);
        }
        else if (IsMainThread()) {
            resource =
                GetMemoryCache()->ResourceForURL(params.Url(), GetCacheIdentifier());
            if (resource) {
                policy = DetermineRevalidationPolicy(resource_type, params, *resource,
                    is_static_data);
            }
        }
    }

    UpdateMemoryCacheStats(resource, policy, params, factory, is_static_data);

    switch (policy) {
    case kReload:
        GetMemoryCache()->Remove(resource);
        FALLTHROUGH;
    case kLoad:
        resource = CreateResourceForLoading(params, factory);
        break;
    case kRevalidate:
        InitializeRevalidation(resource_request, resource);
        break;
    case kUse:
        if (resource_request.AllowsStaleResponse() &&
            resource->ShouldRevalidateStaleResponse()) {
            ScheduleStaleRevalidate(resource);
        }

        if (resource->IsLinkPreload() && !params.IsLinkPreload())
            resource->SetLinkPreload(false);
        break;
    }
#endif
    ASSERT(nullptr != resource);
    ASSERT(resource->GetType() == resourceType);

    if (policy != kUse)
        resource->SetIdentifier(identifier);

    if (client)
        client->SetResource(resource, Context().GetLoadingTaskRunner().get());

    // TODO(yoav): It is not clear why preloads are exempt from this check. Can we
    // remove the exemption?
    if (!params.IsSpeculativePreload() || policy != kUse)
    {
        // When issuing another request for a resource that is already in-flight
        // make sure to not demote the priority of the in-flight request. If the new
        // request isn't at the same priority as the in-flight request, only allow
        // promotions. This can happen when a visible image's priority is increased
        // and then another reference to the image is parsed (which would be at a
        // lower priority).
        if (resourceRequest.Priority() > resource->GetResourceRequest().Priority())
            resource->DidChangePriority(resourceRequest.Priority(), 0);
        // TODO(yoav): I'd expect the stated scenario to not go here, as its policy
        // would be Use.
    }

    // If only the fragment identifiers differ, it is the same resource.
    ASSERT(EqualIgnoringFragmentIdentifier(resource->Url(), params.Url()));
#ifndef BLINKIT_CRAWLER_ONLY
    RequestLoadStarted(identifier, resource.get(), params, policy, isStaticData);
    if (!is_stale_revalidation) {
        cached_resources_map_.Set(
            MemoryCache::RemoveFragmentIdentifierIfNeeded(params.Url()), resource);
    }
    document_resources_.insert(resource);
#endif

    // Returns with an existing resource if the resource does not need to start
    // loading immediately. If revalidation policy was determined as |Revalidate|,
    // the resource was already initialized for the revalidation here, but won't
    // start loading.
    if (ResourceNeedsLoad(resource.get(), params, policy))
    {
        if (!StartLoad(resource))
        {
            ASSERT(false); // BKTODO:
#if 0
            resource->FinishAsError(ResourceError::CancelledError(params.Url()),
                Context().GetLoadingTaskRunner().get());
#endif
        }
    }

    if (policy != kUse)
        InsertAsPreloadIfNecessary(resource.get(), params, resourceType);

    return resource;
}

std::shared_ptr<Resource> ResourceFetcher::ResourceForBlockedRequest(
    const FetchParameters &params,
    const ResourceFactory &factory,
    ResourceRequestBlockedReason blockedReason,
    ResourceClient  *client)
{
    std::shared_ptr<base::SingleThreadTaskRunner> taskRunner = Context().GetLoadingTaskRunner();

    std::shared_ptr<Resource> resource = factory.Create(params.GetResourceRequest(), params.Options(),
        params.DecoderOptions());
    if (nullptr != client)
        client->SetResource(resource, taskRunner.get());
    ASSERT(false); // BKTODO:
#if 0
    resource->FinishAsError(ResourceError::CancelledDueToAccessCheckError(
        params.Url(), blocked_reason),
        Context().GetLoadingTaskRunner().get());
#endif
    return resource;
}

#ifndef BLINKIT_CRAWLER_ONLY
void ResourceFetcher::RequestLoadStarted(
    unsigned long identifier,
    Resource *resource,
    const FetchParameters &params,
    RevalidationPolicy policy,
    bool isStaticData)
{
    BkURL url = RemoveFragmentIdentifierIfNeeded(params.Url());
    if (policy == kUse && resource->GetStatus() == ResourceStatus::kCached &&
        !cached_resources_map_.Contains(url)) {
        // Loaded from MemoryCache.
        DidLoadResourceFromMemoryCache(identifier, resource,
            params.GetResourceRequest());
    }

    if (is_static_data)
        return;

    if (policy == kUse && !resource->StillNeedsLoad() &&
        !cached_resources_map_.Contains(url)) {
        // Resources loaded from memory cache should be reported the first time
        // they're used.
        scoped_refptr<ResourceTimingInfo> info = ResourceTimingInfo::Create(
            params.Options().initiator_info.name, CurrentTimeTicks(),
            resource->GetType() == ResourceType::kMainResource);
        PopulateTimingInfo(info.get(), resource);
        info->ClearLoadTimings();
        info->SetLoadFinishTime(info->InitialTime());
        scheduled_resource_timing_reports_.push_back(std::move(info));
        if (!resource_timing_report_timer_.IsActive())
            resource_timing_report_timer_.StartOneShot(TimeDelta(), FROM_HERE);
    }
}
#endif // BLINKIT_CRAWLER_ONLY

bool ResourceFetcher::ResourceNeedsLoad(Resource *resource, const FetchParameters &params, RevalidationPolicy policy)
{
    // Defer a font load until it is actually needed unless this is a link
    // preload.
    if (resource->GetType() == ResourceType::kFont && !params.IsLinkPreload())
        return false;

#ifndef BLINKIT_CRAWLER_ONLY
    // Defer loading images either when:
    // - images are disabled
    // - instructed to defer loading images from network
    if (resource->GetType() == ResourceType::kImage &&
        (ShouldDeferImageLoad(resource->Url()) ||
            params.GetImageRequestOptimization() ==
            FetchParameters::kDeferImageLoad)) {
        return false;
    }
#endif
    return policy != kUse || resource->StillNeedsLoad();
}

bool ResourceFetcher::StartLoad(std::shared_ptr<Resource> &resource)
{
    ASSERT(nullptr != resource);
    ASSERT(resource->StillNeedsLoad());

    ResourceRequest request(resource->GetResourceRequest());
    ResourceLoader *loader = nullptr;

    {
        // Forbids JavaScript/revalidation until start()
        // to prevent unintended state transitions.
        Resource::RevalidationStartForbiddenScope revalidationStartForbiddenScope(resource.get());
        ScriptForbiddenScope scriptForbiddenScope;

        if (!Context().ShouldLoadNewResource(resource->GetType()) && IsMainThread())
        {
            ASSERT(false); // BKTODO:
#if 0
            GetMemoryCache()->Remove(resource);
#endif
            return false;
        }

        ResourceResponse response;

        Context().DispatchWillSendRequest(resource->Identifier(), request, response, resource->GetType(),
            resource->Options().initiator_info);

        // TODO(shaochuan): Saving modified ResourceRequest back to |resource|,
        // remove once dispatchWillSendRequest() takes const ResourceRequest.
        // crbug.com/632580
        resource->SetResourceRequest(request);

        using QuotaType = decltype(m_inflightKeepaliveBytes);
        QuotaType size = 0;
        if (request.GetKeepalive() && request.HttpBody())
        {
            ASSERT(false); // BKTODO:
#if 0
            auto original_size = request.HttpBody()->SizeInBytes();
            DCHECK_LE(inflight_keepalive_bytes_, kKeepaliveInflightBytesQuota);
            if (original_size > std::numeric_limits<QuotaType>::max())
                return false;
            size = static_cast<QuotaType>(original_size);
            if (kKeepaliveInflightBytesQuota - inflight_keepalive_bytes_ < size)
                return false;

            inflight_keepalive_bytes_ += size;
#endif
        }

        loader = ResourceLoader::Create(this, resource, size);
        if (resource->ShouldBlockLoadEvent())
            m_loaders.insert(loader);
        else
            m_nonBlockingLoaders.insert(loader);
    }

    loader->Start();

    {
        Resource::RevalidationStartForbiddenScope revalidationStartForbiddenScope(resource.get());
        ScriptForbiddenScope scriptForbiddenScope;

        // NotifyStartLoad() shouldn't cause AddClient/RemoveClient().
        Resource::ProhibitAddRemoveClientInScope prohibitAddRemoveClientInScope(resource.get());
        if (!resource->IsLoaded())
            resource->NotifyStartLoad();
    }
    return true;
}

void ResourceFetcher::StopFetching(void)
{
    StopFetchingInternal(StopFetchingTarget::kExcludingKeepaliveLoaders);
}

void ResourceFetcher::StopFetchingInternal(StopFetchingTarget target)
{
    if (StopFetchingTarget::kIncludingKeepaliveLoaders != target)
        return;

    // TODO(toyoshim): May want to suspend scheduler while canceling loaders so
    // that the cancellations below do not awake unnecessary scheduling.

    ASSERT(false); // BKTODO:
#if 0
    std::vector<ResourceLoader *> loadersToCancel;
    for (const auto &loader : m_nonBlockingLoaders)
    {
        ASSERT(false); // BKTODO:
        if (target == StopFetchingTarget::kIncludingKeepaliveLoaders ||
            !loader->ShouldBeKeptAliveWhenDetached()) {
            loaders_to_cancel.push_back(loader);
        }
    }
    for (const auto& loader : loaders_) {
        if (target == StopFetchingTarget::kIncludingKeepaliveLoaders ||
            !loader->ShouldBeKeptAliveWhenDetached()) {
            loaders_to_cancel.push_back(loader);
        }
    }

    for (const auto& loader : loaders_to_cancel) {
        if (loaders_.Contains(loader) || non_blocking_loaders_.Contains(loader))
            loader->Cancel();
    }
#endif
}

}  // namespace blink
