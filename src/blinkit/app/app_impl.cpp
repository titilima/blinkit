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
#include "blinkit/app/app_constants.h"
#include "blinkit/blink_impl/url_loader_impl.h"
#include "third_party/blink/public/platform/web_thread_scheduler.h"
#include "third_party/blink/public/web/blink.h"

#if 0 // BKTODO:
#include "blink_impl/cookie_jar_impl.h"
#include "blink_impl/mime_registry_impl.h"
#include "crawler/crawler_impl.h"
#endif

namespace BlinKit {

AppImpl::AppImpl(int mode, BkAppClient *client) : m_mode(mode)
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

#if 0 // BKTODO:
blink::WebURLError AppImpl::cancelledError(const blink::WebURL &url) const
{
    blink::WebURLError ret;
    ret.reason = BkError::Cancelled;
    ret.isCancellation = true;
    return ret;
}

CookieJarImpl& AppImpl::CookieJar(void)
{
    if (!m_cookieJar)
    {
        AutoLock lock(m_lock);
        if (!m_cookieJar)
            m_cookieJar = std::make_unique<CookieJarImpl>();
    }
    return *m_cookieJar;
}

blink::WebCookieJar* AppImpl::cookieJar(void)
{
    CookieJarImpl &cookieJar = CookieJar();
    return &cookieJar;
}

blink::WebThread* AppImpl::createThread(const char *name)
{
    ThreadImpl *thread = ThreadImpl::CreateInstance(name);

    AutoLock lock(m_lock);
    m_threads[thread->threadId()] = thread;

    return thread;
}

BkView* BKAPI AppImpl::CreateView(BkViewClient &client)
{
    assert(false); // Not reached!
    return nullptr;
}

blink::WebThread* AppImpl::currentThread(void)
{
    return CurrentThreadImpl();
}

ThreadImpl* AppImpl::CurrentThreadImpl(void)
{
    AutoLock lock(m_lock);
    auto it = m_threads.find(ThreadImpl::CurrentThreadId());
    if (std::end(m_threads) != it)
        return it->second;

    assert(std::end(m_threads) != it);
    return nullptr;
}

double AppImpl::currentTimeSeconds(void)
{
    return base::Time::Now().ToDoubleT();
}
#endif // 0

AppImpl& AppImpl::Get(void)
{
    AppImpl *app = static_cast<AppImpl *>(Platform::Current());
    return *app;
}

void AppImpl::Initialize(BkAppClient *client)
{
    ASSERT(nullptr == client); // BKTODO:
    blink::Initialize(this, m_mainThreadScheduler.get());
}

#if 0 // BKTODO:
blink::WebThread& AppImpl::IOThread(void)
{
    if (!m_IOThread)
    {
        AutoLock lock(m_lock);
        if (!m_IOThread)
            m_IOThread.reset(createThread("IO"));
    }
    return *m_IOThread;
}

blink::WebMimeRegistry* AppImpl::mimeRegistry(void)
{
    if (!m_mimeRegistry)
    {
        AutoLock lock(m_lock);
        if (!m_mimeRegistry)
            m_mimeRegistry = std::make_unique<MimeRegistryImpl>();
    }
    return m_mimeRegistry.get();
}

double AppImpl::monotonicallyIncreasingTimeSeconds(void)
{
    double t = currentTimeSeconds();
    return t - m_firstMonotonicallyIncreasingTime;
}

blink::WebString AppImpl::userAgent(void)
{
    return blink::WebString::fromUTF8(AppConstants::DefaultUserAgent);
}
#endif // 0

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
