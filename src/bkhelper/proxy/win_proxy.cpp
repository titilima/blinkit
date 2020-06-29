// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: win_proxy.cpp
// Description: WinProxy Class
//      Author: Ziming Li
//     Created: 2020-04-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "win_proxy.h"

namespace BlinKit {

WinProxy::WinProxy(const BkProxyClient &client) : ProxyImpl(client)
{
}

WinProxy::~WinProxy(void)
{
    if (nullptr != m_listeningThread)
    {
        WaitForSingleObject(m_listeningThread, INFINITE);
        CloseHandle(m_listeningThread);
    }
    WSACleanup();
}

DWORD WINAPI WinProxy::ListeningThread(PVOID param)
{
    reinterpret_cast<WinProxy *>(param)->RunListeningThread();
    return EXIT_SUCCESS;
}

bool WinProxy::StartListeningThread(void)
{
    m_listeningThread = CreateThread(nullptr, 0, ListeningThread, this, 0, nullptr);
    return nullptr != m_listeningThread;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkProxy BKAPI BkCreateProxy(struct BkProxyClient *client)
{
    WSADATA wsaData;
    int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (0 != r)
    {
        ASSERT(0 == r);
        return nullptr;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        ASSERT(LOBYTE(wsaData.wVersion) == 2 && HIBYTE(wsaData.wVersion) == 2);
        WSACleanup();
        return nullptr;
    }

    return new BlinKit::WinProxy(*client);
}

} // extern "C"
