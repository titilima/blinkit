// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: http_response.cpp
// Description: HttpResponse Class
//      Author: Ziming Li
//     Created: 2018-09-29
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "http_response.h"

#include <regex>
#include <zlib.h>
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "bkcommon/bk_strings.h"
#include "url/gurl.h"

namespace BlinKit {

HttpResponse::HttpResponse(const std::string &URL) : ResponseBase(URL), m_currentURL(URL)
{
    // Nothing
}

void HttpResponse::AppendData(const void *data, size_t cb)
{
    size_t n = m_body.size();
    m_body.resize(n + cb);
    memcpy(m_body.data() + n, data, cb);
}

int HttpResponse::EnumerateHeaders(BkHttpHeaderEnumerator enumerator, void *userData) const
{
    const auto &rawHeaders = m_headers.GetRawMap();
    if (rawHeaders.empty())
        return BK_ERR_NOT_FOUND;

    for (const auto &it : rawHeaders)
    {
        if (!enumerator(it.first.c_str(), it.second.c_str(), userData))
            return BK_ERR_CANCELLED;
    }
    return BK_ERR_SUCCESS;
}

int HttpResponse::GetCookie(size_t index, BkBuffer *dst) const
{
    if (m_cookies.size() <= index)
    {
        ASSERT(index < m_cookies.size());
        return BK_ERR_NOT_FOUND;
    }

    const std::string &cookie = m_cookies.at(index);
    BkSetBufferData(dst, cookie.data(), cookie.length());
    return BK_ERR_SUCCESS;
}

int HttpResponse::GetData(int data, BkBuffer *dst) const
{
    switch (data)
    {
        case BK_RESPONSE_CURRENT_URL:
            BkSetBufferData(dst, m_currentURL.data(), m_currentURL.length());
            break;
        case BK_RESPONSE_HTTP_VERSION:
            if (m_httpVersion.empty())
                return BK_ERR_NOT_FOUND;
            BkSetBufferData(dst, m_httpVersion.data(), m_httpVersion.length());
            break;
        case BK_RESPONSE_STATUS_TEXT:
            if (m_statusText.empty())
                return BK_ERR_NOT_FOUND;
            BkSetBufferData(dst, m_statusText.data(), m_statusText.length());
            break;
        default:
            return ResponseBase::GetData(data, dst);
    }
    return BK_ERR_SUCCESS;
}

int HttpResponse::GetHeader(const char *name, BkBuffer *dst) const
{
    std::string s(name);
    if (!m_headers.Has(s))
        return BK_ERR_NOT_FOUND;

    std::string ret = m_headers.Get(name);
    BkSetBufferData(dst, ret.data(), ret.length());
    return BK_ERR_SUCCESS;
}

bool HttpResponse::GZipInflate(void)
{
    const size_t BufSize = 4096;
    char buf[BufSize];

    z_stream stm = { 0 };
    stm.next_in = m_body.data();
    stm.avail_in = static_cast<uInt>(m_body.size());

    int err = inflateInit2(&stm, MAX_WBITS + 32);
    if (Z_OK != err)
    {
        BKLOG("inflateInit2 failed, code = %d", err);
        ASSERT(Z_OK == err);
        return false;
    }

    std::string uncompressedData;
    do {
        stm.next_out = reinterpret_cast<Bytef *>(buf);
        stm.avail_out = BufSize;
        err = inflate(&stm, Z_SYNC_FLUSH);
        if (err < 0)
        {
            BKLOG("inflate failed, code = %d", err);
            ASSERT(err >= 0);
            return false;
        }

        uncompressedData.append(buf, BufSize - stm.avail_out);
    } while (stm.avail_in > 0);

    ASSERT(Z_STREAM_END == err);
    m_body.assign(uncompressedData.begin(), uncompressedData.end());
    return true;
}

CURLcode HttpResponse::InflateBodyIfNecessary(CURLcode code)
{
    if (m_body.empty())
        return code;

    std::string contentEncoding = m_headers.Get(Strings::HttpHeader::ContentEncoding);
    if (contentEncoding.empty())
        return code;

    if (!base::EqualsCaseInsensitiveASCII(contentEncoding, "gzip"))
    {
        NOTREACHED(); // Unexpected content encoding!
        return CURLE_BAD_CONTENT_ENCODING;
    }

    // In some cases, CURL may return `CURLE_RECV_ERROR` with a correctly-deflated body.
    // So the returning code SHOULD BE verified by inflating the body data.
    return GZipInflate() ? CURLE_OK : CURLE_RECV_ERROR;
}

void HttpResponse::ParseHeaders(const std::string &rawHeaders)
{
    std::regex pattern(R"(HTTP\/([\d+\.]+)\s+(\d+)\s+(.*))");
    std::smatch match;
    if (!std::regex_search(rawHeaders, match, pattern))
    {
        ASSERT(false); // Invalid header!
        return;
    }

    m_httpVersion = match.str(1);
    m_statusCode = std::stoi(match.str(2));
    m_statusText = match.str(3);

    std::string_view input(rawHeaders);
    size_t p = input.find('\n', match.length(0));
    if (std::string_view::npos == p)
        return;

    input = input.substr(p + 1);

    base::StringPairs headers;
    std::vector<std::string> lines = base::SplitString(input, "\n", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
    for (const std::string &line : lines)
    {
        p = line.find(':');
        if (std::string::npos == p)
        {
            ASSERT(std::string::npos != p); // Invalid line format!
            continue;
        }

        std::string k, v;
        k = line.substr(0, p);
        base::TrimWhitespaceASCII(line.substr(p + 1), base::TRIM_LEADING, &v);
        if (base::EqualsCaseInsensitiveASCII(k.c_str(), "Set-Cookie"))
            m_cookies.push_back(v);
        else
            m_headers.Set(k, v);
    }
}

void HttpResponse::Reset(void)
{
    m_errorCode = BK_ERR_SUCCESS;
    m_statusCode = 0;
    m_httpVersion.clear();
    m_statusText.clear();
    m_headers.Clear();
    m_cookies.clear();
    m_body.clear();
}

}; // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkEnumerateResponseHeaders(BkResponse response, BkHttpHeaderEnumerator enumerator, void *userData)
{
    return response->EnumerateHeaders(enumerator, userData);
}

BKEXPORT int BKAPI BkGetResponseCookie(BkResponse response, size_t index, BkBuffer *dst)
{
    return response->GetCookie(index, dst);
}

BKEXPORT size_t BKAPI BkGetResponseCookiesCount(BkResponse response)
{
    return response->CookiesCount();
}

BKEXPORT int BKAPI BkGetResponseData(BkResponse response, int data, BkBuffer *dst)
{
    return response->GetData(data, dst);
}

BKEXPORT int BKAPI BkGetResponseHeader(BkResponse response, const char *name, BkBuffer *dst)
{
    return response->GetHeader(name, dst);
}

BKEXPORT int BKAPI BkGetResponseStatusCode(BkResponse response)
{
    return response->StatusCode();
}

} // extern "C"
