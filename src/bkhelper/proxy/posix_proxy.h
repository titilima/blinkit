// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: posix_proxy.cpp
// Description: PosixProxy Class
//      Author: Ziming Li
//     Created: 2020-05-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_POSIX_PROXY_H
#define BLINKIT_BKLOGIN_POSIX_PROXY_H

#pragma once

#include "bkhelper/proxy/proxy_impl.h"

namespace BlinKit {

class PosixProxy final : public ProxyImpl
{
public:
    PosixProxy(const BkProxyClient &client);
    ~PosixProxy(void) override;
private:
    static void* ListeningThread(void *arg);

    bool StartListeningThread(void) override;

    pthread_t m_listeningThread = 0;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_POSIX_PROXY_H
