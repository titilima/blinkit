// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: PreloadRequest.cpp
// Description: PreloadRequest Class
//      Author: Ziming Li
//     Created: 2021-07-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/html/parser/PreloadRequest.h"

#include "core/dom/Document.h"
#include "core/fetch/FetchInitiatorInfo.h"
#include "core/fetch/ResourceFetcher.h"
// BKTODO: #include "platform/CrossOriginAttributeValue.h"

namespace blink {

bool PreloadRequest::isSafeToSendToAnotherThread() const
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return m_initiatorName.isSafeToSendToAnotherThread()
        && m_charset.isSafeToSendToAnotherThread()
        && m_resourceURL.isSafeToSendToAnotherThread()
        && m_baseURL.isSafeToSendToAnotherThread();
#endif
}

KURL PreloadRequest::completeURL(Document* document)
{
    if (!m_baseURL.isEmpty())
        return document->completeURLWithOverride(m_resourceURL, m_baseURL);
    return document->completeURL(m_resourceURL);
}

FetchRequest PreloadRequest::resourceRequest(Document* document)
{
    ASSERT(isMainThread());
    ASSERT(false); // BKTODO:
#if 0 // BKTODO:
    FetchInitiatorInfo initiatorInfo;
    initiatorInfo.name = AtomicString(m_initiatorName);
    initiatorInfo.position = m_initiatorPosition;
#endif
    ResourceRequest resourceRequest(completeURL(document));
    ASSERT(false); // BKTODO: resourceRequest.setHTTPReferrer(SecurityPolicy::generateReferrer(m_referrerPolicy, resourceRequest.url(), document->outgoingReferrer()));
    ResourceFetcher::determineRequestContext(resourceRequest, m_resourceType, false);
    FetchRequest request(resourceRequest);

    if (m_resourceType == Resource::ImportResource) {
        ASSERT(false); // BKTODO:
#if 0
        SecurityOrigin* securityOrigin = document->contextDocument()->securityOrigin();
        request.setCrossOriginAccessControl(securityOrigin, CrossOriginAttributeAnonymous);
#endif
    }
    ASSERT(false); // BKTODO:
#if 0
    if (m_crossOrigin != CrossOriginAttributeNotSet)
        request.setCrossOriginAccessControl(document->securityOrigin(), m_crossOrigin);
    request.setDefer(m_defer);
    request.setResourceWidth(m_resourceWidth);
    request.clientHintsPreferences().updateFrom(m_clientHintsPreferences);
    request.setIntegrityMetadata(m_integrityMetadata);
#endif

    if (m_requestType == RequestTypeLinkRelPreload)
        request.setAvoidBlockingOnLoad(true);
    return request;
}

}
