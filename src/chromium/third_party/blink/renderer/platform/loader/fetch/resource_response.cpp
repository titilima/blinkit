// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_response.cpp
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

#include "resource_response.h"

#include "third_party/blink/renderer/platform/network/http_names.h"
#include "third_party/blink/renderer/platform/network/http_parsers.h"

namespace blink {

ResourceResponse::ResourceResponse(const GURL &URL) : m_isNull(false), m_URL(URL)
{
}

bool ResourceResponse::Compare(const ResourceResponse &a, const ResourceResponse &b)
{
    ASSERT(false); // BKTODO:
    return false;
}

AtomicString ResourceResponse::HttpContentType(void) const
{
    return ExtractMIMETypeFromMediaType(HttpHeaderField(http_names::kContentType).DeprecatedLower());
}

AtomicString ResourceResponse::HttpHeaderField(const AtomicString &name) const
{
    std::string s = m_httpHeaderFields.Get(name.StdUtf8());
    return AtomicString::FromStdUTF8(s);
}

void ResourceResponse::SetMimeType(const AtomicString &mimeType)
{
    m_isNull = false;
    m_mimeType = mimeType;
}

void ResourceResponse::SetTextEncodingName(const AtomicString &encodingName)
{
    m_isNull = false;
    m_textEncodingName = encodingName;
}

void ResourceResponse::SetURL(const GURL &URL)
{
    m_isNull = false;
    m_URL = URL;
}

}  // namespace blink
