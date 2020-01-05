// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource.cpp
// Description: Resource Class
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller (mueller@kde.org)
    Copyright (C) 2002 Waldo Bastian (bastian@kde.org)
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

#include "resource.h"

#include "third_party/blink/renderer/platform/loader/fetch/resource_client.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader.h"
#include "third_party/blink/renderer/platform/loader/fetch/cached_metadata_handler.h"
#include "third_party/blink/renderer/platform/shared_buffer.h"

namespace blink {

Resource::Resource(const ResourceRequest &request, ResourceType type, const ResourceLoaderOptions &options)
    : m_type(type)
    // BKTODO: , m_responseTimestamp(CurrentTime())
    , m_resourceRequest(request)
    , m_options(options)
{
}

Resource::~Resource(void) = default;

static bool NeedsSynchronousCacheHit(ResourceType type, const ResourceLoaderOptions &options)
{
    // Synchronous requests must always succeed or fail synchronously.
    if (options.synchronous_policy == kRequestSynchronously)
        return true;
    // Some resources types default to return data synchronously. For most of
    // these, it's because there are layout tests that expect data to return
    // synchronously in case of cache hit. In the case of fonts, there was a
    // performance regression.
    // FIXME: Get to the point where we don't need to special-case sync/async
    // behavior for different resource types.
    if (ResourceType::kCSSStyleSheet == type)
        return true;
    if (ResourceType::kScript == type)
        return true;
    if (ResourceType::kFont == type)
        return true;
    return false;
}

void Resource::AddClient(ResourceClient *client, base::SingleThreadTaskRunner *taskRunner)
{
    ASSERT(!m_isAddRemoveClientProhibited);

    WillAddClientOrObserver();

    if (m_isRevalidating)
    {
        m_clients.insert(client);
        return;
    }

    // If an error has occurred or we have existing data to send to the new client
    // and the resource type supports it, send it asynchronously.
    if ((ErrorOccurred() || !GetResponse().IsNull()) && !NeedsSynchronousCacheHit(GetType(), m_options))
    {
        ASSERT(false); // BKTODO:
#if 0
        clients_awaiting_callback_.insert(client);
        if (!async_finish_pending_clients_task_.IsActive()) {
            async_finish_pending_clients_task_ = PostCancellableTask(
                *task_runner, FROM_HERE,
                WTF::Bind(&Resource::FinishPendingClients, WrapWeakPersistent(this)));
        }
#endif
        return;
    }

    m_clients.insert(client);
    DidAddClient(client);
}

void Resource::AllClientsAndObserversRemoved(void)
{
    if (m_loader)
        m_loader->ScheduleCancel();
    // BKTODO: Check image & font overrides.
}

void Resource::AppendData(const char *data, size_t length)
{
    ASSERT(!m_isRevalidating);
    ASSERT(!ErrorOccurred());
    if (m_options.data_buffering_policy == kBufferData)
    {
        if (m_data)
            m_data->Append(data, length);
        else
            m_data = SharedBuffer::Create(data, length);
        SetEncodedSize(m_data->size());
    }

    std::vector<ResourceClient *> clients(Clients().begin(), Clients().end());
    for (ResourceClient *c : clients)
        c->DataReceived(this, data, length);
}

void Resource::ClearData(void)
{
    m_data.reset();
}

void Resource::DidAddClient(ResourceClient *c)
{
    if (std::shared_ptr<SharedBuffer> data = Data())
    {
        ASSERT(false); // BKTODO:
#if 0
        for (const auto& span : *data) {
            c->DataReceived(this, span.data(), span.size());
            // Stop pushing data if the client removed itself.
            if (!HasClient(c))
                break;
        }
#endif
    }
    if (!HasClient(c))
        return;
    if (IsLoaded())
    {
        ASSERT(false); // BKTODO:
#if 0
        c->NotifyFinished(this);
        if (clients_.Contains(c)) {
            finished_clients_.insert(c);
            clients_.erase(c);
        }
#endif
    }
    BKLOG("// BKTODO: Check child classes.");
}

void Resource::DidChangePriority(ResourceLoadPriority loadPriority, int intraPriorityValue)
{
    ASSERT(false); // BKTODO:
}

void Resource::DidRemoveClientOrObserver(void)
{
    if (!HasClientsOrObservers() && m_isAlive)
    {
        m_isAlive = false;
        AllClientsAndObserversRemoved();

#if 0 // BKTODO: Check if necessary.
        // RFC2616 14.9.2:
        // "no-store: ... MUST make a best-effort attempt to remove the information
        // from volatile storage as promptly as possible"
        // "... History buffers MAY store such responses as part of their normal
        // operation."
        // We allow non-secure content to be reused in history, but we do not allow
        // secure content to be reused.
        if (HasCacheControlNoStoreHeader() && Url().ProtocolIs("https") &&
            IsMainThread())
            GetMemoryCache()->Remove(this);
#endif
    }
}

void Resource::Finish(base::SingleThreadTaskRunner *taskRunner)
{
    ASSERT(!m_isRevalidating);
#if 0 // BKTODO:
    load_finish_time_ = load_finish_time;
#endif
    if (!ErrorOccurred())
        m_status = ResourceStatus::kCached;
    m_loader.reset();
    // BKTODO: CheckResourceIntegrity();
    TriggerNotificationForFinishObservers(taskRunner);
    NotifyFinished();
}

void Resource::FinishAsError(const ResourceError &error, base::SingleThreadTaskRunner *taskRunner)
{
    m_error = error;
    m_isRevalidating = false;

#if 0
    if (IsMainThread())
        GetMemoryCache()->Remove(this);
#endif

    bool failedDuringStart = ResourceStatus::kNotStarted == m_status;
    if (!ErrorOccurred())
        SetStatus(ResourceStatus::kLoadError);
    ASSERT(ErrorOccurred());
    ClearData();
    m_loader = nullptr;
    // BKTODO: CheckResourceIntegrity();
    TriggerNotificationForFinishObservers(taskRunner);

    ASSERT(!failedDuringStart); // BKTODO:
#if 0
    // Most resource types don't expect to succeed or fail inside
    // ResourceFetcher::RequestResource(). If the request does complete
    // immediately, the convention is to notify the client asynchronously
    // unless the type is exempted for historical reasons (mostly due to
    // performance implications to making those notifications asynchronous).
    // So if this is an immediate failure (i.e., before NotifyStartLoad()),
    // post a task if the Resource::Type supports it.
    if (failed_during_start && !NeedsSynchronousCacheHit(GetType(), options_)) {
        task_runner->PostTask(FROM_HERE, WTF::Bind(&Resource::NotifyFinished,
            WrapWeakPersistent(this)));
    }
    else {
        NotifyFinished();
    }
#else
    NotifyFinished();
#endif
    // BKTODO: Check image overrides
}

bool Resource::HasClient(ResourceClient * client) const
{
    if (m_clients.find(client) != std::end(m_clients))
        return true;
    ASSERT(false); // BKTODO:
#if 0
    return clients_awaiting_callback_.Contains(client) ||
        finished_clients_.Contains(client);
#endif
    return false;
}

bool Resource::HasClientsOrObservers(void) const
{
    if (m_clients.empty())
        return false;
    ASSERT(false); // BKTODO:
#if 0
    return !clients_awaiting_callback_.IsEmpty() ||
        !finished_clients_.IsEmpty() || !finish_observers_.IsEmpty();
#endif
    return true;
}

bool Resource::IsLoadEventBlockingResourceType(void) const
{
    switch (m_type)
    {
        case ResourceType::kMainResource:
        case ResourceType::kImage:
        case ResourceType::kCSSStyleSheet:
        case ResourceType::kScript:
        case ResourceType::kFont:
        case ResourceType::kXSLStyleSheet:
        case ResourceType::kImportResource:
            return true;
        case ResourceType::kRaw:
        case ResourceType::kLinkPrefetch:
        case ResourceType::kTextTrack:
        case ResourceType::kAudio:
        case ResourceType::kVideo:
        case ResourceType::kManifest:
            return false;
    }
    NOTREACHED();
    return false;
}

void Resource::MarkClientFinished(ResourceClient *client)
{
    auto it = m_clients.find(client);
    if (std::end(m_clients) != it)
    {
        m_finishedClients.insert(client);
        m_clients.erase(it);
    }
}

void Resource::NotifyFinished(void)
{
    ASSERT(IsLoaded());

    std::vector<ResourceClient *> clients(m_clients.begin(), m_clients.end());
    for (ResourceClient *c : clients)
    {
        MarkClientFinished(c);
        c->NotifyFinished(this);
    }
    // BKTODO: Check resource overrides.
}

void Resource::RemoveClient(ResourceClient *client)
{
    ASSERT(!m_isAddRemoveClientProhibited);

    auto it = m_finishedClients.find(client);
    if (std::end(m_finishedClients) != it)
    {
        m_finishedClients.erase(it);
    }
    else
    {
        it = m_clientsAwaitingCallback.find(client);
        if (std::end(m_clientsAwaitingCallback) != it)
            m_clientsAwaitingCallback.erase(it);
        else
            m_clients.erase(client);
    }

#if 0 // BKTODO:
    if (clients_awaiting_callback_.IsEmpty() &&
        async_finish_pending_clients_task_.IsActive()) {
        async_finish_pending_clients_task_.Cancel();
    }
#endif

    DidRemoveClientOrObserver();
}

void Resource::ResponseReceived(const ResourceResponse &response)
{
    ASSERT(ResourceType::kImage != m_type); // BKTODO:
    // BKTODO: m_responseTimestamp = CurrentTime();
    if (m_isRevalidating)
    {
        ASSERT(false); // BKTODO:
#if 0
        if (response.HttpStatusCode() == 304) {
            RevalidationSucceeded(response);
            return;
        }
        RevalidationFailed();
#endif
    }
    SetResponse(response);
    String encoding = response.TextEncodingName();
    if (!encoding.IsNull())
        SetEncoding(encoding);
}

void Resource::SetEncodedSize(size_t encodedSize)
{
    // BKTODO:
}

void Resource::SetLoader(std::unique_ptr<ResourceLoader> &loader)
{
    ASSERT(!m_loader);
    ASSERT(StillNeedsLoad());
    m_loader = std::move(loader);
}

void Resource::SetResponse(const ResourceResponse &response)
{
    m_response = response;

#if 0 // BKTODO: Check if necessary
    // Currently we support the metadata caching only for HTTP family.
    if (!GetResourceRequest().Url().ProtocolIsInHTTPFamily() ||
        !GetResponse().Url().ProtocolIsInHTTPFamily()) {
        cache_handler_.Clear();
        return;
    }

    cache_handler_ = CreateCachedMetadataHandler(CreateCachedMetadataSender());
#endif
}

bool Resource::ShouldBlockLoadEvent(void) const
{
    return !m_linkPreload && IsLoadEventBlockingResourceType();
}

void Resource::TriggerNotificationForFinishObservers(base::SingleThreadTaskRunner *taskRunner)
{
    if (m_finishObservers.empty())
        return;

    ASSERT(false); // BKTODO:
}

void Resource::WillAddClientOrObserver(void)
{
    if (!HasClientsOrObservers())
        m_isAlive = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NonTextResourceFactory::NonTextResourceFactory(ResourceType type)
    : ResourceFactory(type, TextResourceDecoderOptions::kPlainTextContent)
{
}

std::shared_ptr<Resource> NonTextResourceFactory::Create(const ResourceRequest &request, const ResourceLoaderOptions &options, const TextResourceDecoderOptions &) const
{
    return Create(request, options);
}

}  // namespace blink
