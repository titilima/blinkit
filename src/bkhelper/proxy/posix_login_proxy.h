// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: posix_login_proxy.cpp
// Description: PosixLoginProxy Class
//      Author: Ziming Li
//     Created: 2020-05-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_POSIX_LOGIN_PROXY_H
#define BLINKIT_BKLOGIN_POSIX_LOGIN_PROXY_H

#pragma once

#include "bklogin/login/login_proxy_impl.h"

namespace BlinKit {

class PosixLoginProxy final : public LoginProxyImpl
{
public:
    PosixLoginProxy(const BkLoginProxyClient &client);
    ~PosixLoginProxy(void) override;
private:
    static void* ListeningThread(void *arg);

    bool StartListeningThread(void) override;

    pthread_t m_listeningThread = 0;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_POSIX_LOGIN_PROXY_H
