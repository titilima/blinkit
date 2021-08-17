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
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/crawler/cookie_jar_impl.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/loader/frame_load_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_error.h"
#endif

using namespace blink;
using namespace BlinKit;

CrawlerImpl::CrawlerImpl(const BkCrawlerClient &client, ClientCaller &clientCaller)
    : FrameLoaderClient(AppImpl::Get().GetAppCaller(), clientCaller)
    , m_client(client)
    // BKTODO: , m_frame(LocalFrame::create(this))
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

int CrawlerImpl::CallJS(BkJSCallback callback, void *userData)
{
    ASSERT(false); // BKTODO: return LocalFrameClientImpl::CallJS(m_frame.get(), callback, userData);
    return BK_ERR_UNKNOWN;
}

void CrawlerImpl::CancelLoading(void)
{
    ASSERT(isMainThread());
    ASSERT(false); // BKTODO: m_frame->Loader().StopAllLoaders();
}

void CrawlerImpl::Destroy(void)
{
    ASSERT(false); // BKTODO:
#if 0
    ASSERT(IsClientThread());
    auto task = std::bind(std::default_delete<CrawlerImpl>(), this);
    m_appCaller.Call(BLINK_FROM_HERE, std::move(task));
#endif
}

#if 0 // BKTODO:
void CrawlerImpl::DidFinishLoad(void)
{
    ScriptController &ctx = m_frame->GetScriptController();
    m_client.DocumentReady(&ctx, m_client.UserData);
}

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

bool CrawlerImpl::GetConfig(int cfg, std::string &dst) const
{
    ASSERT(isMainThread());
    if (nullptr != m_client.GetConfig)
        return m_client.GetConfig(cfg, BufferImpl::Wrap(dst), m_client.UserData);
    return false;
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

std::string CrawlerImpl::GetCookies(const std::string &URL) const
{
    std::string ret;
    {
        auto _ = m_mutex.guard_shared();
        if (nullptr != m_cookieJar)
        {
            std::shared_lock<CookieJarImpl> lock(*m_cookieJar);
            ret = m_cookieJar->Get(URL.c_str());
        }
    }

    if (nullptr != m_client.GetCookies)
    {
        std::string userSetCookies;
        if (m_client.GetCookies(URL.c_str(), ret.c_str(), BufferImpl::Wrap(userSetCookies), m_client.UserData))
            ret = userSetCookies;
    }

    return ret;
}

bool CrawlerImpl::HijackRequest(const char *URL, std::string &dst) const
{
    if (nullptr == m_client.HijackRequest)
        return false;
    return m_client.HijackRequest(URL, BufferImpl::Wrap(dst), m_client.UserData);
}

void CrawlerImpl::HijackResponse(BkResponse response)
{
    if (nullptr != m_client.HijackResponse)
        m_client.HijackResponse(response, m_client.UserData);
}

void CrawlerImpl::ModifyRequest(const char *URL, BkRequest req)
{
    if (nullptr != m_client.ModifyRequest)
        m_client.ModifyRequest(URL, req, m_client.UserData);
}

bool CrawlerImpl::ProcessConsoleMessage(int type, const char *msg)
{
    if (nullptr == m_client.ConsoleMessage)
        return false;
    m_client.ConsoleMessage(type, msg, m_client.UserData);
    return true;
}

bool CrawlerImpl::ProcessRequestComplete(BkResponse response, BkWorkController controller)
{
    if (nullptr == m_client.RequestComplete)
        return false;

    auto task = std::bind(m_client.RequestComplete, response, controller, m_client.UserData);
    ASSERT(false); // BKTODO: m_clientCaller.Post(BLINK_FROM_HERE, task);
    return true;
}

int CrawlerImpl::Run(const char *URL)
{
    ASSERT(false); // BKTODO:
#if 0
    ASSERT(IsClientThread());

    GURL u(URL);
    if (!u.SchemeIsHTTPOrHTTPS())
    {
        BKLOG("Invalid URL: %s", URL);
        return BK_ERR_URI;
    }

    auto task = [this, &u]
    {
        FrameLoadRequest request(nullptr, ResourceRequest(u));

        auto &resourceRequest = request.GetResourceRequest();
        resourceRequest.SetCrawler(this);
        resourceRequest.SetHijackType(HijackType::kMainHTML);

        m_dirty = true;
        m_frame->Loader().StartNavigation(request);
    };
    m_appCaller.Call(FROM_HERE, std::move(task));
#endif
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

String CrawlerImpl::userAgent(void)
{
    if (nullptr != m_client.GetConfig)
    {
        std::string userAgent;
        m_client.GetConfig(BK_CFG_USER_AGENT, BufferImpl::Wrap(userAgent), m_client.UserData);
        if (!userAgent.empty())
            return String::fromStdUTF8(userAgent);
    }
    return FrameLoaderClient::userAgent();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkCrawlerCallJS(BkCrawler crawler, BkJSCallback callback, void *userData)
{
    return crawler->CallJS(callback, userData);
}

BKEXPORT void BKAPI BkCrawlerEnableCookies(BkCrawler crawler, BkCookieJar *cookieJar)
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
    AppImpl &app = AppImpl::Get();

    ClientCaller &clientCaller = app.AcquireCallerForClient();

    CrawlerImpl *ret = nullptr;
    auto task = [&ret, client, &clientCaller]
    {
        ASSERT(false); // BKTODO: ret = new CrawlerImpl(*client, clientCaller);
    };
    app.GetAppCaller().SyncCall(BLINK_FROM_HERE, task);
    return ret;
}

BKEXPORT void BKAPI BkDestroyCrawler(BkCrawler crawler)
{
    crawler->Destroy();
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
