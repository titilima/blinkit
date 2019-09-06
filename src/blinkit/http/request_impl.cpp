// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: request_impl.cpp
// Description: RequestImpl Class
//      Author: Ziming Li
//     Created: 2018-09-27
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "request_impl.h"

#include "app/app_constants.h"
#include "http/request_controller_impl.h"
#include "http/response_impl.h"

using namespace BlinKit;

RequestImpl::RequestImpl(const char *URL, const BkRequestClient &client)
    : m_URL(URL), m_client(client), m_method("GET"), m_timeoutInMs(AppConstants::DefaultTimeoutInMs)
{
    m_headers["Accept"] = "*/*";
}

RequestImpl::~RequestImpl(void) {}

std::string RequestImpl::GetAllHeaders(void) const
{
    std::string headers;
    for (const auto &it : m_headers)
    {
        headers.append(it.first);
        headers.append(": ");
        headers.append(it.second);
        headers.append("\r\n");
    }
    return headers;
}

ControllerImpl* RequestImpl::GetController(void)
{
    ++m_refCount;
    return new RequestControllerImpl(*this);
}

void RequestImpl::Release(void)
{
    if (0 == --m_refCount)
        delete this;
}

void RequestImpl::SetBody(const void *data, size_t dataLength)
{
    m_body.resize(dataLength);
    memcpy(m_body.data(), data, dataLength);
}

void RequestImpl::SetHeader(const char *name, const char *value)
{
    m_headers[name] = value;
}

void RequestImpl::SetProxy(const char *proxy)
{
    if (nullptr == proxy)
        m_proxy->clear();
    else
        m_proxy->assign(proxy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BkWorkController BKAPI BkGetRequestController(BkRequest request)
{
    return request->GetController();
}

int BKAPI BkPerformRequest(BkRequest request)
{
    return request->Perform();
}

void BKAPI BkSetRequestBody(BkRequest request, const void *data, size_t dataLength)
{
    request->SetBody(data, dataLength);
}

void BKAPI BkSetRequestHeader(BkRequest request, const char *name, const char *value)
{
    request->SetHeader(name, value);
}

void BKAPI BkSetRequestMethod(BkRequest request, const char *method)
{
    request->SetMethod(method);
}

void BKAPI BkSetRequestProxy(BkRequest request, const char *proxy)
{
    request->SetProxy(proxy);
}

void BKAPI BkSetRequestTimeout(BkRequest request, unsigned timeout)
{
    request->SetTimeout(timeout);
}

} // extern "C"
