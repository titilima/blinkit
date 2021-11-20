#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: app_thread_request_client.h
// Description: AppThreadRequestClient Class
//      Author: Ziming Li
//     Created: 2021-11-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_APP_THREAD_REQUEST_CLIENT_H
#define BLINKIT_BLINKIT_APP_THREAD_REQUEST_CLIENT_H

#include "bk_http.h"

namespace blink {
class WebTaskRunner;
}

namespace BlinKit {

class AppThreadRequestClient
{
public:
    static BkRequestClient* Wrap(const std::shared_ptr<blink::WebTaskRunner> &taskRunner, const BkRequestClient &userClient);
    ~AppThreadRequestClient(void);
private:
    AppThreadRequestClient(const std::shared_ptr<blink::WebTaskRunner> &taskRunner, const BkRequestClient &userClient);

    void RequestComplete(BkResponse response);
    void RequestFailed(int errorCode);
    bool RequestRedirect(BkResponse response, BkRequest request);

    static void BKAPI RequestComplete(BkResponse response, void *userData);
    static void BKAPI RequestFailed(int errorCode, void *userData);
    static bool_t BKAPI RequestRedirect(BkResponse response, BkRequest request, void *userData);

    const std::shared_ptr<blink::WebTaskRunner> m_taskRunner;
    BkRequestClient m_userClient, m_wrappedClient;
    BkResponse m_response = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_THREAD_REQUEST_CLIENT_H
