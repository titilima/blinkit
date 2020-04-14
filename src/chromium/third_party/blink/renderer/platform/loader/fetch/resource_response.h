// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_response.h
// Description: ResourceResponse Class
//      Author: Ziming Li
//     Created: 2019-10-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#ifndef BLINKIT_BLINK_RESOURCE_RESPONSE_H
#define BLINKIT_BLINK_RESOURCE_RESPONSE_H

#pragma once

#include "blinkit/common/bk_http_header_map.h"
#include "blinkit/common/bk_url.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {

class ResourceResponse final
{
public:
    enum HTTPVersion : uint8_t {
        kHTTPVersionUnknown,
        kHTTPVersion_0_9,
        kHTTPVersion_1_0,
        kHTTPVersion_1_1,
        kHTTPVersion_2_0
    };

    ResourceResponse(void) = default;
    explicit ResourceResponse(const BlinKit::BkURL &URL);

    bool IsNull(void) const { return m_isNull; }

    const BlinKit::BkURL& Url(void) const { return m_URL; }
    void SetURL(const BlinKit::BkURL &URL);

    const AtomicString& MimeType(void) const { return m_mimeType; }
    void SetMimeType(const AtomicString &mimeType);

    const AtomicString& TextEncodingName(void) const { return m_textEncodingName; }
    void SetTextEncodingName(const AtomicString &encodingName);

    int HttpStatusCode(void) const { return m_httpStatusCode; }
    void SetHTTPStatusCode(int statusCode) { m_httpStatusCode = statusCode; }

    AtomicString HttpContentType(void) const;

    AtomicString HttpHeaderField(const AtomicString &name) const;

    // This method doesn't compare the all members.
    static bool Compare(const ResourceResponse &a, const ResourceResponse &b);
private:
    bool m_isNull = true;
    BlinKit::BkURL m_URL;
    AtomicString m_mimeType, m_textEncodingName;

    int m_httpStatusCode = 0;
    BlinKit::BkHTTPHeaderMap m_httpHeaderFields;
};

inline bool operator==(const ResourceResponse &a, const ResourceResponse &b)
{
    return ResourceResponse::Compare(a, b);
}
inline bool operator!=(const ResourceResponse &a, const ResourceResponse &b)
{
    return !(a == b);
}

}  // namespace blink

#endif  // BLINKIT_BLINK_RESOURCE_RESPONSE_H
