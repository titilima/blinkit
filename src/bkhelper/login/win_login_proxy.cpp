// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: win_login_proxy.cpp
// Description: WinLoginProxy Class
//      Author: Ziming Li
//     Created: 2020-04-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "win_login_proxy.h"

namespace BlinKit {

WinLoginProxy::WinLoginProxy(const BkLoginProxyClient &client) : LoginProxyImpl(client)
{
}

WinLoginProxy::~WinLoginProxy(void)
{
    if (nullptr != m_listeningThread)
    {
        WaitForSingleObject(m_listeningThread, INFINITE);
        CloseHandle(m_listeningThread);
    }
    WSACleanup();
}

DWORD WINAPI WinLoginProxy::ListeningThread(PVOID param)
{
    reinterpret_cast<WinLoginProxy *>(param)->RunListeningThread();
    return EXIT_SUCCESS;
}

bool WinLoginProxy::StartListeningThread(void)
{
    m_listeningThread = CreateThread(nullptr, 0, ListeningThread, this, 0, nullptr);
    return nullptr != m_listeningThread;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkLoginProxy BKAPI BkCreateLoginProxy(struct BkLoginProxyClient *client)
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

    return new BlinKit::WinLoginProxy(*client);
}

} // extern "C"
