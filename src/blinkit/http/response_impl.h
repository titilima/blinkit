// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: response_impl.h
// Description: ResponseImpl Class
//      Author: Ziming Li
//     Created: 2018-09-29
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_RESPONSE_IMPL_H
#define BLINKIT_BLINKIT_RESPONSE_IMPL_H

#pragma once

#include <atomic>
#include "bk_http.h"
#include "blinkit/common/bk_http_header_map.h"

class ResponseImpl final : public std::enable_shared_from_this<ResponseImpl>
{
public:
    ResponseImpl(const std::string &URL);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    int StatusCode(void) const { return m_statusCode; }
    int GetData(int data, BkBuffer *dst) const;
    int GetHeader(const char *name, BkBuffer *dst) const;
    size_t CookiesCount(void) const { return m_cookies.size(); }
    int GetCookie(size_t index, BkBuffer *dst) const;
    void Hijack(const void *newBody, size_t length);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ResetForRedirection(void);

    int ErrorCode(void) const { return m_errorCode; }
    void SetErrorCode(int errorCode) { m_errorCode = errorCode; }

    BlinKit::BkHTTPHeaderMap& MutableHeaders(void) { return m_headers; }
    const BlinKit::BkHTTPHeaderMap& Headers(void) const { return m_headers; }

    const char* BodyData(void) const { return m_body.empty() ? nullptr : reinterpret_cast<const char *>(m_body.data()); }
    int BodyLength(void) const { return m_body.size(); }

    const std::string& CurrentURL(void) const { return m_URL; }
    void SetCurrentURL(const std::string &URL) { m_URL = URL; }

    void SetStatusCode(int statusCode) { m_statusCode = statusCode; }
    void AppendHeader(const char *name, const char *val);

    void ParseHeaders(const std::string &rawHeaders);
    std::string ResolveRedirection(void);
    void PrepareBody(size_t cb) { m_body.reserve(cb); }
    void AppendData(const void *data, size_t cb);
    void GZipInflate(void);
private:
    std::string m_originURL, m_URL;
    int m_errorCode = BK_ERR_SUCCESS, m_statusCode = 0;
    BlinKit::BkHTTPHeaderMap m_headers;
    std::vector<std::string> m_cookies;
    std::vector<unsigned char> m_body;
};

#endif // BLINKIT_BLINKIT_RESPONSE_IMPL_H
