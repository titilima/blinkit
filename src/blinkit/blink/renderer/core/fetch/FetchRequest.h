// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: FetchRequest.h
// Description: FetchRequest Class
//      Author: Ziming Li
//     Created: 2021-07-15
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
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

#ifndef FetchRequest_h
#define FetchRequest_h

#include "core/CoreExport.h"
// BKTODO: #include "core/fetch/ClientHintsPreferences.h"
#include "core/fetch/FetchInitiatorInfo.h"
// BKTODO: #include "core/fetch/IntegrityMetadata.h"
#include "core/fetch/ResourceLoaderOptions.h"
// BKTODO: #include "platform/CrossOriginAttributeValue.h"
// BKTODO: #include "platform/network/ResourceLoadPriority.h"
#include "platform/network/ResourceRequest.h"
#include "wtf/Allocator.h"
#include "wtf/text/AtomicString.h"

namespace blink {
class SecurityOrigin;

class CORE_EXPORT FetchRequest {
    STACK_ALLOCATED();
public:
    enum DeferOption { NoDefer, LazyLoad, DeferredByClient };
    enum OriginRestriction { UseDefaultOriginRestrictionForType, RestrictToSameOrigin, NoOriginRestriction };

    struct ResourceWidth {
        DISALLOW_NEW();
        float width;
        bool isSet;

        ResourceWidth()
            : width(0)
            , isSet(false)
        {
        }
    };

    explicit FetchRequest(const ResourceRequest&, const AtomicString& initiator, const String& charset = String()); // BKTODO: , ResourceLoadPriority = ResourceLoadPriorityUnresolved);
    FetchRequest(const ResourceRequest&, const ResourceLoaderOptions&);
    FetchRequest(const ResourceRequest&, const FetchInitiatorInfo&);
    ~FetchRequest();

    ResourceRequest& mutableResourceRequest() { return m_resourceRequest; }
    const ResourceRequest& resourceRequest() const { return m_resourceRequest; }
    const KURL& url() const { return m_resourceRequest.url(); }

    const String& charset() const { return m_charset; }
    void setCharset(const String& charset) { m_charset = charset; }

    const ResourceLoaderOptions& options() const { return m_options; }
    void setOptions(const ResourceLoaderOptions& options) { m_options = options; }

#if 0 // BKTODO:
    ResourceLoadPriority priority() const { return m_priority; }
    void setPriority(ResourceLoadPriority priority) { m_priority = priority; }
#endif

    DeferOption defer() const { return m_defer; }
    void setDefer(DeferOption defer) { m_defer = defer; }

    ResourceWidth resourceWidth() const { return m_resourceWidth; }
    void setResourceWidth(ResourceWidth);

    // BKTODO: ClientHintsPreferences& clientHintsPreferences() { return m_clientHintPreferences; }

    bool forPreload() const { return m_forPreload; }
    void setForPreload(bool forPreload) { m_forPreload = forPreload; }

    bool avoidBlockingOnLoad() { return m_avoidBlockingOnLoad; }
    void setAvoidBlockingOnLoad(bool doNotBlock) { m_avoidBlockingOnLoad = doNotBlock; }

#if 0 // BKTODO:
    void setContentSecurityCheck(ContentSecurityPolicyDisposition contentSecurityPolicyOption) { m_options.contentSecurityPolicyOption = contentSecurityPolicyOption; }
    void setCrossOriginAccessControl(SecurityOrigin*, CrossOriginAttributeValue);
#endif
    OriginRestriction originRestriction() const { return m_originRestriction; }
    void setOriginRestriction(OriginRestriction restriction) { m_originRestriction = restriction; }
#if 0 // BKTODO:
    const IntegrityMetadataSet& integrityMetadata() const { return m_integrityMetadata; }
    void setIntegrityMetadata(const IntegrityMetadataSet& metadata) { m_integrityMetadata = metadata; }
#endif

private:
    ResourceRequest m_resourceRequest;
    String m_charset;
    ResourceLoaderOptions m_options;
    // BKTODO: ResourceLoadPriority m_priority;
    bool m_forPreload;
    bool m_avoidBlockingOnLoad;
    DeferOption m_defer;
    OriginRestriction m_originRestriction;
    ResourceWidth m_resourceWidth;
#if 0 // BKTODO:
    ClientHintsPreferences m_clientHintPreferences;
    IntegrityMetadataSet m_integrityMetadata;
#endif
};

} // namespace blink

#endif
