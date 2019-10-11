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
#include <optional>
#include "bk_http.h"
#include "blinkit/common/bk_http_header_map.h"

class ControllerImpl;
class ResponseImpl;

class RequestImpl
{
public:
    virtual ~RequestImpl(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    virtual int Perform(void) = 0;
    void SetMethod(const char *method) { m_method = method; }
    virtual void SetHeader(const char *name, const char *value);
    void SetBody(const void *data, size_t dataLength);
    void SetTimeout(unsigned timeout) { m_timeoutInMs = timeout * 1000; }
    void SetProxy(const char *proxy);
    virtual ControllerImpl* GetController(void);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Release(void);
    virtual void Cancel(void) = 0;
protected:
    RequestImpl(const char *URL, const BkRequestClient &client);

    unsigned long TimeoutInMs(void) const { return m_timeoutInMs; }
    bool HasProxy(void) const { return m_proxy.has_value(); }
    const std::string& Proxy(void) const
    {
        assert(m_proxy.has_value());
        return *m_proxy;
    }

    const std::string m_URL;
    BkRequestClient m_client;
    std::string m_method;
    BlinKit::BkHTTPHeaderMap m_headers;
    std::vector<unsigned char> m_body;
    std::unique_ptr<ResponseImpl> m_response;
private:
    std::atomic<unsigned> m_refCount{ 1 };
    unsigned long m_timeoutInMs;
    std::optional<std::string> m_proxy;
};

#endif // BLINKIT_BLINKIT_REQUEST_IMPL_H
