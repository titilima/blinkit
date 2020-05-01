// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: login_proxy.h
// Description: LoginProxy Class
//      Author: Ziming Li
//     Created: 2020-04-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LOGIN_PROXY_H
#define BLINKIT_BLINKIT_LOGIN_PROXY_H

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string_view>
#include <vector>
#include "bk_crawler.h"
#include "blinkit/common/bk_socket.h"

namespace BlinKit {

class LoginTask;

class LoginProxy
{
public:
    static std::unique_ptr<LoginProxy> Create(const BkLoginProxyClient &client);
    virtual ~LoginProxy(void);

    const std::string& UserAgent(void) const;
    BkRequest CreateRequest(const std::string &URL, SOCKET socket);
    void SetCookie(const std::string &cookie);
    bool IsLoginSuccessful(const std::string &currentURL) const;
    std::string GetLoginSuccessfulResponseData(void) const;

    int Run(const char *loginURL, uint16_t port);
protected:
    LoginProxy(const BkLoginProxyClient &client);

    void RunListeningThread(void);
    int RunTaskLoop(void);

    virtual bool StartListeningThread(void) = 0;
private:
    bool InitSocket(uint16_t port);
    void ProcessRequestComplete(BkResponse response, SOCKET client);
    static void BKAPI ProcessRequestComplete(BkResponse response, void *userData);
    static void BKAPI ProcessRequestFailed(int errorCode, void *userData);

    BkLoginProxyClient m_client;
    std::string m_loginURL;
    mutable bool m_loggedIn = false;
    SOCKET m_socket = INVALID_SOCKET;

    mutable std::string m_userAgent;

    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::queue<BlinKit::LoginTask *> m_tasks;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOGIN_PROXY_H
