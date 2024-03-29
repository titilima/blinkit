// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ResourceRequest.cpp
// Description: ResourceRequest Class
//      Author: Ziming Li
//     Created: 2021-07-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2003, 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2009, 2012 Google Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
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

#include "./ResourceRequest.h"

#include "blinkit/blink/public/platform/WebURLRequest.h"
#include "blinkit/blink/renderer/platform/HTTPNames.h"
// BKTODO: #include "platform/weborigin/SecurityOrigin.h"

namespace blink {

double ResourceRequest::s_defaultTimeoutInterval = INT_MAX;

bool ResourceRequest::isEmpty() const
{
    return m_url.isEmpty();
}

bool ResourceRequest::isNull() const
{
    return m_url.isEmpty();
}

const KURL& ResourceRequest::url() const
{
    return m_url;
}

void ResourceRequest::setURL(const KURL& url)
{
    m_url = url;
}

void ResourceRequest::removeCredentials()
{
    if (m_url.user().isEmpty() && m_url.pass().isEmpty())
        return;

    ASSERT(false); // BKTODO:
#if 0
    m_url.setUser(String());
    m_url.setPass(String());
#endif
}

ResourceRequestCachePolicy ResourceRequest::cachePolicy() const
{
    return m_cachePolicy;
}

void ResourceRequest::setCachePolicy(ResourceRequestCachePolicy cachePolicy)
{
    m_cachePolicy = cachePolicy;
}

double ResourceRequest::timeoutInterval() const
{
    return m_timeoutInterval;
}

void ResourceRequest::setTimeoutInterval(double timeoutInterval)
{
    m_timeoutInterval = timeoutInterval;
}

const KURL& ResourceRequest::firstPartyForCookies() const
{
    return m_firstPartyForCookies;
}

void ResourceRequest::setFirstPartyForCookies(const KURL& firstPartyForCookies)
{
    m_firstPartyForCookies = firstPartyForCookies;
}

#if 0 // BKTODO:
PassRefPtr<SecurityOrigin> ResourceRequest::requestorOrigin() const
{
    return m_requestorOrigin;
}

void ResourceRequest::setRequestorOrigin(PassRefPtr<SecurityOrigin> requestorOrigin)
{
    m_requestorOrigin = requestorOrigin;
}
#endif

const AtomicString& ResourceRequest::httpMethod() const
{
    return m_httpMethod;
}

void ResourceRequest::setHTTPMethod(const AtomicString& httpMethod)
{
    m_httpMethod = httpMethod;
}

const HTTPHeaderMap& ResourceRequest::httpHeaderFields() const
{
    return m_httpHeaderFields;
}

const AtomicString& ResourceRequest::httpHeaderField(const AtomicString& name) const
{
    return m_httpHeaderFields.get(name);
}

void ResourceRequest::setHTTPHeaderField(const AtomicString& name, const AtomicString& value)
{
    m_httpHeaderFields.set(name, value);
}

#if 0 // BKTODO:
void ResourceRequest::setHTTPReferrer(const Referrer& referrer)
{
    if (referrer.referrer.isEmpty())
        m_httpHeaderFields.remove(HTTPNames::Referer);
    else
        setHTTPHeaderField(HTTPNames::Referer, referrer.referrer);
    m_referrerPolicy = referrer.referrerPolicy;
    m_didSetHTTPReferrer = true;
}
#else
void ResourceRequest::setHTTPReferrer(const AtomicString &referrer)
{
    if (referrer.isEmpty())
        m_httpHeaderFields.remove(HTTPNames::Referer);
    else
        setHTTPHeaderField(HTTPNames::Referer, referrer);
    m_didSetHTTPReferrer = true;
}
#endif

#if 0 // BKTODO:
void ResourceRequest::clearHTTPReferrer()
{
    m_httpHeaderFields.remove(HTTPNames::Referer);
    m_referrerPolicy = ReferrerPolicyDefault;
    m_didSetHTTPReferrer = false;
}

void ResourceRequest::setHTTPOrigin(PassRefPtr<SecurityOrigin> origin)
{
    setHTTPHeaderField(HTTPNames::Origin, origin->toAtomicString());
    if (origin->hasSuborigin())
        setHTTPHeaderField(HTTPNames::Suborigin, AtomicString(origin->suboriginName()));
}

void ResourceRequest::clearHTTPOrigin()
{
    m_httpHeaderFields.remove(HTTPNames::Origin);
    m_httpHeaderFields.remove(HTTPNames::Suborigin);
}

void ResourceRequest::addHTTPOriginIfNeeded(PassRefPtr<SecurityOrigin> origin)
{
    if (!httpOrigin().isEmpty())
        return; // Request already has an Origin header.

    // Don't send an Origin header for GET or HEAD to avoid privacy issues.
    // For example, if an intranet page has a hyperlink to an external web
    // site, we don't want to include the Origin of the request because it
    // will leak the internal host name. Similar privacy concerns have lead
    // to the widespread suppression of the Referer header at the network
    // layer.
    if (httpMethod() == HTTPNames::GET || httpMethod() == HTTPNames::HEAD)
        return;

    // For non-GET and non-HEAD methods, always send an Origin header so the
    // server knows we support this feature.

    AtomicString originString = origin->toAtomicString();
    if (originString.isEmpty()) {
        // If we don't know what origin header to attach, we attach the value
        // for an empty origin.
        setHTTPOrigin(SecurityOrigin::createUnique());
        return;
    }
    setHTTPOrigin(origin);
}
#endif

void ResourceRequest::clearHTTPUserAgent()
{
    m_httpHeaderFields.remove(HTTPNames::User_Agent);
}

EncodedFormData* ResourceRequest::httpBody() const
{
    return m_httpBody.get();
}

void ResourceRequest::setHTTPBody(PassRefPtr<EncodedFormData> httpBody)
{
    m_httpBody = httpBody;
}

bool ResourceRequest::allowStoredCredentials() const
{
    return m_allowStoredCredentials;
}

void ResourceRequest::setAllowStoredCredentials(bool allowCredentials)
{
    m_allowStoredCredentials = allowCredentials;
}

#if 0 // BKTODO:
ResourceLoadPriority ResourceRequest::priority() const
{
    return m_priority;
}

void ResourceRequest::setPriority(ResourceLoadPriority priority, int intraPriorityValue)
{
    m_priority = priority;
    m_intraPriorityValue = intraPriorityValue;
}
#endif

void ResourceRequest::addHTTPHeaderField(const AtomicString& name, const AtomicString& value)
{
    HTTPHeaderMap::AddResult result = m_httpHeaderFields.add(name, value);
    if (!result.isNewEntry)
        result.storedValue->value = result.storedValue->value + ',' + value;
}

void ResourceRequest::addHTTPHeaderFields(const HTTPHeaderMap& headerFields)
{
    HTTPHeaderMap::const_iterator end = headerFields.end();
    for (HTTPHeaderMap::const_iterator it = headerFields.begin(); it != end; ++it)
        addHTTPHeaderField(it->key, it->value);
}

void ResourceRequest::clearHTTPHeaderField(const AtomicString& name)
{
    m_httpHeaderFields.remove(name);
}

bool equalIgnoringHeaderFields(const ResourceRequest& a, const ResourceRequest& b)
{
    if (a.url() != b.url())
        return false;

    if (a.cachePolicy() != b.cachePolicy())
        return false;

    if (a.timeoutInterval() != b.timeoutInterval())
        return false;

    if (a.firstPartyForCookies() != b.firstPartyForCookies())
        return false;

    if (a.httpMethod() != b.httpMethod())
        return false;

    if (a.allowStoredCredentials() != b.allowStoredCredentials())
        return false;

#if 0 // BKTODO:
    if (a.priority() != b.priority())
        return false;

    if (a.referrerPolicy() != b.referrerPolicy())
        return false;
#endif

    EncodedFormData* formDataA = a.httpBody();
    EncodedFormData* formDataB = b.httpBody();

    if (!formDataA)
        return !formDataB;
    if (!formDataB)
        return !formDataA;

    if (*formDataA != *formDataB)
        return false;

    return true;
}

bool ResourceRequest::compare(const ResourceRequest& a, const ResourceRequest& b)
{
    if (!equalIgnoringHeaderFields(a, b))
        return false;

    if (a.httpHeaderFields() != b.httpHeaderFields())
        return false;

    return true;
}

bool ResourceRequest::isConditional() const
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return (m_httpHeaderFields.contains(HTTPNames::If_Match)
        || m_httpHeaderFields.contains(HTTPNames::If_Modified_Since)
        || m_httpHeaderFields.contains(HTTPNames::If_None_Match)
        || m_httpHeaderFields.contains(HTTPNames::If_Range)
        || m_httpHeaderFields.contains(HTTPNames::If_Unmodified_Since));
#endif
}

#if 0 // BKTODO:
void ResourceRequest::setHasUserGesture(bool hasUserGesture)
{
    m_hasUserGesture |= hasUserGesture;
}
#endif

const CacheControlHeader& ResourceRequest::cacheControlHeader() const
{
    ASSERT(false); // BKTODO:
#if 0
    if (!m_cacheControlHeaderCache.parsed)
        m_cacheControlHeaderCache = parseCacheControlDirectives(m_httpHeaderFields.get(HTTPNames::Cache_Control), m_httpHeaderFields.get(HTTPNames::Pragma));
#endif
    return m_cacheControlHeaderCache;
}

bool ResourceRequest::cacheControlContainsNoCache() const
{
    return cacheControlHeader().containsNoCache;
}

bool ResourceRequest::cacheControlContainsNoStore() const
{
    return cacheControlHeader().containsNoStore;
}

bool ResourceRequest::hasCacheValidatorFields() const
{
    ASSERT(false); // BKTODO: return !m_httpHeaderFields.get(HTTPNames::Last_Modified).isEmpty() || !m_httpHeaderFields.get(HTTPNames::ETag).isEmpty();
    return false;
}

void ResourceRequest::initialize(const KURL& url)
{
    m_url = url;
    m_cachePolicy = UseProtocolCachePolicy;
    m_timeoutInterval = s_defaultTimeoutInterval;
    m_httpMethod = HTTPNames::GET;
    m_allowStoredCredentials = true;
    m_reportUploadProgress = false;
    m_reportRawHeaders = false;
    // BKTODO: m_hasUserGesture = false;
    m_downloadToFile = false;
    m_useStreamOnResponse = false;
    m_skipServiceWorker = false;
    m_shouldResetAppCache = false;
    // BKTODO: m_priority = ResourceLoadPriorityLowest;
    m_intraPriorityValue = 0;
    m_requestorID = 0;
    m_requestorProcessID = 0;
    m_appCacheHostID = 0;
#if 0 // BKTODO:
    m_requestContext = WebURLRequest::RequestContextUnspecified;
    m_frameType = WebURLRequest::FrameTypeNone;
    m_fetchRequestMode = WebURLRequest::FetchRequestModeNoCORS;
    // Contrary to the Fetch spec, we default to same-origin mode here, and deal
    // with CORS modes in updateRequestForAccessControl if we're called in a
    // context which requires it.
    m_fetchCredentialsMode = WebURLRequest::FetchCredentialsModeSameOrigin;
    m_fetchRedirectMode = WebURLRequest::FetchRedirectModeFollow;
    m_referrerPolicy = ReferrerPolicyDefault;
    m_loFiState = WebURLRequest::LoFiUnspecified;
#endif
    m_didSetHTTPReferrer = false;
    m_checkForBrowserSideNavigation = true;
    m_uiStartTime = 0;
    m_originatesFromReservedIPRange = false;
    m_inputPerfMetricReportPolicy = InputToLoadPerfMetricReportPolicy::NoReport;
    m_followedRedirect = false;
    // BKTODO: m_requestorOrigin = SecurityOrigin::createUnique();
}

} // namespace blink
