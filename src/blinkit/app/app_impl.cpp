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

#include "base/time/time.h"
#include "public/web/WebCache.h"
#include "public/web/WebKit.h"

#include "app/app_constants.h"
#include "blink_impl/cookie_jar_impl.h"
#include "blink_impl/mime_registry_impl.h"
#include "blink_impl/url_loader_impl.h"
#include "crawler/crawler_impl.h"

namespace BlinKit {

static AppImpl *theApp = nullptr;

AppImpl::AppImpl(void)
{
    assert(nullptr == theApp);
    theApp = this;

    m_firstMonotonicallyIncreasingTime = base::Time::Now().ToDoubleT();

    blink::PlatformThreadId currentThreadId = ThreadImpl::CurrentThreadId();
    ThreadImpl::ApplyThreadId(currentThreadId);
    m_threads[currentThreadId] = this;
}

AppImpl::~AppImpl(void)
{
    assert(theApp == this);
    theApp = nullptr;
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

BkCrawler* BKAPI AppImpl::CreateCrawler(BkCrawlerClient &client)
{
    return new CrawlerImpl(client);
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
    blink::WebThread *thread = ThreadImpl::CreateInstance(name);

    AutoLock lock(m_lock);
    m_threads[thread->threadId()] = thread;

    return thread;
}

blink::WebURLLoader* AppImpl::createURLLoader(void)
{
    return new URLLoaderImpl;
}

BkView* BKAPI AppImpl::CreateView(BkViewClient &client)
{
    assert(false); // Not reached!
    return nullptr;
}

blink::WebThread* AppImpl::currentThread(void)
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

void BKAPI AppImpl::Exit(void)
{
    blink::WebCache::clear();
    blink::shutdown();
    delete theApp;
}

AppImpl& AppImpl::Get(void)
{
    return *theApp;
}

void AppImpl::Initialize(BkAppClient *client)
{
    m_client = client;
    blink::initialize(this);
}

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

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace BlinKit;

extern "C" BkApp* BKAPI BkGetApp(void)
{
    return theApp;
}

extern "C" BkApp* BKAPI BkInitialize(BkAppClient *client)
{
    assert(nullptr == theApp);
    if (nullptr == theApp)
    {
        AppImpl::CreateInstance();
        assert(nullptr != theApp);
        theApp->Initialize(client);
    }
    return theApp;
}
