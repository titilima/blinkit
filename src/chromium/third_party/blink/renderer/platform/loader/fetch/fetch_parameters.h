// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: fetch_parameters.h
// Description: FetchParameters Class
//      Author: Ziming Li
//     Created: 2019-11-27
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY GOOGLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_FETCH_PARAMETERS_H
#define BLINKIT_BLINK_FETCH_PARAMETERS_H

#pragma once

#include "third_party/blink/renderer/platform/loader/fetch/resource_loader_options.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/text_resource_decoder_options.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace blink {

struct ResourceLoaderOptions;
class ResourceRequest;

class FetchParameters
{
    DISALLOW_NEW();
public:
    enum DeferOption { kNoDefer, kLazyLoad, kIdleLoad };
    enum class SpeculativePreloadType {
        kNotSpeculative,
        kInDocument,  // The request was discovered in the main document
        kInserted     // The request was discovered in a document.write()
    };
#ifndef BLINKIT_CRAWLER_ONLY
    enum ImageRequestOptimization {
        kNone = 0,          // No optimization.
        kAllowPlaceholder,  // The image is allowed to be a placeholder.
        kDeferImageLoad,  // Defer loading the image from network. Full image might
                          // still load if the request is already-loaded or in
                          // memory cache.
    };
#endif

    FetchParameters(const ResourceRequest &resourceRequest, const ResourceLoaderOptions &options);

    ResourceRequest& MutableResourceRequest(void) { return m_resourceRequest; }
    const ResourceRequest& GetResourceRequest(void) const { return m_resourceRequest; }
    bool ForCrawler(void) const { return m_resourceRequest.ForCrawler(); }

    const GURL& Url(void) const { return m_resourceRequest.Url(); }

    const TextResourceDecoderOptions& DecoderOptions(void) const { return m_decoderOptions; }

    const ResourceLoaderOptions& Options(void) const { return m_options; }
    bool IsLinkPreload(void) const { return m_options.initiator_info.is_link_preload; }
    
    SpeculativePreloadType GetSpeculativePreloadType(void) const { return m_speculativePreloadType; }
    bool IsSpeculativePreload(void) const { return SpeculativePreloadType::kNotSpeculative != m_speculativePreloadType; }

    DeferOption Defer(void) const { return m_defer; }

    bool IsStaleRevalidation(void) const { return m_isStaleRevalidation; }
    void SetStaleRevalidation(bool isStaleRevalidation) { m_isStaleRevalidation = isStaleRevalidation; }

    void OverrideContentType(TextResourceDecoderOptions::ContentType contentType) { m_decoderOptions.OverrideContentType(contentType); }
private:
    ResourceRequest m_resourceRequest;
    // |decoder_options_|'s ContentType is set to |kPlainTextContent| in
    // FetchParameters but is later overridden by ResourceFactory::ContentType()
    // in ResourceFetcher::PrepareRequest() before actual use.
    TextResourceDecoderOptions m_decoderOptions { TextResourceDecoderOptions::kPlainTextContent };
    ResourceLoaderOptions m_options;
    SpeculativePreloadType m_speculativePreloadType = SpeculativePreloadType::kNotSpeculative;
    DeferOption m_defer = kNoDefer;
    bool m_isStaleRevalidation = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_FETCH_PARAMETERS_H
