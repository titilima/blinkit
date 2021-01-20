// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_request.h
// Description: ResourceRequest Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2003, 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2006 Samuel Weinig <sam.weinig@gmail.com>
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

#ifndef BLINKIT_BLINK_RESOURCE_REQUEST_H
#define BLINKIT_BLINK_RESOURCE_REQUEST_H

#pragma once

#include "bk_crawler.h"
#include "bkcommon/bk_http_header_map.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_load_priority.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"
#include "url/gurl.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "bk_ui.h"
#endif

namespace blink {

class EncodedFormData;

enum class HijackType {
    kNotForCrawler = 0,
    kMainHTML,
    kScript,
    kOther
};

class ResourceRequest final
{
public:
    enum class RedirectStatus : uint8_t { kFollowedRedirect, kNoRedirect };

    explicit ResourceRequest(const String &urlString);
    explicit ResourceRequest(const GURL &URL);

    BkCrawler Crawler(void) const { return m_crawler; }
    bool ForCrawler(void) const { return nullptr != m_crawler; }
    void SetCrawler(BkCrawler crawler) { m_crawler = crawler; }

#ifndef BLINKIT_CRAWLER_ONLY
    BkWebView View(void) const { return m_view; }
    void SetView(BkWebView view) { m_view = view; }
#endif

    const GURL& Url(void) const { return m_URL; }
    void SetURL(const GURL &URL) { m_URL = URL; }

    void SetHTTPReferrer(const String &referrer);
    bool DidSetHTTPReferrer(void) const { return m_didSetHttpReferrer; }

    const GURL& SiteForCookies(void) const { return m_siteForCookies; }
    void SetSiteForCookies(const GURL &siteForCookies) { m_siteForCookies = siteForCookies; }

    const AtomicString& HttpMethod(void) const { return m_httpMethod; }

    const BlinKit::BkHTTPHeaderMap& AllHeaders(void) const { return m_headers; }
    void SetHTTPUserAgent(const String &httpUserAgent);

    EncodedFormData* HttpBody(void) const;

    bool DownloadToBlob(void) const { return m_downloadToBlob; }
    bool GetKeepalive(void) const { return m_keepalive; }

    ResourceLoadPriority Priority(void) const { return m_priority; }
    void SetPriority(ResourceLoadPriority priority, int intraPriorityValue = 0)
    {
        m_priority = priority;
        m_intraPriorityValue = intraPriorityValue;
    }

    bool WasDiscarded(void) const { return m_wasDiscarded; }
    void SetWasDiscarded(bool wasDiscarded) { m_wasDiscarded = wasDiscarded; }

    HijackType GetHijackType(void) const { return m_hijackType; }
    void SetHijackType(HijackType hijackType) { m_hijackType = hijackType; }

    RedirectStatus GetRedirectStatus(void) const { return m_redirectStatus; }
private:
    BkCrawler m_crawler = nullptr;
#ifndef BLINKIT_CRAWLER_ONLY
    BkWebView m_view = nullptr;
#endif
    GURL m_URL, m_siteForCookies;
    AtomicString m_httpMethod;
    BlinKit::BkHTTPHeaderMap m_headers;
    bool m_downloadToBlob : 1;
    bool m_keepalive : 1;
    ResourceLoadPriority m_priority = ResourceLoadPriority::kLowest;
    int m_intraPriorityValue = 0;
    bool m_didSetHttpReferrer = false;
    bool m_wasDiscarded = false;
    RedirectStatus m_redirectStatus = RedirectStatus::kNoRedirect;
    HijackType m_hijackType = HijackType::kNotForCrawler;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_RESOURCE_REQUEST_H
