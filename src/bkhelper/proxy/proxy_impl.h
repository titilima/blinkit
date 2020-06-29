// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: proxy_impl.h
// Description: ProxyImpl Class
//      Author: Ziming Li
//     Created: 2020-04-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_PROXY_IMPL_H
#define BLINKIT_BKHELPER_PROXY_IMPL_H

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <unordered_map>
#include "bk_helper.h"
#include <openssl/ossl_typ.h>
#include "bkcommon/bk_socket.h"

namespace BlinKit {
class ProxyTask;
class RequestTaskBase;
class SSLPair;
}

class ProxyImpl
{
public:
    virtual ~ProxyImpl(void);

    int SetupCA(const BkPathChar *privateKeyFile, const BkPathChar *certFile);
    int RunLoop(uint16_t port);
    void ExitLoop(void);

    void AddTask(BlinKit::ProxyTask *task);
    SSL* NewSSL(const std::string &domain);
    bool PreProcessRequestPerform(BlinKit::RequestTaskBase *requestTask);
    bool PreProcessRequestComplete(BkResponse response);
    std::string GetExitBody(void) const;
protected:
    ProxyImpl(const BkProxyClient &client);

    void RunListeningThread(void);
    void StopListeningThread(void);
    void RunTaskLoop(void);

    virtual bool StartListeningThread(void) = 0;
private:
    int ApplyPEM(const std::string &privateKey, const std::string &cert);

    bool InitSocket(uint16_t port);

    BkProxyClient m_client;

    std::unique_ptr<BlinKit::SSLPair> m_caData;
    std::unordered_map<std::string, SSL_CTX *> m_sslData;

    bool m_running = true;
    const std::string m_exitString;
    SOCKET m_socket = INVALID_SOCKET;
    uint16_t m_port = 0;

    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::queue<BlinKit::ProxyTask *> m_tasks;
};

#endif // BLINKIT_BKHELPER_PROXY_IMPL_H
