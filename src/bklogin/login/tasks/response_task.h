// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: response_task.h
// Description: ResponseTask Class
//      Author: Ziming Li
//     Created: 2020-06-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_RESPONSE_TASK_H
#define BLINKIT_BKLOGIN_RESPONSE_TASK_H

#pragma once

#include <unordered_map>
#include "bklogin/login/socket_wrapper.h"
#include "bklogin/login/tasks/login_task.h"

class ResponseImpl;

namespace BlinKit {

class BkHTTPHeaderMap;

class ResponseTask final : public LoginTask
{
public:
    ResponseTask(const std::shared_ptr<SocketWrapper> &socketWrapper, LoginProxyImpl &loginProxy);
    ~ResponseTask(void) override;

    BkRequest CreateRequest(const std::string &URL);
private:
    typedef std::unordered_map<std::string, std::string> HttpHeaders;
    void AdjustHeaders(HttpHeaders &headers, LoginProxyImpl &loginProxy);
    static bool_t BKAPI HeaderCallback(const char *k, const char *v, void *userData);

    bool ProcessLoginOK(LoginProxyImpl &loginProxy);
    void ProcessResponse(LoginProxyImpl &loginProxy);
    void ProcessRequestComplete(BkResponse response);
    void ProcessRequestFailed(int errorCode);

    static void BKAPI RequestCompleteImpl(BkResponse response, void *userData);
    static void BKAPI RequestFailedImpl(int errorCode, void *userData);
    static bool_t BKAPI RequestRedirectImpl(BkResponse, void *) { return false; }

    LoginTask* Execute(LoginProxyImpl &loginProxy) override;

    std::shared_ptr<SocketWrapper> m_socketWrapper;
    LoginProxyImpl &m_loginProxy;
    ResponseImpl *m_response = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_RESPONSE_TASK_H
