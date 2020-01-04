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

#include "bk_app.h"
#if 0 // BKTODO:
#include "base/time/time.h"
#endif
#include "blinkit/app/app_constants.h"
#include "blinkit/blink_impl/url_loader_impl.h"
#include "third_party/blink/public/platform/web_cache.h"
#include "third_party/blink/public/platform/web_thread_scheduler.h"
#include "third_party/blink/public/web/blink.h"

#if 0 // BKTODO:
#include "blink_impl/cookie_jar_impl.h"
#include "blink_impl/mime_registry_impl.h"
#include "crawler/crawler_impl.h"
#endif

namespace BlinKit {

AppImpl::AppImpl(void)
{
    m_threadId = ThreadImpl::CurrentThreadId();
    AttachMainThread(this);
#if 0 // BKTODO:
    m_firstMonotonicallyIncreasingTime = base::Time::Now().ToDoubleT();
#endif
}

AppImpl::~AppImpl(void)
{
#if 0 // BKTODO:
    assert(theApp == this);
    theApp = nullptr;
#endif
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

blink::Settings& AppImpl::CrawlerSettings(void)
{
    do {
        if (m_crawlerSettings)
            break;

        AutoLock lock(m_lock);
        if (m_crawlerSettings)
            break;

        m_crawlerSettings = blink::Settings::create();
    } while (false);
    return *m_crawlerSettings;
}

BkCrawler* BKAPI AppImpl::CreateCrawler(BkCrawlerClient &client, BkBuffer *errorMessage)
{
    int errorCode;
    std::string message;

    CrawlerImpl *crawler = new CrawlerImpl(client);
    std::tie(errorCode, message) = crawler->Initialize();
    if (BkError::Success != errorCode)
    {
        if (nullptr != errorMessage)
            errorMessage->Assign(message);
        else
            assert(false);

        delete crawler;
        crawler = nullptr;
    }
    return crawler;
}

BkRequest* AppImpl::CreateRequest(const char *URL, BkRequestClient &client)
{
    if (nullptr != m_client)
    {
        BkRequest *request = m_client->CreateRequest(URL, client);
        if (nullptr != request)
            return request;
    }
    return BkCreateRequest(URL, client);
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
    assert(nullptr == client);
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

BKEXPORT void BKAPI BkFinalize(void)
{
    WebCache::Clear();
    delete Platform::Current();
}

BKEXPORT bool_t BKAPI BkInitialize(void *reserved)
{
    if (nullptr == Platform::Current())
    {
        AppImpl *app = AppImpl::CreateInstance();
        if (nullptr == app)
        {
            assert(nullptr != app);
            return false;
        }

        app->Initialize(nullptr);
    }
    return true;
}

} // extern "C"
