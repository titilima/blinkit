// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: response_task.h
// Description: ResponseTask Class
//      Author: Ziming Li
//     Created: 2020-06-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_RESPONSE_TASK_H
#define BLINKIT_BKHELPER_RESPONSE_TASK_H

#pragma once

#include <unordered_map>
#include "bk_http.h"
#include "bkhelper/proxy/socket_wrapper.h"
#include "bkhelper/proxy/tasks/proxy_task.h"

class ResponseImpl;

namespace BlinKit {

class BkHTTPHeaderMap;

class ResponseTask final : public ProxyTask
{
public:
    ResponseTask(const std::shared_ptr<SocketWrapper> &socketWrapper, ProxyImpl &proxy);
    ~ResponseTask(void) override;

    BkRequest CreateRequest(const std::string &URL);
private:
    typedef std::unordered_map<std::string, std::string> HttpHeaders;
    void AdjustHeaders(HttpHeaders &headers, ProxyImpl &proxy);
    static bool_t BKAPI HeaderCallback(const char *k, const char *v, void *userData);

    void ProcessResponse(ProxyImpl &proxy);
    void ProcessRequestComplete(BkResponse response);
    void ProcessRequestFailed(int errorCode);

    static void BKAPI RequestCompleteImpl(BkResponse response, void *userData);
    static void BKAPI RequestFailedImpl(int errorCode, void *userData);
    static bool_t BKAPI RequestRedirectImpl(BkResponse, void *) { return false; }

    ProxyTask* Execute(ProxyImpl &proxy) override;

    std::shared_ptr<SocketWrapper> m_socketWrapper;
    ProxyImpl &m_proxy;
    ResponseImpl *m_response = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_RESPONSE_TASK_H
