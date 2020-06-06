// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: request_impl.h
// Description: RequestImpl Class
//      Author: Ziming Li
//     Created: 2018-09-27
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_REQUEST_IMPL_H
#define BLINKIT_BLINKIT_REQUEST_IMPL_H

#pragma once

#include <atomic>
#include "bk_http.h"
#include "blinkit/common/bk_http_header_map.h"

class ControllerImpl;
class ResponseImpl;

class RequestImpl
{
public:
    static RequestImpl* CreateInstance(const char *URL, const BkRequestClient &client);
    virtual ~RequestImpl(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    virtual int Perform(void) = 0;
    void SetMethod(const std::string &method) { m_method = method; }
    virtual void SetHeader(const char *name, const char *value);
    void SetHeaders(const BlinKit::BkHTTPHeaderMap &headers) { m_headers = headers; }
    void SetBody(const void *data, size_t dataLength);
    void SetTimeout(unsigned timeout) { m_timeoutInMs = timeout * 1000; }
    void SetProxy(int type, const char *proxy);
    virtual ControllerImpl* GetController(void);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Release(void);
    virtual void Cancel(void) = 0;
protected:
    RequestImpl(const char *URL, const BkRequestClient &client);

    unsigned long TimeoutInMs(void) const { return m_timeoutInMs; }
    int ProxyType(void) const { return m_proxyType; }
    const std::string& Proxy(void) const;

    const std::string m_URL;
    BkRequestClient m_client;
    std::string m_method;
    BlinKit::BkHTTPHeaderMap m_headers;
    std::vector<unsigned char> m_body;
    std::shared_ptr<ResponseImpl> m_response;
private:
    std::atomic<unsigned> m_refCount{ 1 };
    unsigned long m_timeoutInMs;
    int m_proxyType = BK_PROXY_SYSTEM_DEFAULT;
    std::string m_proxy;
};

#endif // BLINKIT_BLINKIT_REQUEST_IMPL_H
