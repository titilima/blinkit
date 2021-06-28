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

#include "app_impl.h"

#include "base/single_thread_task_runner.h"
#include "blinkit/blink/impl/url_loader.h"
#include "blinkit/blink/public/web/WebKit.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/gc/gc_heap.h"
#include "blinkit/loader/loader_thread.h"
#if 0 // BKTODO:
#include "third_party/blink/public/platform/web_thread_scheduler.h"
#include "third_party/blink/public/web/blink.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

#include "blink_impl/mime_registry_impl.h"
#endif

using namespace blink;

namespace BlinKit {

AppImpl::AppImpl(BkAppClient *client) : m_gcHeap(std::make_unique<GCHeap>())
{
    memset(&m_client, 0, sizeof(BkAppClient));
    if (nullptr != client)
    {
        size_t size = sizeof(BkAppClient);
        if (client->SizeOfStruct < size)
            size = client->SizeOfStruct;
        memcpy(&m_client, client, size);
    }
}

AppImpl::~AppImpl(void) = default;

WebURLLoader* AppImpl::createURLLoader(void)
{
    return new URLLoader;
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

void AppImpl::Initialize(void)
{
    m_threadId = Thread::CurrentThreadId();
    m_threads[m_threadId] = this;

    blink::initialize(this);
}

void AppImpl::OnExit(void)
{
    if (nullptr != m_client.Exit)
        m_client.Exit(m_client.UserData);
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace blink;
using namespace BlinKit;

extern "C" {

#ifdef BLINKIT_CRAWLER_ONLY
BKEXPORT int BKAPI BkCrawlerMain(BkAppClient *client, void (BKAPI * Init)(void *))
{
    Platform *p = Platform::Current();
    if (nullptr != p)
    {
        ASSERT(nullptr == p);
        return EXIT_FAILURE;
    }

    std::unique_ptr<AppImpl> app = AppImpl::CreateInstanceForExclusiveMode(client);
    Init(client->UserData);
    return app->RunMessageLoop();
}

BKEXPORT void BKAPI BkExit(int code)
{
    AppImpl::Get().Exit(code);
}
#endif

BKEXPORT void BKAPI BkFinalize(void)
{
    Platform *p = Platform::current();
    if (nullptr == p)
        return;

    static_cast<AppImpl *>(p)->Exit(EXIT_SUCCESS);
}

BKEXPORT bool_t BKAPI BkInitialize(BkAppClient *client)
{
    if (nullptr != Platform::current())
        return false;
    return AppImpl::InitializeForBackgroundMode(client);
}

BKEXPORT bool_t BKAPI IsBlinKitThread(void)
{
    return isMainThread();
}

} // extern "C"
