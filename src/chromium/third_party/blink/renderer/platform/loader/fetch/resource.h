// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource.h
// Description: Resource Class
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller <mueller@kde.org>
    Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
    Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
    rights reserved.

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

#ifndef BLINKIT_BLINK_RESOURCE_H
#define BLINKIT_BLINK_RESOURCE_H

#pragma once

#include <optional>
#include <unordered_set>
#include <vector>
#include "base/auto_reset.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_error.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader_options.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_response.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_status.h"
#include "third_party/blink/renderer/platform/loader/fetch/text_resource_decoder_options.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class CachedMetadataHandler;
class ResourceClient;
class ResourceFinishObserver;
class ResourceLoader;
class SharedBuffer;

enum class ResourceType : uint8_t {
    kMainResource,
    kImage,
    kCSSStyleSheet,
    kScript,
    kFont,
    kRaw,
    kXSLStyleSheet,
    kLinkPrefetch,
    kTextTrack,
    kImportResource,
    kAudio,
    kVideo,
    kManifest,
};

class Resource : public GarbageCollectedFinalized<Resource>
{
public:
    virtual ~Resource(void);

    virtual WTF::TextEncoding Encoding(void) const { return WTF::TextEncoding(); }
    ResourceType GetType(void) const { return m_type; }
    void SetStatus(ResourceStatus status) { m_status = status; }
    bool IsLoading(void) const { return ResourceStatus::kPending == m_status; }
    bool ErrorOccurred(void) const { return ResourceStatus::kLoadError == m_status || ResourceStatus::kDecodeError == m_status; }
    bool IsLoaded(void) const { return m_status > ResourceStatus::kPending; }
    bool StillNeedsLoad(void) const { return m_status < ResourceStatus::kPending; }

    const ResourceError& GetResourceError(void) const
    {
        ASSERT(m_error);
        return *m_error;
    }
    bool WasCanceled(void) const { return m_error && m_error->IsCancellation(); }

    unsigned long Identifier(void) const { return m_identifier; }
    void SetIdentifier(unsigned long identifier) { m_identifier = identifier; }

    bool IsLinkPreload(void) const { return m_linkPreload; }
    void SetLinkPreload(bool isLinkPreload) { m_linkPreload = isLinkPreload; }
    bool IsCacheValidator(void) const { return m_isRevalidating; }

    const std::unordered_set<ResourceClient *>& Clients(void) const { return m_clients; }
    const ResourceLoaderOptions& Options(void) const { return m_options; }

    const ResourceRequest& GetResourceRequest(void) const { return m_resourceRequest; }
    void SetResourceRequest(const ResourceRequest &resourceRequest) { m_resourceRequest = resourceRequest; }
    const ResourceRequest& LastResourceRequest(void) const;
    const GURL& Url(void) const { return GetResourceRequest().Url(); }
    
    ResourceLoader* Loader(void) const { return m_loader.get(); }
    void SetLoader(std::unique_ptr<ResourceLoader> &loader);

    virtual std::shared_ptr<const SharedBuffer> ResourceBuffer(void) const
    {
        ASSERT(GetType() != ResourceType::kImage); // BKTODO:
        return m_data;
    }

    const ResourceResponse& GetResponse(void) const { return m_response; }
    void SetResponse(const ResourceResponse &response);

    virtual void AppendData(const char *data, size_t length);
    virtual void FinishAsError(const ResourceError &error, base::SingleThreadTaskRunner *taskRunner);

    bool ShouldBlockLoadEvent(void) const;
    bool IsLoadEventBlockingResourceType(void) const;

    // If this Resource is already finished when AddClient is called, the
    // ResourceClient will be notified asynchronously by a task scheduled
    // on the given base::SingleThreadTaskRunner. Otherwise, the given
    // base::SingleThreadTaskRunner is unused.
    void AddClient(ResourceClient *client, base::SingleThreadTaskRunner *taskRunner);
    void RemoveClient(ResourceClient *client);

    void DidChangePriority(ResourceLoadPriority loadPriority, int intraPriorityValue);

    // Used to notify ImageResourceContent of the start of actual loading.
    // JavaScript calls or client/observer notifications are disallowed inside
    // NotifyStartLoad().
    virtual void NotifyStartLoad(void)
    {
        ASSERT(ResourceType::kImage != m_type); // BKTODO:
        ASSERT(ResourceStatus::kNotStarted == m_status);
        m_status = ResourceStatus::kPending;
    }
    virtual void ResponseReceived(const ResourceResponse &response);
    virtual bool ShouldIgnoreHTTPStatusCodeErrors(void) const { return false; }
    // Computes the status of an object after loading.
    virtual void Finish(base::SingleThreadTaskRunner *taskRunner);

    class ProhibitAddRemoveClientInScope : public base::AutoReset<bool>
    {
    public:
        ProhibitAddRemoveClientInScope(Resource *resource) : AutoReset(&resource->m_isAddRemoveClientProhibited, true) {}
    };
    class RevalidationStartForbiddenScope : public base::AutoReset<bool>
    {
    public:
        RevalidationStartForbiddenScope(Resource* resource) : AutoReset(&resource->m_isRevalidationStartForbidden, true) {}
    };
protected:
    Resource(const ResourceRequest &request, ResourceType type, const ResourceLoaderOptions &options);

    void SetEncodedSize(size_t encodedSize);

    bool HasClient(ResourceClient *client) const;
    std::shared_ptr<SharedBuffer> Data(void) const { return m_data; }
    void ClearData(void);

    struct RedirectPair
    {
        DISALLOW_NEW();
    public:
        explicit RedirectPair(const ResourceRequest &request, const ResourceResponse &redirectResponse)
            : m_request(request), m_redirectResponse(redirectResponse)
        {
        }

        ResourceRequest m_request;
        ResourceResponse m_redirectResponse;
    };
    const std::vector<RedirectPair>& RedirectChain(void) const { return m_redirectChain; }

    virtual void SetEncoding(const String &encoding) {}

    void WillAddClientOrObserver(void);
    virtual bool HasClientsOrObservers(void) const;
    virtual void DidAddClient(ResourceClient *c);
    void DidRemoveClientOrObserver(void);
    virtual void AllClientsAndObserversRemoved(void);
    virtual void NotifyFinished(void);
    void MarkClientFinished(ResourceClient *client);

    CachedMetadataHandler* CacheHandler(void) { return m_cacheHandler.get(); }
private:
    void TriggerNotificationForFinishObservers(base::SingleThreadTaskRunner *taskRunner);

    ResourceType m_type;
    ResourceStatus m_status = ResourceStatus::kNotStarted;

    std::unique_ptr<CachedMetadataHandler> m_cacheHandler;

    std::optional<ResourceError> m_error;

    unsigned long m_identifier = 0;

    bool m_linkPreload = false;
    bool m_isRevalidating = false;
    bool m_isAlive = false;
    bool m_isAddRemoveClientProhibited = false;
    bool m_isRevalidationStartForbidden = false;

    // Ordered list of all redirects followed while fetching this resource.
    std::vector<RedirectPair> m_redirectChain;

    std::unordered_set<ResourceClient *> m_clients;
    std::unordered_set<ResourceClient *> m_clientsAwaitingCallback;
    std::unordered_set<ResourceClient *> m_finishedClients;
    std::unordered_set<ResourceFinishObserver *> m_finishObservers;

    ResourceLoaderOptions m_options;

    // BKTODO: double m_responseTimestamp;

    ResourceRequest m_resourceRequest;
    std::unique_ptr<ResourceLoader> m_loader;
    ResourceResponse m_response;

    std::shared_ptr<SharedBuffer> m_data;
};

class ResourceFactory
{
    STACK_ALLOCATED();
public:
    virtual std::shared_ptr<Resource> Create(const ResourceRequest &request, const ResourceLoaderOptions &options,
        const TextResourceDecoderOptions &decoderOptions) const = 0;

    ResourceType GetType(void) const { return m_type; }
    TextResourceDecoderOptions::ContentType ContentType(void) const { return m_contentType; }
protected:
    explicit ResourceFactory(ResourceType type, TextResourceDecoderOptions::ContentType contentType)
        : m_type(type), m_contentType(contentType)
    {
    }

    ResourceType m_type;
    TextResourceDecoderOptions::ContentType m_contentType;
};

class NonTextResourceFactory : public ResourceFactory
{
protected:
    explicit NonTextResourceFactory(ResourceType type);
private:
    virtual std::shared_ptr<Resource> Create(const ResourceRequest &request, const ResourceLoaderOptions &options) const = 0;

    std::shared_ptr<Resource> Create(const ResourceRequest &request, const ResourceLoaderOptions &options,
        const TextResourceDecoderOptions &) const final;
};

}  // namespace blink

#endif
