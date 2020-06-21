// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: hijack_response.cpp
// Description: Response Classes for Hijacking
//      Author: Ziming Li
//     Created: 2020-06-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "hijack_response.h"

#include "base/strings/string_util.h"
#include "bkcommon/bk_strings.h"

namespace BlinKit {

FakeResponse::FakeResponse(const std::string &URL) : ResponseBase(URL)
{
    m_statusCode = 200;
}

int FakeResponse::EnumerateHeaders(BkHttpHeaderEnumerator enumerator, void *userData) const
{
    NOTREACHED();
    return BK_ERR_FORBIDDEN;
}

int FakeResponse::GetCookie(size_t index, BkBuffer *dst) const
{
    NOTREACHED();
    return BK_ERR_FORBIDDEN;
}

int FakeResponse::GetHeader(const char *name, BkBuffer *dst) const
{
    if (base::EqualsCaseInsensitiveASCII(name, Strings::HttpHeader::ContentType))
        BkSetBufferData(dst, m_contentType.data(), m_contentType.length());
    else
        return BK_ERR_NOT_FOUND;
    return BK_ERR_SUCCESS;
}

} // namespace BlinKit
