// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_proxy.cpp
// Description: PosixProxy Class
//      Author: Ziming Li
//     Created: 2020-05-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "posix_proxy.h"

namespace BlinKit {

PosixProxy::PosixProxy(const BkProxyClient &client) : ProxyImpl(client)
{
}

PosixProxy::~PosixProxy(void)
{
    pthread_join(m_listeningThread, nullptr);
}

void* PosixProxy::ListeningThread(void *arg)
{
    reinterpret_cast<PosixProxy *>(arg)->RunListeningThread();
    return nullptr;
}

bool PosixProxy::StartListeningThread(void)
{
    int r = pthread_create(&m_listeningThread, nullptr, ListeningThread, this);
    return 0 == r;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkProxy BKAPI BkCreateProxy(struct BkProxyClient *client)
{
    return new BlinKit::PosixProxy(*client);
}

} // extern "C"
