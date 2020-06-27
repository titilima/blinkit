// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: login_proxy_impl.h
// Description: LoginProxyImpl Class
//      Author: Ziming Li
//     Created: 2020-04-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_LOGIN_PROXY_IMPL_H
#define BLINKIT_BKHELPER_LOGIN_PROXY_IMPL_H

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <unordered_map>
#include "bk_helper.h"
#include <openssl/ossl_typ.h>
#include "bkcommon/bk_socket.h"

namespace BlinKit {
class LoginTask;
class SSLPair;
}

class LoginProxyImpl
{
public:
    virtual ~LoginProxyImpl(void);

    int SetupCA(const BkPathChar *privateKeyFile, const BkPathChar *certFile);
    int Run(uint16_t port);

    void AddTask(BlinKit::LoginTask *task);
    SSL* NewSSL(const std::string &domain);
    const std::string& UserAgent(void) const;
    void SetCookie(const std::string &URL, const std::string &cookie);
    bool IsLoginSuccessful(const std::string &currentURL) const;
    std::string GetLoggedInHTML(void) const;
protected:
    LoginProxyImpl(const BkLoginProxyClient &client);

    void RunListeningThread(void);
    void StopListeningThread(uint16_t port);
    int RunTaskLoop(void);

    virtual bool StartListeningThread(void) = 0;
private:
    int ApplyPEM(const std::string &privateKey, const std::string &cert);

    bool InitSocket(uint16_t port);

    BkLoginProxyClient m_client;

    std::unique_ptr<BlinKit::SSLPair> m_caData;
    std::unordered_map<std::string, SSL_CTX *> m_sslData;

    mutable bool m_loggedIn = false;
    SOCKET m_socket = INVALID_SOCKET;

    mutable std::string m_userAgent;

    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::queue<BlinKit::LoginTask *> m_tasks;
};

#endif // BLINKIT_BKHELPER_LOGIN_PROXY_IMPL_H
