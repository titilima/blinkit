// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: app_thread_request_client.cpp
// Description: AppThreadRequestClient Class
//      Author: Ziming Li
//     Created: 2021-11-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./app_thread_request_client.h"

#include "bkcommon/response_impl.h"
#include "blinkit/app/app_impl.h"
#include "blinkit/blink/public/platform/WebTaskRunner.h"
#include "blinkit/blink/public/platform/WebTraceLocation.h"

using namespace blink;

namespace BlinKit {

AppThreadRequestClient::AppThreadRequestClient(const std::shared_ptr<WebTaskRunner> &taskRunner, const BkRequestClient &userClient)
    : m_taskRunner(taskRunner)
{
    constexpr size_t clientSize = sizeof(BkRequestClient);

    memcpy(&m_userClient, &userClient, std::min(userClient.SizeOfStruct, clientSize));

    memset(&m_wrappedClient, 0, clientSize);
    m_wrappedClient.UserData        = this;
    m_wrappedClient.RequestComplete = RequestComplete;
    m_wrappedClient.RequestFailed   = RequestFailed;
    if (nullptr != m_userClient.RequestRedirect)
        m_wrappedClient.RequestRedirect = RequestRedirect;
}

AppThreadRequestClient::~AppThreadRequestClient(void)
{
    if (nullptr != m_response)
        m_response->Release();
}

void AppThreadRequestClient::RequestComplete(BkResponse response)
{
    m_response = response->Retain();
    auto task = [this] {
        m_userClient.RequestComplete(m_response, m_userClient.UserData);
        delete this;
    };
    m_taskRunner->postTask(BLINK_FROM_HERE, std::move(task));
}

void BKAPI AppThreadRequestClient::RequestComplete(BkResponse response, void *userData)
{
    reinterpret_cast<AppThreadRequestClient *>(userData)->RequestComplete(response);
}

void AppThreadRequestClient::RequestFailed(int errorCode)
{
    auto task = [this, errorCode] {
        m_userClient.RequestFailed(errorCode, m_userClient.UserData);
        delete this;
    };
    m_taskRunner->postTask(BLINK_FROM_HERE, std::move(task));
}

void BKAPI AppThreadRequestClient::RequestFailed(int errorCode, void *userData)
{
    reinterpret_cast<AppThreadRequestClient *>(userData)->RequestFailed(errorCode);
}

bool AppThreadRequestClient::RequestRedirect(BkResponse response, BkRequest request)
{
    ASSERT(nullptr != m_userClient.RequestRedirect);
    return m_userClient.RequestRedirect(response, request, m_userClient.UserData);
}

bool_t BKAPI AppThreadRequestClient::RequestRedirect(BkResponse response, BkRequest request, void *userData)
{
    return reinterpret_cast<AppThreadRequestClient *>(userData)->RequestRedirect(response, request);
}

BkRequestClient* AppThreadRequestClient::Wrap(const std::shared_ptr<WebTaskRunner> &taskRunner, const BkRequestClient &userClient)
{
    AppThreadRequestClient *wrapper = new AppThreadRequestClient(taskRunner, userClient);
    return &(wrapper->m_wrappedClient);
}

} // namespace BlinKit

using namespace BlinKit;

extern "C" {

BKEXPORT BkRequest BKAPI BkCreateRequestForAppThread(const char *URL, struct BkRequestClient *client)
{
    std::shared_ptr<WebTaskRunner> taskRunner = AppImpl::Get().taskRunner();
    client = AppThreadRequestClient::Wrap(taskRunner, *client);
    return BkCreateRequest(URL, client);
}

} // extern "C"
