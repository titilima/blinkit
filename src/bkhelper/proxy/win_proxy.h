// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: win_proxy.h
// Description: WinProxy Class
//      Author: Ziming Li
//     Created: 2020-04-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_WIN_PROXY_H
#define BLINKIT_BKHELPER_WIN_PROXY_H

#pragma once

#include "bkhelper/proxy/proxy_impl.h"

namespace BlinKit {

class WinProxy final : public ProxyImpl
{
public:
    WinProxy(const BkProxyClient &client);
    ~WinProxy(void) override;
private:
    static DWORD WINAPI ListeningThread(PVOID param);

    bool StartListeningThread(void) override;

    HANDLE m_listeningThread = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_WIN_PROXY_H
