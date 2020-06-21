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

#include "bkcommon/bk_http_header_map.h"
#include "bkcommon/response_impl.h"

namespace BlinKit {

class HttpResponse final : public ResponseBase
{
public:
    HttpResponse(const std::string &URL);

    int ErrorCode(void) const { return m_errorCode; }
    void SetErrorCode(int errorCode) { m_errorCode = errorCode; }

    const std::string& CurrentURL(void) const { return m_currentURL; }
    void SetCurrentURL(const std::string &URL) { m_currentURL = URL; }

    void ParseHeaders(const std::string &rawHeaders);

    std::string ResolveRedirection(void);
    void ResetForRedirection(void);

    void PrepareBody(size_t cb) { m_body.reserve(cb); }
    void AppendData(const void *data, size_t cb);
    void GZipInflate(void);

    int GetHeader(const char *name, BkBuffer *dst) const override;
private:
    int GetData(int data, BkBuffer *dst) const override;
    size_t CookiesCount(void) const override { return m_cookies.size(); }
    int GetCookie(size_t index, BkBuffer *dst) const override;

    int m_errorCode = BK_ERR_SUCCESS;
    std::string m_currentURL, m_httpVersion, m_reason;
    BkHTTPHeaderMap m_headers;
    std::vector<std::string> m_cookies;
};

} // namespace BlinKit

#endif // BLINKIT_BKBASE_HTTP_RESPONSE_H
