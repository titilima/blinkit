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
#include "sdk/include/BlinKit.h"

namespace BlinKit {

class ResponseImpl final : public BkRetainedResponse
{
public:
    int ErrorCode(void) const { return m_errorCode; }
    void SetErrorCode(int errorCode) { m_errorCode = errorCode; }

    void ParseHeaders(const std::string &rawHeaders);
    void PrepareBody(size_t cb) { m_body.reserve(cb); }
    void AppendData(void *data, size_t cb);
    void GZipInflate(void);

    // BkResponse
    void BKAPI Release(void) override;
    int BKAPI GetHeader(const char *name, BkBuffer &value) const override;
private:
    // BkResponse
    int BKAPI GetHTTPVersion(BkBuffer &v) const override;
    int BKAPI StatusCode(void) const override { return m_statusCode; }
    int BKAPI GetReasonPhrase(BkBuffer &reasonPhrase) const override;
    unsigned BKAPI CookiesCount(void) const override { return m_cookies.size(); }
    int BKAPI GetCookie(unsigned i, BkBuffer &cookie) const override;
    int BKAPI GetBody(BkBuffer &body) const override;
    BkRetainedResponse* BKAPI Retain(void) const override;

    std::atomic<unsigned> m_refCount{ 1 };

    std::string m_version;
    int m_errorCode = BkError::Success, m_statusCode = 0;
    std::string m_reasonPhrase;
    std::unordered_map<std::string, std::string> m_headers;
    std::vector<std::string> m_cookies;
    std::vector<unsigned char> m_body;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_RESPONSE_IMPL_H
