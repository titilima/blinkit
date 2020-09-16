// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: http_response.h
// Description: HttpResponse Class
//      Author: Ziming Li
//     Created: 2018-09-29
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKBASE_HTTP_RESPONSE_H
#define BLINKIT_BKBASE_HTTP_RESPONSE_H

#pragma once

#include <curl/curl.h>
#include "bkcommon/bk_http_header_map.h"
#include "bkcommon/response_impl.h"

namespace BlinKit {

class HttpResponse final : public ResponseBase
{
public:
    HttpResponse(const std::string &URL);

    void Reset(void);
    void ParseHeaders(const std::string &rawHeaders);

    int ErrorCode(void) const { return m_errorCode; }
    void SetErrorCode(int errorCode) { m_errorCode = errorCode; }

    const std::string& CurrentURL(void) const { return m_currentURL; }
    void SetCurrentURL(const std::string &URL) { m_currentURL = URL; }


    void AppendData(const void *data, size_t cb);
    CURLcode InflateBodyIfNecessary(CURLcode code);

    int GetHeader(const char *name, BkBuffer *dst) const override;
private:
    int GetData(int data, BkBuffer *dst) const override;
    int EnumerateHeaders(BkHttpHeaderEnumerator enumerator, void *userData) const override;
    size_t CookiesCount(void) const override { return m_cookies.size(); }
    int GetCookie(size_t index, BkBuffer *dst) const override;
    bool GZipInflate(void);

    int m_errorCode = BK_ERR_SUCCESS;
    std::string m_currentURL, m_httpVersion, m_statusText;
    BkHTTPHeaderMap m_headers;
    std::vector<std::string> m_cookies;
};

} // namespace BlinKit

#endif // BLINKIT_BKBASE_HTTP_RESPONSE_H
