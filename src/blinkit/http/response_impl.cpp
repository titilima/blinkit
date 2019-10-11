// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: response_impl.cpp
// Description: ResponseImpl Class
//      Author: Ziming Li
//     Created: 2018-09-29
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "response_impl.h"

#include <regex>
#include <zlib.h>
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "url/bk_url.h"

using namespace BlinKit;

ResponseImpl::ResponseImpl(const std::string &URL) : m_originURL(URL), m_URL(URL)
{
    // Nothing
}

void ResponseImpl::AppendData(const void *data, size_t cb)
{
    size_t n = m_body.size();
    m_body.resize(n + cb);
    memcpy(m_body.data() + n, data, cb);
}

void ResponseImpl::AppendHeader(const char *name, const char *val)
{
    m_headers.Set(name, val);
}

int ResponseImpl::GetCookie(size_t index, BkBuffer *dst) const
{
    if (m_cookies.size() <= index)
    {
        assert(index < m_cookies.size());
        return BK_ERR_NOT_FOUND;
    }

    const std::string &cookie = m_cookies.at(index);
    BkSetBufferData(dst, cookie.data(), cookie.length());
    return BK_ERR_SUCCESS;
}

int ResponseImpl::GetData(int data, BkBuffer *dst) const
{
    switch (data)
    {
        case BK_RE_CURRENT_URL:
            BkSetBufferData(dst, m_URL.data(), m_URL.length());
            break;
        case BK_RE_ORIGINAL_URL:
            BkSetBufferData(dst, m_originURL.data(), m_originURL.length());
            break;
        case BK_RE_BODY:
            BkSetBufferData(dst, m_body.data(), m_body.size());
            break;
        default:
            assert(false); // Not reached!
            return BK_ERR_NOT_FOUND;
    }
    return BK_ERR_SUCCESS;
}

int ResponseImpl::GetHeader(const char *name, BkBuffer *dst) const
{
    std::string ret = m_headers.Get(name);
    if (ret.empty())
        return BK_ERR_NOT_FOUND;

    BkSetBufferData(dst, ret.data(), ret.length());
    return BK_ERR_SUCCESS;
}

void ResponseImpl::GZipInflate(void)
{
    const size_t BufSize = 4096;
    char buf[BufSize];

    z_stream stm = { 0 };
    stm.next_in = m_body.data();
    stm.avail_in = m_body.size();

    int err = inflateInit2(&stm, MAX_WBITS + 32);
    if (Z_OK != err)
    {
        BKLOG("inflateInit2 failed, code = %d", err);
        assert(Z_OK == err);
        return;
    }

    std::string uncompressedData;
    do {
        stm.next_out = reinterpret_cast<Bytef *>(buf);
        stm.avail_out = BufSize;
        err = inflate(&stm, Z_SYNC_FLUSH);
        if (err < 0)
        {
            BKLOG("inflate failed, code = %d", err);
            assert(err >= 0);
            return;
        }

        uncompressedData.append(buf, BufSize - stm.avail_out);
    } while (stm.avail_in > 0);

    assert(Z_STREAM_END == err);
    m_body.assign(uncompressedData.begin(), uncompressedData.end());
}

void ResponseImpl::ParseHeaders(const std::string &rawHeaders)
{
    std::regex pattern(R"(HTTP\/[\d+\.]+\s+(\d+).*?)");
    std::smatch match;
    if (!std::regex_search(rawHeaders, match, pattern))
    {
        assert(false); // Invalid header!
        return;
    }

    m_statusCode = std::stoi(match.str(1));

    std::string_view input(rawHeaders);
    input = input.substr(match.length(0));

    base::StringPairs headers;
    if (!base::SplitStringIntoKeyValuePairs(input, ':', '\n', &headers))
        return;

    for (const auto &kv : headers)
    {
        std::string k, v;
        base::TrimWhitespaceASCII(kv.first, base::TRIM_ALL, &k);
        base::TrimWhitespaceASCII(kv.second, base::TRIM_ALL, &v);
        if (base::EqualsCaseInsensitiveASCII(k.c_str(), "Set-Cookie"))
            m_cookies.push_back(v);
        else
            m_headers.Set(k, v);
    }
}

void ResponseImpl::ResetForRedirection(void)
{
    m_errorCode = BK_ERR_SUCCESS;
    m_statusCode = 0;
    m_headers.Clear();
    m_cookies.clear();
    m_body.clear();
}

std::string ResponseImpl::ResolveRedirection(void)
{
    std::string ret;

    std::string location = m_headers.Get("Location");
    assert(!location.empty());
    if (!location.empty())
    {
        ret = m_URL;
        m_URL = BkURL(m_URL).Resolve(location).AsString();
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

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
