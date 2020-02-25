// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_request.cpp
// Description: ResourceRequest Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
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

#include "resource_request.h"

#include "third_party/blink/renderer/platform/network/http_names.h"

using namespace BlinKit;

namespace blink {

ResourceRequest::ResourceRequest(const BkURL &URL)
    : m_URL(URL)
    , m_httpMethod(http_names::kGET)
    , m_downloadToBlob(false)
    , m_keepalive(false)
{
}

EncodedFormData* ResourceRequest::HttpBody(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

void ResourceRequest::SetHTTPReferrer(const String &referrer)
{
    const std::string name = http_names::kReferer.StdUtf8();
    if (referrer.IsEmpty())
        m_headers.Remove(name);
    else
        m_headers.Set(name, referrer.StdUtf8());
    m_didSetHttpReferrer = true;
}

void ResourceRequest::SetHTTPUserAgent(const String &httpUserAgent)
{
    std::string name = http_names::kUserAgent.StdUtf8();
    m_headers.Set(name, httpUserAgent.StdUtf8());
}

}  // namespace blink
