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
#include "sdk/include/BlinKit.h"

namespace BlinKit {

class ResponseImpl;

class RequestImpl : public BkRequest
{
public:
    static BkRequest* CreateInstance(const char *URL, BkRequestClient &client);
    virtual ~RequestImpl(void);

    void Release(void);
    virtual void Cancel(void) = 0;
protected:
    RequestImpl(const char *URL, BkRequestClient &client);

    std::string GetAllHeaders(void) const;
    unsigned long TimeoutInMs(void) const { return m_timeoutInMs; }

    // BkRequest
    void BKAPI SetHeader(const char *name, const char *value) override;
    BkRequestController* BKAPI RequireLifecycleController(void) override;

    const std::string m_URL;
    BkRequestClient &m_client;
    std::string m_method;
    std::vector<unsigned char> m_body;
    ResponseImpl *m_response = nullptr;
private:
    // BkNetRequest
    void BKAPI SetMethod(const char *method) override final { m_method = method; }
    void BKAPI SetTimeout(unsigned timeout) override final { m_timeoutInMs = timeout * 1000; }
    void BKAPI SetBody(const void *data, size_t dataLength) override final;

    std::atomic<unsigned> m_refCount{ 1 };
    unsigned long m_timeoutInMs;
    std::unordered_map<std::string, std::string> m_headers;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_REQUEST_IMPL_H
