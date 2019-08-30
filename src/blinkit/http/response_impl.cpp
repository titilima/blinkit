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
#include "url/gurl.h"

namespace BlinKit {

static void CanonizeHeaderName(std::string &headerName)
{
    static const char* specialNames[] = { "ETag" };
    for (const char *specialName : specialNames)
    {
        if (base::EqualsCaseInsensitiveASCII(headerName, specialName))
        {
            headerName = specialName;
            return;
        }
    }

    bool upperNext = true;
    for (size_t i = 0; i < headerName.length(); ++i)
    {
        char ch = headerName.at(i);

        if (upperNext)
        {
            headerName.at(i) = base::ToUpperASCII(ch);
            upperNext = false;
        }
        else
        {
            headerName.at(i) = base::ToLowerASCII(ch);
        }

        if ('-' == ch)
            upperNext = true;
    }
}

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
    m_headers[name] = val;
}

int BKAPI ResponseImpl::GetBody(BkBuffer &body) const
{
    if (m_body.empty())
        return BkError::NotFound;

    body.Assign(m_body.data(), m_body.size());
    return BkError::Success;
}

int BKAPI ResponseImpl::GetCookie(unsigned i, BkBuffer &cookie) const
{
    if (m_cookies.size() <= i)
    {
        assert(i < m_cookies.size());
        return BkError::NotFound;
    }

    cookie.Assign(m_cookies.at(i));
    return BkError::Success;
}

int BKAPI ResponseImpl::GetCurrentURL(BkBuffer &URL) const
{
    URL.Assign(m_URL);
    return BkError::Success;
}

int BKAPI ResponseImpl::GetHeader(const char *name, BkBuffer &value) const
{
    std::string s(name);
    CanonizeHeaderName(s);

    auto it = m_headers.find(s);
    if (std::end(m_headers) == it)
        return BkError::NotFound;

    value.Assign(it->second);
    return BkError::Success;
}

int ResponseImpl::GetInformation(Information i, BkBuffer &value) const
{
    switch (i)
    {
        case BkResponse::Information::OriginalURL:
            value.Assign(m_originURL);
            break;
        case BkResponse::Information::HTTPVersion:
            value.Assign(m_version);
            break;
        case BkResponse::Information::ReasonPhrase:
            value.Assign(m_reasonPhrase);
            break;
        default:
            assert(false); // Not reached!
            return BkError::NotFound;
    }
    return BkError::Success;
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
    std::regex pattern(R"(HTTP/(\d+\.\d+)\s+(\d+)\s+(.+))");
    std::smatch match;
    if (!std::regex_search(rawHeaders, match, pattern))
    {
        assert(false); // Invalid header!
        return;
    }

    m_version = match.str(1);
    m_statusCode = std::stoi(match.str(2));
    m_reasonPhrase = match.str(3);

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
        {
            m_cookies.push_back(v);
        }
        else
        {
            CanonizeHeaderName(k);
            m_headers[k] = v;
        }
    }
}

void BKAPI ResponseImpl::Release(void)
{
    if (0 == --m_refCount)
        delete this;
}

void ResponseImpl::ResetForRedirection(void)
{
    m_version.clear();
    m_errorCode = BkError::Success;
    m_statusCode = 0;
    m_reasonPhrase.clear();
    m_headers.clear();
    m_cookies.clear();
    m_body.clear();
}

std::string ResponseImpl::ResolveRedirection(void)
{
    std::string ret;

    auto it = m_headers.find("Location");
    assert(std::end(m_headers) != it);
    if (std::end(m_headers) != it)
    {
        ret = m_URL;
        m_URL = GURL(m_URL).Resolve(it->second).spec();
    }
    return ret;
}

BkRetainedResponse* BKAPI ResponseImpl::Retain(void) const
{
    ResponseImpl *r = const_cast<ResponseImpl *>(this);
    ++r->m_refCount;
    return r;
}

} // namespace BlinKit
