// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_fetcher.h
// Description: ResourceFetcher Class
//      Author: Ziming Li
//     Created: 2019-10-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller <mueller@kde.org>
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

#ifndef BLINKIT_BLINK_RESOURCE_FETCHER_H
#define BLINKIT_BLINK_RESOURCE_FETCHER_H

#pragma once

#include <optional>
#include <unordered_set>
#include "third_party/blink/public/platform/resource_request_blocked_reason.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_parameters.h"
#include "third_party/blink/renderer/platform/loader/fetch/substitute_data.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_load_priority.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_priority.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace blink {

class FetchContext;
class FetchParameters;
class Resource;
class ResourceClient;
class ResourceError;
class ResourceFactory;
class ResourceLoader;
enum class ResourceType : uint8_t;

class ResourceFetcher : public GarbageCollectedFinalized<ResourceFetcher>
{
    WTF_MAKE_NONCOPYABLE(ResourceFetcher);
public:
    BK_DECLARE_GC_NAME(ResourceFetcher)

    static ResourceFetcher* Create(void)
    {
        return new ResourceFetcher;
    }
    static ResourceFetcher* Create(FetchContext *context)
    {
        return new ResourceFetcher(context);
    }
    ~ResourceFetcher(void);
    void Trace(Visitor *visitor);

    FetchContext& Context(void) const;
    void ClearContext(void);

    // Triggers a fetch based on the given FetchParameters (if there isn't a
    // suitable Resource already cached) and registers the given ResourceClient
    // with the Resource. Guaranteed to return a non-null Resource of the subtype
    // specified by ResourceFactory::GetType().
    Resource* RequestResource(FetchParameters &params, const ResourceFactory &factory, ResourceClient *client,
        const SubstituteData &substituteData = SubstituteData());

    // Binds the given Resource instance to this ResourceFetcher instance to
    // start loading the Resource actually.
    // Usually, RequestResource() calls this method internally, but needs to
    // call this method explicitly on cases such as ResourceNeedsLoad() returning
    // false.
    bool StartLoad(Resource *resource);
    void StopFetching(void);
    enum LoaderFinishType { kDidFinishLoading, kDidFinishFirstPartInMultipart };
    void HandleLoaderFinish(Resource *resource, LoaderFinishType type);
    void HandleLoaderError(Resource *resource, const ResourceError &error);

    void LoosenLoadThrottlingPolicy(void) { BKLOG("// BKTODO: scheduler_->LoosenThrottlingPolicy();"); }

    int BlockingRequestCount(void) const;
private:
    ResourceFetcher(void);
    ResourceFetcher(FetchContext *context);

    Resource* CreateResourceForLoading(const FetchParameters &params, const ResourceFactory &factory);

    ResourceLoadPriority ComputeLoadPriority(ResourceType type, const ResourceRequest &resourceRequest,
        ResourcePriority::VisibilityStatus visibility, FetchParameters::DeferOption deferOption = FetchParameters::kNoDefer,
        FetchParameters::SpeculativePreloadType speculativePreloadType = FetchParameters::SpeculativePreloadType::kNotSpeculative,
        bool isLinkPreload = false);

    std::optional<ResourceRequestBlockedReason> PrepareRequest(FetchParameters &params, const ResourceFactory &factory,
        const SubstituteData &substituteData, unsigned long identifier);

    Resource* ResourceForBlockedRequest(const FetchParameters &params, const ResourceFactory &factory,
        ResourceRequestBlockedReason blockedReason, ResourceClient *client);

    void InsertAsPreloadIfNecessary(Resource *resource, const FetchParameters &params, ResourceType type);

    enum RevalidationPolicy { kUse, kRevalidate, kReload, kLoad };

#ifndef BLINKIT_CRAWLER_ONLY
    void RequestLoadStarted(unsigned long identifier, Resource *resource, const FetchParameters &params,
        RevalidationPolicy policy, bool isStaticData = false);
#endif
    bool ResourceNeedsLoad(Resource *resource, const FetchParameters &params, RevalidationPolicy policy);

    void RemoveResourceLoader(ResourceLoader *loader);
    void HandleLoadCompletion(Resource *resource);

    Member<FetchContext> m_context;
    std::unordered_set<ResourceLoader *> m_loaders;
    std::unordered_set<ResourceLoader *> m_nonBlockingLoaders;

    // When populated, forces Resources to remain alive across a navigation, to
    // increase the odds the next document will be able to reuse resources from
    // the previous page. Unpopulated unless experiment is enabled.
    std::unordered_set<Resource *> m_resourcesFromPreviousFetcher;

    bool m_imageFetched : 1;

    uint32_t m_inflightKeepaliveBytes = 0;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_RESOURCE_FETCHER_H
