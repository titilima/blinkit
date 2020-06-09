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

template <class T>
static void SetBufferData(BkBuffer *dst, const T &data)
{
    ASSERT(nullptr != dst->Allocator);
    if (!data.empty())
    {
        void *buf = dst->Allocator(data.size(), dst->UserData);
        memcpy(buf, data.data(), data.size());
    }
}

static void SetBufferString(BkBuffer *dst, const std::string &s)
{
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
    m_headers.Set(name, val);
}

int ResponseImpl::GetCookie(size_t index, BkBuffer *dst) const
{
    if (m_cookies.size() <= index)
    {
        ASSERT(index < m_cookies.size());
        return BK_ERR_NOT_FOUND;
    }

    SetBufferData(dst, m_cookies.at(index));
    return BK_ERR_SUCCESS;
}

int ResponseImpl::GetData(int data, BkBuffer *dst) const
{
    switch (data)
    {
        case BK_RE_CURRENT_URL:
            SetBufferData(dst, m_URL);
            break;
        case BK_RE_ORIGINAL_URL:
            SetBufferData(dst, m_originURL);
            break;
        case BK_RE_BODY:
            SetBufferData(dst, m_body);
            break;
        default:
            NOTREACHED();
            return BK_ERR_NOT_FOUND;
    }
    return BK_ERR_SUCCESS;
}

int ResponseImpl::GetHeader(const char *name, BkBuffer *dst) const
{
    std::string ret = m_headers.Get(name);
    if (ret.empty())
        return BK_ERR_NOT_FOUND;
    SetBufferData(dst, ret);
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
        ASSERT(Z_OK == err);
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
            ASSERT(err >= 0);
            return;
        }

        uncompressedData.append(buf, BufSize - stm.avail_out);
    } while (stm.avail_in > 0);

    ASSERT(Z_STREAM_END == err);
    m_body.assign(uncompressedData.begin(), uncompressedData.end());
}

void ResponseImpl::Hijack(const void *newBody, size_t length)
{
    m_body.resize(length);
    if (nullptr != newBody)
        memcpy(m_body.data(), newBody, length);
    else
        ASSERT(0 == length);
}

void ResponseImpl::ParseHeaders(const std::string &rawHeaders)
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
    m_reason = match.str(3);

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
    ASSERT(!location.empty());
    if (!location.empty())
    {
        ret = m_URL;
        m_URL = GURL(m_URL).Resolve(location).spec();
    }
    return ret;
}

void ResponseImpl::SetBody(const void *data, size_t length)
{
    m_body.resize(length);
    if (length > 0)
        memcpy(m_body.data(), data, length);
}
