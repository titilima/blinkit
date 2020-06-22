// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_login_proxy.cpp
// Description: PosixLoginProxy Class
//      Author: Ziming Li
//     Created: 2020-05-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "posix_login_proxy.h"

namespace BlinKit {

PosixLoginProxy::PosixLoginProxy(const BkLoginProxyClient &client) : LoginProxy(client)
{
}

PosixLoginProxy::~PosixLoginProxy(void)
{
    pthread_join(m_listeningThread, nullptr);
}

void* PosixLoginProxy::ListeningThread(void *arg)
{
    reinterpret_cast<PosixLoginProxy *>(arg)->RunListeningThread();
    return nullptr;
}

bool PosixLoginProxy::StartListeningThread(void)
{
    int r = pthread_create(&m_listeningThread, nullptr, ListeningThread, this);
    return 0 == r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<LoginProxy> LoginProxy::Create(const BkLoginProxyClient &client)
{
    return std::make_unique<PosixLoginProxy>(client);
}

} // namespace BlinKit
