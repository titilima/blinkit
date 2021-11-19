// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: app_impl.cpp
// Description: AppImpl Class
//      Author: Ziming Li
//     Created: 2019-03-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "./app_impl.h"

#include "bkcommon/buffer_impl.hpp"
#include "blinkit/blink/impl/url_loader.h"
#include "blinkit/blink/public/web/blink.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/gc/gc_heap.h"
#include "blinkit/loader/loader_thread.h"
#include "chromium/base/time/time.h"

using namespace blink;

namespace BlinKit {

AppImpl::AppImpl(BkAppClient *client) : m_firstMonotonicallyIncreasingTime(base::Time::Now().ToDoubleT())
{
    GCHeap::Initialize();

    memset(&m_client, 0, sizeof(BkAppClient));
    if (nullptr != client)
    {
        size_t size = sizeof(BkAppClient);
        if (client->SizeOfStruct < size)
            size = client->SizeOfStruct;
        memcpy(&m_client, client, size);
    }

    m_threadId = Thread::CurrentThreadId();
    m_mainThread = this;

    blink::Initialize(this);
}

AppImpl::~AppImpl(void)
{
    GCHeap::Finalize();
}

WebURLLoader* AppImpl::createURLLoader(void)
{
    return new URLLoader;
}

WebThread* AppImpl::currentThread(void)
{
    if (isMainThread())
        return m_mainThread;
    NOTREACHED();
    return nullptr;
}

double AppImpl::currentTimeSeconds(void)
{
    return base::Time::Now().ToDoubleT();
}

AppImpl& AppImpl::Get(void)
{
    AppImpl *app = static_cast<AppImpl *>(Platform::current());
    return *app;
}

LoaderThread& AppImpl::GetLoaderThread(void)
{
    ASSERT(isMainThread());
    if (!m_loaderThread)
        m_loaderThread = LoaderThread::Create();
    return *m_loaderThread;
}

bool AppImpl::LoadResourceFromClient(const char *URI, std::string &dst) const
{
    ASSERT(nullptr != m_client.LoadResource);
    return m_client.LoadResource(URI, BufferImpl::Wrap(dst), m_client.UserData);
}

double AppImpl::monotonicallyIncreasingTimeSeconds(void)
{
    double t = currentTimeSeconds();
    return t - m_firstMonotonicallyIncreasingTime;
}

void AppImpl::OnExit(void)
{
    if (nullptr != m_client.Exit)
        m_client.Exit(m_client.UserData);
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace BlinKit;

extern "C" {

BKEXPORT bool_t BKAPI IsBlinKitThread(void)
{
    return isMainThread();
}

} // extern "C"
