// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_impl.cpp
// Description: CrawlerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "crawler_impl.h"

#include "bkcommon/buffer_impl.hpp"
#include "bkcommon/controller_impl.h"
#include "bkcommon/response_impl.h"
#include "blinkit/app/app_impl.h"
#include "blinkit/blink/renderer/bindings/core/script_controller.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/blink/renderer/core/loader/FrameLoadRequest.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/crawler/cookie_jar_impl.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/platform/loader/fetch/resource_error.h"
#endif

using namespace blink;
using namespace BlinKit;

CrawlerImpl::CrawlerImpl(const BkCrawlerClient &client)
    : m_client(client)
    , m_frame(LocalFrame::create(this))
{
    ASSERT(isMainThread());
    m_frame->init();
}

CrawlerImpl::~CrawlerImpl(void)
{
    ASSERT(isMainThread());
    m_frame->detach();
    if (nullptr != m_cookieJar)
        m_cookieJar->Release();
}

void CrawlerImpl::CancelLoading(void)
{
    ASSERT(isMainThread());
    m_frame->loader().stopAllLoaders();
}

void CrawlerImpl::Destroy(void)
{
    ASSERT(isMainThread());
    delete this;
}

void CrawlerImpl::dispatchDidFinishLoad(void)
{
    m_frame->script().EnsureContext();
    AppImpl::Get().taskRunner()->postTask(BLINK_FROM_HERE, std::bind(m_client.DocumentReady, this, m_client.UserData));
}

#if 0 // BKTODO:
void CrawlerImpl::DispatchDidFailProvisionalLoad(const ResourceError &error)
{
    AutoGarbageCollector gc;

    int errorCode = error.ErrorCode();
    std::string URL = error.FailingURL();
    auto task = [this, errorCode, URL]
    {
        m_client.Error(errorCode, URL.c_str(), m_client.UserData);
    };
    m_clientCaller.Post(BLINK_FROM_HERE, task);
}
#endif

CrawlerImpl* CrawlerImpl::From(const Document &document)
{
    ASSERT(document.isCrawlerNode());
    return static_cast<CrawlerImpl *>(document.frame()->client());
}

CookieJarImpl* CrawlerImpl::GetCookieJar(bool createIfNotExists)
{
    if (createIfNotExists)
    {
        auto _ = m_mutex.guard();
        if (nullptr == m_cookieJar)
            m_cookieJar = new CookieJarImpl;
        return m_cookieJar;
    }
    else
    {
        auto _ = m_mutex.guard_shared();
        return m_cookieJar;
    }
}

BkJSContext CrawlerImpl::GetJSContext(void)
{
    return m_frame->script().EnsureContext();
}

void CrawlerImpl::HijackResponse(BkResponse response)
{
    if (nullptr != m_client.HijackResponse)
        m_client.HijackResponse(response, m_client.UserData);
}

bool CrawlerImpl::HijackScript(const char *URL, std::string &dst) const
{
    if (nullptr == m_client.HijackScript)
        return false;
    return m_client.HijackScript(URL, BufferImpl::Wrap(dst), m_client.UserData);
}

void CrawlerImpl::ModifyRequest(const char *URL, BkRequest req)
{
    if (nullptr != m_client.ModifyRequest)
        m_client.ModifyRequest(URL, req, m_client.UserData);
}

bool CrawlerImpl::ProcessRequestComplete(BkResponse response, BkWorkController controller)
{
    if (nullptr == m_client.RequestComplete)
        return false;

    m_client.RequestComplete(response, controller, m_client.UserData);
    return true;
}

BkContext CrawlerImpl::RequireScriptContext(void) const
{
    if (nullptr != m_client.RequireScriptContext)
        return m_client.RequireScriptContext(m_client.UserData);
    else
        return FrameLoaderClient::RequireScriptContext();
}

int CrawlerImpl::Run(const char *URL)
{
    KURL u(URL);
    if (!u.isValid() || !u.scheme_is_in_http_family())
    {
        ASSERT(u.isValid());
        ASSERT(u.scheme_is_in_http_family());
        return BK_ERR_URI;
    }

    ResourceRequest request(u);
    request.SetType(ResourceRequest::Type::MainHTML);
    request.SetCrawler(this);
    m_frame->loader().load(FrameLoadRequest(nullptr, request));
    return BK_ERR_SUCCESS;
}

bool CrawlerImpl::ScriptEnabled(const std::string &URL) const
{
    return nullptr != m_client.IsScriptEnabled
        ? m_client.IsScriptEnabled(URL.c_str(), m_client.UserData)
        : false;
}

void CrawlerImpl::SetCookieJar(CookieJarImpl *cookieJar)
{
    cookieJar->Retain();

    auto _ = m_mutex.guard();
    if (nullptr != m_cookieJar)
        m_cookieJar->Release();
    m_cookieJar = cookieJar;
}

#if 0 // BKTODO:
void CrawlerImpl::TransitionToCommittedForNewPage(void)
{
    // Nothing to do for crawlers.
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT void BKAPI BkEnableCrawlerCookies(BkCrawler crawler, BkCookieJar *cookieJar)
{
    CookieJarImpl *cj = crawler->GetCookieJar(true);
    if (nullptr != cookieJar)
    {
        cj->Retain();
        *cookieJar = cj;
    }
}

BKEXPORT BkCrawler BKAPI BkCreateCrawler(BkCrawlerClient *client)
{
    return new CrawlerImpl(*client);
}

BKEXPORT void BKAPI BkDestroyCrawler(BkCrawler crawler)
{
    crawler->Destroy();
}

BKEXPORT BkJSContext BKAPI BkGetJSContextFromCrawler(BkCrawler crawler)
{
    return crawler->GetJSContext();
}

BKEXPORT void BKAPI BkHijackResponse(BkResponse response, const void *newBody, size_t length)
{
    response->HijackBody(newBody, length);
}

BKEXPORT int BKAPI BkRunCrawler(BkCrawler crawler, const char *URL)
{
    return crawler->Run(URL);
}

BKEXPORT void BKAPI BkSetCookieJar(BkCrawler crawler, BkCookieJar cookieJar)
{
    crawler->SetCookieJar(cookieJar);
}

} // extern "C"
