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
#include "blinkit/app/heap_storage.h"
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

AppImpl::AppImpl(int mode, BkAppClient *client)
    : m_mode(mode)
    , m_gcHeap(std::make_unique<GCHeap>())
#ifndef BLINKIT_CRAWLER_ONLY
    , m_heapStorage(new HeapStorage)
#endif
{
    memset(&m_client, 0, sizeof(BkAppClient));
    if (nullptr != client)
    {
        size_t size = sizeof(BkAppClient);
        if (client->SizeOfStruct < size)
            size = client->SizeOfStruct;
        memcpy(&m_client, client, size);
    }

    m_threadId = ThreadImpl::CurrentThreadId();
    AttachMainThread(this);
}

AppImpl::~AppImpl(void)
{
    if (nullptr != m_client.Exit)
        m_client.Exit(m_client.UserData);
}

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

void AppImpl::Initialize(BkAppClient *client)
{
    ASSERT(nullptr == client); // BKTODO:
    blink::Initialize(this, m_mainThreadScheduler.get());
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace blink;
using namespace BlinKit;

extern "C" {

BKEXPORT bool_t BKAPI BkAppExecute(BkBackgroundWorker worker, void *userData)
{
    AppImpl &app = AppImpl::Get();
    switch (app.Mode())
    {
        case BK_APP_BACKGROUND_MODE:
        {
            const auto task = [worker, userData]
            {
                worker(userData);
            };
            app.GetTaskRunner()->PostTask(FROM_HERE, task);
            return true;
        }

        default:
            NOTREACHED();
    }
    return false;
}

BKEXPORT void BKAPI BkExitApp(int code)
{
    AppImpl &app = AppImpl::Get();
    switch (app.Mode())
    {
        case BK_APP_MAINTHREAD_MODE:
        case BK_APP_BACKGROUND_MODE:
            app.Exit(code);
            break;
        default:
            NOTREACHED();
    }
}

BKEXPORT void BKAPI BkFinalize(void)
{
    Platform *p = Platform::Current();
    if (nullptr == p)
        return;

    AppImpl *app = static_cast<AppImpl *>(p);
    switch (app->Mode())
    {
        case BK_APP_MAINTHREAD_MODE:
            delete app;
            break;
        default:
            NOTREACHED();
    }
}

BKEXPORT bool_t BKAPI BkInitialize(int mode, BkAppClient *client)
{
    if (nullptr != Platform::Current())
        return false;

    switch (mode)
    {
        case BK_APP_MAINTHREAD_MODE:
        {
            AppImpl *app = AppImpl::CreateInstance(mode, client);
            if (nullptr == app)
            {
                ASSERT(nullptr != app);
                return false;
            }

            app->Initialize(nullptr);
            break;
        }
        case BK_APP_BACKGROUND_MODE:
        {
            AppImpl::InitializeBackgroundInstance(client);
            break;
        }

        default:
            NOTREACHED();
            return false;
    }
    return true;
}

BKEXPORT int BKAPI BkRunApp(void)
{
    AppImpl &app = AppImpl::Get();
    switch (app.Mode())
    {
        case BK_APP_MAINTHREAD_MODE:
            return app.RunAndFinalize();
        default:
            NOTREACHED();
    }
    return EXIT_FAILURE;
}

} // extern "C"
