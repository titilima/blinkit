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
    virtual ~RequestImpl(void);

    void Release(void);
    virtual void Cancel(void) = 0;
protected:
    RequestImpl(const char *URL, BkRequestClient &client);

    std::string GetAllHeaders(void) const;

    // BkRequest
    void BKAPI SetHeader(const char *name, const char *value) override;
    BkRequestController* BKAPI RequireLifecycleController(void) override;

    static const unsigned long TimeOutInMs;
    const std::string m_URL;
    BkRequestClient &m_client;
    std::string m_method;
    ResponseImpl *m_response = nullptr;
private:
    // BkNetRequest
    void BKAPI SetMethod(const char *method) override final { m_method = method; }

    std::unordered_map<std::string, std::string> m_headers;
    std::atomic<unsigned> m_refCount{ 1 };
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_REQUEST_IMPL_H
