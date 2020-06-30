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

#include "bkcommon/controller_impl.h"
#include "bkcommon/response_impl.h"
#include "blinkit/crawler/cookie_jar_impl.h"
#include "blinkit/js/context_impl.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/loader/frame_load_request.h"
#include "third_party/blink/renderer/platform/bindings/gc_pool.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_error.h"
#include "url/gurl.h"

using namespace blink;
using namespace BlinKit;

CrawlerImpl::CrawlerImpl(const BkCrawlerClient &client) : m_client(client), m_frame(LocalFrame::Create(this))
{
    m_frame->Init();
}

CrawlerImpl::~CrawlerImpl(void)
{
    m_frame->Detach(FrameDetachType::kRemove);
    if (nullptr != m_cookieJar)
    {
        if (BK_CJM_OWNED == m_cookieJarMode)
            delete m_cookieJar;
    }
}

bool CrawlerImpl::ApplyConsoleMessager(std::function<void(int, const char *)> &dst) const
{
    if (nullptr == m_client.ConsoleMessage)
        return false;
    dst = std::bind(m_client.ConsoleMessage, std::placeholders::_1, std::placeholders::_2, m_client.UserData);
    return true;
}

void CrawlerImpl::ApplyProxyToRequest(BkRequest req)
{
    if (BK_PROXY_RESERVED == m_proxyType)
    {
        if (nullptr == m_client.GetConfig)
            m_proxyType = BK_PROXY_SYSTEM_DEFAULT;
        else if (GetConfig(BK_CFG_REQUEST_PROXY, m_proxy))
            m_proxyType = m_proxy.empty() ? BK_PROXY_SYSTEM_DEFAULT : BK_PROXY_USER_SPECIFIED;
        else
            m_proxyType = BK_PROXY_DIRECT;
    }

    if (BK_PROXY_DIRECT == m_proxyType)
        return;

    BkSetRequestProxy(req, m_proxyType, m_proxy.c_str());
}

void CrawlerImpl::CancelLoading(void)
{
    m_frame->Loader().StopAllLoaders();
}

void CrawlerImpl::DispatchDidFailProvisionalLoad(const ResourceError &error)
{
    const std::string URL = error.FailingURL();
    m_client.Error(error.ErrorCode(), URL.c_str(), m_client.UserData);
}

void CrawlerImpl::DispatchDidFinishLoad(void)
{
    m_frame->GetGCPool().CollectGarbage();
    m_client.DocumentReady(m_client.UserData);
}

bool CrawlerImpl::GetConfig(int cfg, std::string &dst) const
{
    if (nullptr != m_client.GetConfig)
        return m_client.GetConfig(cfg, BkMakeBuffer(dst), m_client.UserData);
    return false;
}

std::string CrawlerImpl::GetCookies(const std::string &URL) const
{
    std::string ret;

    if (nullptr != m_cookieJar)
        ret = m_cookieJar->Get(URL.c_str());

    if (nullptr != m_client.GetCookies)
    {
        std::string userSetCookies;
        if (m_client.GetCookies(URL.c_str(), ret.c_str(), BkMakeBuffer(userSetCookies), m_client.UserData))
            ret = userSetCookies;
    }

    return ret;
}

BkJSContext CrawlerImpl::GetScriptContext(void)
{
    return &(m_frame->GetScriptController().EnsureContext());
}

bool CrawlerImpl::HijackRequest(const char *URL, std::string &dst) const
{
    if (nullptr == m_client.HijackRequest)
        return false;
    return m_client.HijackRequest(URL, BkMakeBuffer(dst), m_client.UserData);
}

void CrawlerImpl::HijackResponse(BkResponse response)
{
    if (nullptr != m_client.HijackResponse)
        m_client.HijackResponse(response, m_client.UserData);
}

void CrawlerImpl::ProcessDocumentReset(void)
{
    if (nullptr != m_client.DocumentReset)
        m_client.DocumentReset(m_client.UserData);
}

void CrawlerImpl::ProcessRequestComplete(BkResponse response, BkWorkController controller)
{
    if (nullptr != m_client.RequestComplete)
        m_client.RequestComplete(response, controller, m_client.UserData);
    else
        controller->ContinueWorking();
}

int CrawlerImpl::Run(const char *URL)
{
    GURL u(URL);
    if (!u.SchemeIsHTTPOrHTTPS())
    {
        BKLOG("Invalid URL: %s", URL);
        return BK_ERR_URI;
    }

    FrameLoadRequest request(nullptr, ResourceRequest(u));
    request.GetResourceRequest().SetCrawler(this);
    request.GetResourceRequest().SetHijackType(HijackType::kMainHTML);
    m_frame->Loader().StartNavigation(request);
    return BK_ERR_SUCCESS;
}

void CrawlerImpl::SetCookieJar(CookieJarImpl *cookieJar, int mode)
{
    if (nullptr != m_cookieJar)
    {
        if (BK_CJM_OWNED == m_cookieJarMode)
            delete m_cookieJar;
    }

    m_cookieJar = cookieJar;
    m_cookieJarMode = mode;
}

void CrawlerImpl::TransitionToCommittedForNewPage(void)
{
    // Nothing to do for crawlers.
}

String CrawlerImpl::UserAgent(void)
{
    if (nullptr != m_client.GetConfig)
    {
        std::string userAgent;
        m_client.GetConfig(BK_CFG_USER_AGENT, BkMakeBuffer(userAgent), m_client.UserData);
        if (!userAgent.empty())
            return String::FromStdUTF8(userAgent);
    }
    return LocalFrameClientImpl::UserAgent();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkCrawler BKAPI BkCreateCrawler(BkCrawlerClient *client)
{
    return new CrawlerImpl(*client);
}

BKEXPORT void BKAPI BkDestroyCrawler(BkCrawler crawler)
{
    delete crawler;
}

BKEXPORT BkJSContext BKAPI BkGetScriptContextFromCrawler(BkCrawler crawler)
{
    return crawler->GetScriptContext();
}

BKEXPORT void BKAPI BkHijackResponse(BkResponse response, const void *newBody, size_t length)
{
    response->HijackBody(newBody, length);
}

BKEXPORT int BKAPI BkRunCrawler(BkCrawler crawler, const char *URL)
{
    return crawler->Run(URL);
}

BKEXPORT void BKAPI BkSetCookieJar(BkCrawler crawler, BkCookieJar cookieJar, int mode)
{
    crawler->SetCookieJar(cookieJar, mode);
}

} // extern "C"
