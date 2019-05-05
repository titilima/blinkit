// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: FrameFetchContext.cpp
// Description: FrameFetchContext Class
//      Author: Ziming Li
//     Created: 2019-02-23
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

#include "core/loader/FrameFetchContext.h"

#include "bindings/core/v8/ScriptController.h"
#include "core/dom/Document.h"
#include "core/fetch/ResourceLoader.h"
#include "core/fetch/UniqueIdentifier.h"
#include "core/frame/FrameConsole.h"
#include "core/frame/FrameHost.h"
#include "core/frame/FrameView.h"
#include "core/frame/LocalFrame.h"
#include "core/frame/Settings.h"
#include "core/inspector/ConsoleMessage.h"
#include "core/loader/DocumentLoader.h"
#include "core/loader/FrameLoader.h"
#include "core/loader/FrameLoaderClient.h"
#include "core/loader/LinkLoader.h"
#include "core/loader/MixedContentChecker.h"
#include "core/loader/NetworkHintsInterface.h"
#include "core/loader/PingLoader.h"
#include "core/page/Page.h"
#include "core/svg/graphics/SVGImageChromeClient.h"
#include "platform/Logging.h"
#include "platform/weborigin/SchemeRegistry.h"
#include "platform/weborigin/SecurityPolicy.h"
#include "public/platform/WebFrameScheduler.h"

#include "blinkit/crawler/crawler_impl.h"

#include <algorithm>

namespace blink {

FrameFetchContext::FrameFetchContext(DocumentLoader* loader)
    : m_document(nullptr)
    , m_documentLoader(loader)
    , m_imageFetched(false)
{
}

FrameFetchContext::~FrameFetchContext()
{
    m_document = nullptr;
    m_documentLoader = nullptr;
}

LocalFrame* FrameFetchContext::frame() const
{
    LocalFrame* frame = nullptr;
    if (m_documentLoader)
        frame = m_documentLoader->frame();
    ASSERT(frame);
    return frame;
}

void FrameFetchContext::addAdditionalRequestHeaders(ResourceRequest& request, FetchResourceType type)
{
    bool isMainResource = type == FetchMainResource;
    if (!isMainResource) {
        RefPtr<SecurityOrigin> outgoingOrigin;
        if (!request.didSetHTTPReferrer()) {
            ASSERT(m_document);
            outgoingOrigin = m_document->securityOrigin();
            request.setHTTPReferrer(SecurityPolicy::generateReferrer(m_document->referrerPolicy(), request.url(), m_document->outgoingReferrer()));
        } else {
            RELEASE_ASSERT(SecurityPolicy::generateReferrer(request.referrerPolicy(), request.url(), request.httpReferrer()).referrer == request.httpReferrer());
            outgoingOrigin = SecurityOrigin::createFromString(request.httpReferrer());
        }

        request.addHTTPOriginIfNeeded(outgoingOrigin);
    }

    if (m_document)
        request.setOriginatesFromReservedIPRange(m_document->isHostedInReservedIPRange());

    // The remaining modifications are only necessary for HTTP and HTTPS.
    if (!request.url().isEmpty() && !request.url().protocolIsInHTTPFamily())
        return;

    frame()->loader().applyUserAgent(request);
}

void FrameFetchContext::setFirstPartyForCookies(ResourceRequest& request)
{
    request.setFirstPartyForCookies(frame()->document()->firstPartyForCookies());
}

CachePolicy FrameFetchContext::cachePolicy() const
{
    if (m_document && m_document->loadEventFinished())
        return CachePolicyVerify;

    FrameLoadType loadType = frame()->loader().loadType();
    if (loadType == FrameLoadTypeReloadFromOrigin)
        return CachePolicyReload;

    if (loadType == FrameLoadTypeReload)
        return CachePolicyRevalidate;

    if (m_documentLoader && m_documentLoader->request().cachePolicy() == ReturnCacheDataElseLoad)
        return CachePolicyHistoryBuffer;
    return CachePolicyVerify;
}

ResourceRequestCachePolicy FrameFetchContext::resourceRequestCachePolicy(const ResourceRequest& request, Resource::Type type) const
{
    ASSERT(frame());
    return UseProtocolCachePolicy;
}

// FIXME(http://crbug.com/274173):
// |loader| can be null if the resource is loaded from imported document.
// This means inspector, which uses DocumentLoader as an grouping entity,
// cannot see imported documents.
inline DocumentLoader* FrameFetchContext::ensureLoaderForNotifications() const
{
    return m_documentLoader ? m_documentLoader.get() : frame()->loader().documentLoader();
}

void FrameFetchContext::dispatchDidChangeResourcePriority(unsigned long identifier, ResourceLoadPriority loadPriority, int intraPriorityValue)
{
    frame()->loader().client()->dispatchDidChangeResourcePriority(identifier, loadPriority, intraPriorityValue);
}

void FrameFetchContext::dispatchWillSendRequest(unsigned long identifier, ResourceRequest& request, const ResourceResponse& redirectResponse, const FetchInitiatorInfo& initiatorInfo)
{
    frame()->loader().applyUserAgent(request);
    frame()->loader().client()->dispatchWillSendRequest(m_documentLoader, identifier, request, redirectResponse);
}

void FrameFetchContext::dispatchDidReceiveResponse(unsigned long identifier, const ResourceResponse& response, ResourceLoader* resourceLoader)
{
    frame()->loader().client()->dispatchDidReceiveResponse(m_documentLoader, identifier, response);
    DocumentLoader* documentLoader = ensureLoaderForNotifications();
    // It is essential that inspector gets resource response BEFORE console.
    frame()->console().reportResourceResponseReceived(documentLoader, identifier, response);
}

void FrameFetchContext::dispatchDidReceiveData(unsigned long identifier, const char* data, int dataLength, int encodedDataLength)
{
    // Currently nothing do do.
}

void FrameFetchContext::dispatchDidDownloadData(unsigned long identifier, int dataLength, int encodedDataLength)
{
    // Currently nothing do do.
}

void FrameFetchContext::dispatchDidFinishLoading(unsigned long identifier, double finishTime, int64_t encodedDataLength)
{
    frame()->loader().client()->dispatchDidFinishLoading(m_documentLoader, identifier);
}

void FrameFetchContext::dispatchDidFail(unsigned long identifier, const ResourceError& error, bool isInternalRequest)
{
    frame()->loader().client()->dispatchDidFinishLoading(m_documentLoader, identifier);
    if (!isInternalRequest)
        frame()->console().didFailLoading(identifier, error);
}


void FrameFetchContext::dispatchDidLoadResourceFromMemoryCache(const Resource* resource)
{
    ResourceRequest request(resource->url());
    unsigned long identifier = createUniqueIdentifier();
    frame()->loader().client()->dispatchDidLoadResourceFromMemoryCache(request, resource->response());
    dispatchWillSendRequest(identifier, request, ResourceResponse(), resource->options().initiatorInfo);

    if (!resource->response().isNull())
        dispatchDidReceiveResponse(identifier, resource->response());

    if (resource->encodedSize() > 0)
        dispatchDidReceiveData(identifier, 0, resource->encodedSize(), 0);

    dispatchDidFinishLoading(identifier, 0, 0);
}

bool FrameFetchContext::shouldLoadNewResource(Resource::Type type) const
{
    if (!m_documentLoader)
        return true;
    if (type == Resource::MainResource)
        return m_documentLoader == frame()->loader().provisionalDocumentLoader();
    return m_documentLoader == frame()->loader().documentLoader();
}

void FrameFetchContext::willStartLoadingResource(ResourceRequest& request)
{
    // Nothing to do, application cache is disabled in BlinKit.
}

void FrameFetchContext::didLoadResource(Resource* resource)
{
    if (resource->isLoadEventBlockingResourceType())
        frame()->loader().checkCompleted();
}

bool FrameFetchContext::allowImage(bool imagesEnabled, const KURL& url) const
{
    return frame()->loader().client()->allowImage(imagesEnabled, url);
}

void FrameFetchContext::printAccessDeniedMessage(const KURL& url) const
{
    if (url.isNull())
        return;

    String message;
    if (!m_document || m_document->url().isNull())
        message = "Unsafe attempt to load URL " + url.elidedString() + '.';
    else if (url.isLocalFile() || m_document->url().isLocalFile())
        message = "Unsafe attempt to load URL " + url.elidedString() + " from frame with URL " + m_document->url().elidedString() + ". 'file:' URLs are treated as unique security origins.\n";
    else
        message = "Unsafe attempt to load URL " + url.elidedString() + " from frame with URL " + m_document->url().elidedString() + ". Domains, protocols and ports must match.\n";

    frame()->document()->addConsoleMessage(ConsoleMessage::create(SecurityMessageSource, ErrorMessageLevel, message));
}

bool FrameFetchContext::canRequest(Resource::Type type, const ResourceRequest& resourceRequest, const KURL& url, const ResourceLoaderOptions& options, bool forPreload, FetchRequest::OriginRestriction originRestriction) const
{
    return true;
}

bool FrameFetchContext::allowResponse(Resource::Type type, const ResourceRequest& resourceRequest, const KURL& url, const ResourceLoaderOptions& options) const
{
    return true;
}

bool FrameFetchContext::isControlledByServiceWorker() const
{
    ASSERT(m_documentLoader || frame()->loader().documentLoader());
    if (m_documentLoader)
        return frame()->loader().client()->isControlledByServiceWorker(*m_documentLoader);
    // m_documentLoader is null while loading resources from an HTML import.
    // In such cases whether the request is controlled by ServiceWorker or not
    // is determined by the document loader of the frame.
    return frame()->loader().client()->isControlledByServiceWorker(*frame()->loader().documentLoader());
}

int64_t FrameFetchContext::serviceWorkerID() const
{
    ASSERT(m_documentLoader || frame()->loader().documentLoader());
    if (m_documentLoader)
        return frame()->loader().client()->serviceWorkerID(*m_documentLoader);
    // m_documentLoader is null while loading resources from an HTML import.
    // In such cases a service worker ID could be retrieved from the document
    // loader of the frame.
    return frame()->loader().client()->serviceWorkerID(*frame()->loader().documentLoader());
}

bool FrameFetchContext::isMainFrame() const
{
    return frame()->isMainFrame();
}

bool FrameFetchContext::defersLoading() const
{
#ifdef BLINKIT_CRAWLER_ONLY
    return false;
#else
    return !frame()->IsCrawlerFrame() && frame()->page()->defersLoading();
#endif
}

bool FrameFetchContext::isLoadComplete() const
{
    return m_document && m_document->loadEventFinished();
}

bool FrameFetchContext::pageDismissalEventBeingDispatched() const
{
    return m_document && m_document->pageDismissalEventBeingDispatched() != Document::NoDismissal;
}

#ifndef BLINKIT_CRAWLER_ONLY
void FrameFetchContext::sendImagePing(const KURL& url)
{
    ASSERT(!frame()->IsCrawlerFrame());
    PingLoader::loadImage(frame(), url);
}
#endif

void FrameFetchContext::addConsoleMessage(const String& message) const
{
    if (frame()->document())
        frame()->document()->addConsoleMessage(ConsoleMessage::create(JSMessageSource, ErrorMessageLevel, message));
}

SecurityOrigin* FrameFetchContext::securityOrigin() const
{
    return m_document ? m_document->securityOrigin() : nullptr;
}

void FrameFetchContext::countClientHintsDPR()
{
    UseCounter::count(frame(), UseCounter::ClientHintsDPR);
}

void FrameFetchContext::countClientHintsResourceWidth()
{
    UseCounter::count(frame(), UseCounter::ClientHintsResourceWidth);
}

void FrameFetchContext::countClientHintsViewportWidth()
{
    UseCounter::count(frame(), UseCounter::ClientHintsViewportWidth);
}

ResourceLoadPriority FrameFetchContext::modifyPriorityForExperiments(ResourceLoadPriority priority, Resource::Type type, const FetchRequest& request, ResourcePriority::VisibilityStatus visibility)
{
    // An image fetch is used to distinguish between "early" and "late" scripts in a document
    if (type == Resource::Image)
        m_imageFetched = true;

    // If Settings is null, we can't verify any experiments are in force.
    if (!frame()->settings())
        return priority;

    // If enabled, drop the priority of all resources in a subframe.
    if (frame()->settings()->lowPriorityIframes() && !frame()->isMainFrame())
        return ResourceLoadPriorityVeryLow;

    // Async/Defer scripts.
    if (type == Resource::Script && FetchRequest::LazyLoad == request.defer())
        return frame()->settings()->fetchIncreaseAsyncScriptPriority() ? ResourceLoadPriorityMedium : ResourceLoadPriorityLow;

    // Runtime experiment that change how we prioritize resources.
    // The toggles do not depend on each other and can be flipped individually
    // though the cumulative result will depend on the interaction between them.
    // Background doc: https://docs.google.com/document/d/1bCDuq9H1ih9iNjgzyAL0gpwNFiEP4TZS-YLRp_RuMlc/edit?usp=sharing

    // Increases the priorities for CSS, Scripts, Fonts and Images all by one level
    // and parser-blocking scripts and visible images by 2.
    // This is used in conjunction with logic on the Chrome side to raise the threshold
    // of "layout-blocking" resources and provide a boost to resources that are needed
    // as soon as possible for something currently on the screen.
    int modifiedPriority = static_cast<int>(priority);
    if (frame()->settings()->fetchIncreasePriorities()) {
        if (type == Resource::CSSStyleSheet || type == Resource::Script || type == Resource::Font || type == Resource::Image)
            modifiedPriority++;
    }

    // Always give visible resources a bump, and an additional bump if generally increasing priorities.
    if (visibility == ResourcePriority::Visible) {
        modifiedPriority++;
        if (frame()->settings()->fetchIncreasePriorities())
            modifiedPriority++;
    }

    if (frame()->settings()->fetchIncreaseFontPriority() && type == Resource::Font)
        modifiedPriority++;

    if (type == Resource::Script) {
        // Reduce the priority of late-body scripts.
        if (frame()->settings()->fetchDeferLateScripts() && request.forPreload() && m_imageFetched)
            modifiedPriority--;
        // Parser-blocking scripts.
        if (frame()->settings()->fetchIncreasePriorities() && !request.forPreload())
            modifiedPriority++;
    }

    // Clamp priority
    modifiedPriority = std::min(static_cast<int>(ResourceLoadPriorityHighest), std::max(static_cast<int>(ResourceLoadPriorityLowest), modifiedPriority));
    return static_cast<ResourceLoadPriority>(modifiedPriority);
}

WebTaskRunner* FrameFetchContext::loadingTaskRunner() const
{
    return frame()->frameScheduler()->loadingTaskRunner();
}

BlinKit::CrawlerImpl* FrameFetchContext::FrameCrawler(void)
{
    FrameClient *client = frame()->client();
#ifndef BLINKIT_CRAWLER_ONLY
    if (!client->IsCrawler())
        return nullptr;
#endif
    return BlinKit::toCrawlerImpl(client);
}

DEFINE_TRACE(FrameFetchContext)
{
    visitor->trace(m_document);
    visitor->trace(m_documentLoader);
    FetchContext::trace(visitor);
}

} // namespace blink
