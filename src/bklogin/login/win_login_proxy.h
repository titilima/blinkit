// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: win_login_proxy.h
// Description: WinLoginProxy Class
//      Author: Ziming Li
//     Created: 2020-04-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_WIN_LOGIN_PROXY_H
#define BLINKIT_BKLOGIN_WIN_LOGIN_PROXY_H

#pragma once

#include "bklogin/login/login_proxy_impl.h"

namespace BlinKit {

class WinLoginProxy final : public LoginProxyImpl
{
public:
    WinLoginProxy(const BkLoginProxyClient &client);
    ~WinLoginProxy(void) override;
private:
    static DWORD WINAPI ListeningThread(PVOID param);

    bool StartListeningThread(void) override;

    HANDLE m_listeningThread = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_WIN_LOGIN_PROXY_H
