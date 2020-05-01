// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_login_proxy.h
// Description: WinLoginProxy Class
//      Author: Ziming Li
//     Created: 2020-04-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_LOGIN_PROXY_H
#define BLINKIT_BLINKIT_WIN_LOGIN_PROXY_H

#pragma once

#include "blinkit/crawler/login_proxy.h"

namespace BlinKit {

class WinLoginProxy final : public LoginProxy
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

#endif // BLINKIT_BLINKIT_WIN_LOGIN_PROXY_H
