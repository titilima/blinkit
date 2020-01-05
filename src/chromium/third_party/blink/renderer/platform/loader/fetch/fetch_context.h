// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: fetch_context.h
// Description: FetchContext Class
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

#ifndef BLINKIT_BLINK_FETCH_CONTEXT_H
#define BLINKIT_BLINK_FETCH_CONTEXT_H

#pragma once

#include <optional>
#include "third_party/blink/public/platform/resource_request_blocked_reason.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_initiator_info.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class Resource;
class ResourceError;
struct ResourceLoaderOptions;
class ResourceRequest;
class ResourceResponse;
enum class ResourceType : uint8_t;
class WebURLLoader;

enum FetchResourceType { kFetchMainResource, kFetchSubresource };

class FetchContext : public GarbageCollectedFinalized<FetchContext>
{
    WTF_MAKE_NONCOPYABLE(FetchContext);
public:
    virtual ~FetchContext(void);

    virtual void AddAdditionalRequestHeaders(ResourceRequest &request, FetchResourceType type) {}

    virtual void RecordDataUriWithOctothorpe(void) {}

    // This internally dispatches WebLocalFrameClient::willSendRequest and hooks
    // request interceptors like ServiceWorker and ApplicationCache.
    // This may modify the request.
    enum class RedirectType { kForRedirect, kNotForRedirect };
    virtual void PrepareRequest(ResourceRequest &request, RedirectType redirectType) {}

    virtual std::optional<ResourceRequestBlockedReason> CanRequest(ResourceType type, const ResourceRequest &resourceRequest,
        const BlinKit::BkURL &url, const ResourceLoaderOptions &options, ResourceRequest::RedirectStatus redirectStatus) const
    {
        return ResourceRequestBlockedReason::kOther;
    }

    virtual bool ShouldLoadNewResource(ResourceType type) const { return false; }

    // The last callback before a request is actually sent to the browser process.
    // TODO(https://crbug.com/632580): make this take const ResourceRequest&.
    virtual void DispatchWillSendRequest(unsigned long identifier, ResourceRequest &request,
        const ResourceResponse &redirectResponse, ResourceType resourceType,
        const FetchInitiatorInfo &initiatorInfo = FetchInitiatorInfo());
    virtual void DispatchDidReceiveResponse(unsigned long identifier, const ResourceResponse &response, Resource *resource) {}
    virtual void DispatchDidReceiveData(unsigned long identifier, const char *data, int dataLength) {}
    virtual void DispatchDidFinishLoading(unsigned long identifier) {}
    virtual void DidLoadResource(Resource *resource) {}
    virtual void DispatchDidFail(const BlinKit::BkURL &url, unsigned long identifier, const ResourceError &error) {}

    virtual bool IsDetached(void) const { return false; }
    bool DefersLoading(void) const { return false; } // BKTODO: This always returns false, because the page cannot be paused in BlinKit.

    // Returns a task runner intended for loading tasks. Should work even in a
    // worker context, where FrameScheduler doesn't exist, but the returned
    // base::SingleThreadTaskRunner will not work after the context detaches
    // (after Detach() is called, this will return a generic timer suitable for
    // post-detach actions like keepalive requests.
    virtual std::shared_ptr<base::SingleThreadTaskRunner> GetLoadingTaskRunner(void)
    {
        return m_taskRunner;
    }

    // Called when the underlying context is detached. Note that some
    // FetchContexts continue working after detached (e.g., for fetch() operations
    // with "keepalive" specified).
    // Returns a "detached" fetch context which can be null.
    virtual FetchContext* Detach(void) { return nullptr; }
protected:
    explicit FetchContext(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner);
private:
    std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;
};

}  // namespace blink

#endif // BLINKIT_BLINK_FETCH_CONTEXT_H
