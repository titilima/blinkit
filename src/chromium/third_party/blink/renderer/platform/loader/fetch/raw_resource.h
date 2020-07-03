// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: raw_resource.h
// Description: RawResource Class
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller <mueller@kde.org>
    Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
    Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.

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
*/

#ifndef BLINKIT_BLINK_RAW_RESOURCE_H
#define BLINKIT_BLINK_RAW_RESOURCE_H

#include "third_party/blink/renderer/platform/loader/fetch/resource.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_client.h"

namespace blink {

class FetchParameters;
class RawResourceClient;
class ResourceFetcher;
class SourceKeyedCachedMetadataHandler;
class SubstituteData;

class RawResource final : public Resource, public std::enable_shared_from_this<RawResource>
{
public:
    static std::shared_ptr<RawResource> FetchMainResource(FetchParameters &params, ResourceFetcher *fetcher,
        RawResourceClient *client, const SubstituteData &substituteData);

    // Used for code caching of scripts with source code inline in the HTML.
    // Returns a cache handler which can store multiple cache metadata entries,
    // keyed by the source code of the script. This is valid only if type is
    // kMainResource.
    SourceKeyedCachedMetadataHandler* InlineScriptCacheHandler(void);
private:
    class RawResourceFactory : public NonTextResourceFactory
    {
    public:
        explicit RawResourceFactory(ResourceType type) : NonTextResourceFactory(type) {}

        std::shared_ptr<Resource> Create(const ResourceRequest &request, const ResourceLoaderOptions &options) const override;
    };

    RawResource(const ResourceRequest &resourceRequest, ResourceType type, const ResourceLoaderOptions &options);

    // Resource
    void ResponseReceived(const ResourceResponse &response) override;
    bool ShouldIgnoreHTTPStatusCodeErrors(void) const override { return !IsLinkPreload(); }
    void DidAddClient(ResourceClient *c) override;
};

// TODO(yhirano): Recover #if ENABLE_SECURITY_ASSERT when we stop adding
// RawResources to MemoryCache.
inline bool IsRawResource(ResourceType type)
{
    switch (type)
    {
        case ResourceType::kMainResource:   case ResourceType::kRaw:   case ResourceType::kTextTrack:
        case ResourceType::kAudio:          case ResourceType::kVideo: case ResourceType::kManifest:
        case ResourceType::kImportResource:
            return true;
        default:
            return false;
    }
}

inline bool IsRawResource(const Resource &resource)
{
    return IsRawResource(resource.GetType());
}

inline std::shared_ptr<RawResource> ToRawResource(const std::shared_ptr<Resource> &resource)
{
    ASSERT(!resource || IsRawResource(*resource));
    return static_cast<RawResource *>(resource.get())->shared_from_this();
}

inline RawResource* ToRawResource(Resource *resource)
{
    ASSERT(nullptr == resource || IsRawResource(*resource));
    return static_cast<RawResource *>(resource);
}

class RawResourceClient : public ResourceClient
{
public:
    virtual void ResponseReceived(Resource *resource, const ResourceResponse &response) {}
private:
    // ResourceClient overrides
    bool IsRawResourceClient(void) const final { return true; }
};

}  // namespace blink

#endif  // BLINKIT_BLINK_RAW_RESOURCE_H
