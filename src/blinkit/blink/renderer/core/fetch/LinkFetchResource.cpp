// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LinkFetchResource.cpp
// Description: LinkFetchResource Class
//      Author: Ziming Li
//     Created: 2021-07-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "core/fetch/LinkFetchResource.h"

#include "core/fetch/FetchRequest.h"
#include "core/fetch/ResourceFetcher.h"

namespace blink {

ResourcePtr<Resource> LinkFetchResource::fetch(Resource::Type type, FetchRequest& request, ResourceFetcher* fetcher)
{
    ASSERT(type == LinkPrefetch || type == LinkSubresource);
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    ASSERT(request.resourceRequest().frameType() == WebURLRequest::FrameTypeNone);
    fetcher->determineRequestContext(request.mutableResourceRequest(), type);
    return fetcher->requestResource(request, LinkResourceFactory(type));
#endif
}

LinkFetchResource::LinkFetchResource(const ResourceRequest& request, Type type)
    : Resource(request, type)
{
}

LinkFetchResource::~LinkFetchResource()
{
}

}
