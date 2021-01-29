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
#include "blinkit/blink_impl/url_loader_impl.h"
#include "blinkit/gc/gc_heap.h"
#include "blinkit/loader/loader_thread.h"
#include "third_party/blink/public/platform/web_thread_scheduler.h"
#include "third_party/blink/public/web/blink.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

#if 0 // BKTODO:
#include "blink_impl/mime_registry_impl.h"
#endif

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

std::unique_ptr<blink::WebURLLoader> AppImpl::CreateURLLoader(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner)
{
    return std::make_unique<URLLoaderImpl>(taskRunner);
}

AppImpl& AppImpl::Get(void)
{
    AppImpl *app = static_cast<AppImpl *>(Platform::Current());
    return *app;
}

LoaderThread& AppImpl::GetLoaderThread(void)
{
    ASSERT(IsMainThread());
    if (!m_loaderThread)
        m_loaderThread = LoaderThread::Create();
    return *m_loaderThread;
}

void AppImpl::Initialize(void)
{
    m_threadId = ThreadImpl::CurrentThreadId();
    AttachMainThread(this);

    blink::Initialize(this);
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
    Platform *p = Platform::Current();
    if (nullptr == p)
        return;

    static_cast<AppImpl *>(p)->Exit(EXIT_SUCCESS);
}

BKEXPORT bool_t BKAPI BkInitialize(BkAppClient *client)
{
    if (nullptr != Platform::Current())
        return false;
    return AppImpl::InitializeForBackgroundMode(client);
}

} // extern "C"
