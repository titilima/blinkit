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

#include "base/single_thread_task_runner.h"
#include "bkcommon/buffer_impl.hpp"
#include "bkcommon/controller_impl.h"
#include "bkcommon/response_impl.h"
#include "blinkit/app/app_impl.h"
#include "blinkit/crawler/cookie_jar_impl.h"
#include "blinkit/js/browser_context.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/loader/frame_load_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_error.h"

using namespace blink;
using namespace BlinKit;

CrawlerImpl::CrawlerImpl(const BkCrawlerClient &client, ClientCaller &clientCaller)
    : LocalFrameClientImpl(AppImpl::Get().GetAppCaller(), clientCaller)
    , m_client(client)
    , m_frame(LocalFrame::Create(this))
{
    ASSERT(IsMainThread());
    m_frame->Init();
}

CrawlerImpl::~CrawlerImpl(void)
{
    ASSERT(IsMainThread());
    m_frame->Detach(FrameDetachType::kRemove);
    if (nullptr != m_cookieJar)
        m_cookieJar->Release();
}

bool CrawlerImpl::ApplyConsoleMessager(std::function<void(int, const char *)> &dst) const
{
    if (nullptr == m_client.ConsoleMessage)
        return false;
    dst = std::bind(m_client.ConsoleMessage, std::placeholders::_1, std::placeholders::_2, m_client.UserData);
    return true;
}

int CrawlerImpl::CallJS(BkJSCallback callback, void *userData)
{
    return LocalFrameClientImpl::CallJS(m_frame.get(), callback, userData);
}

void CrawlerImpl::CancelLoading(void)
{
    ASSERT(IsMainThread());
    m_frame->Loader().StopAllLoaders();
}

void CrawlerImpl::Destroy(void)
{
    ASSERT(IsClientThread());
    auto task = std::bind(std::default_delete<CrawlerImpl>(), this);
    m_appCaller.Call(FROM_HERE, std::move(task));
}

void CrawlerImpl::DidFinishLoad(void)
{
    m_client.DocumentReady(m_client.UserData);
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
    m_clientCaller.Post(FROM_HERE, task);
}

bool CrawlerImpl::GetConfig(int cfg, std::string &dst) const
{
    ASSERT(IsMainThread());
    if (nullptr != m_client.GetConfig)
        return m_client.GetConfig(cfg, BufferImpl::Wrap(dst), m_client.UserData);
    return false;
}

CookieJarImpl* CrawlerImpl::GetCookieJar(bool createIfNotExists)
{
    if (createIfNotExists)
    {
        std::unique_lock<BkSharedMutex> lock(m_mutex);
        if (nullptr == m_cookieJar)
            m_cookieJar = new CookieJarImpl;
        return m_cookieJar;
    }
    else
    {
        std::shared_lock<BkSharedMutex> lock(m_mutex);
        return m_cookieJar;
    }
}

std::string CrawlerImpl::GetCookies(const std::string &URL) const
{
    std::string ret;
    {
        std::shared_lock<BkSharedMutex> lock(m_mutex);
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

void CrawlerImpl::ProcessDocumentReset(ContextImpl *ctx)
{
    if (nullptr != m_client.DocumentReset)
        m_client.DocumentReset(ctx, m_client.UserData);
}

bool CrawlerImpl::ProcessRequestComplete(BkResponse response, BkWorkController controller)
{
    if (nullptr == m_client.RequestComplete)
        return false;

    auto task = std::bind(m_client.RequestComplete, response, controller, m_client.UserData);
    m_clientCaller.Post(FROM_HERE, task);
    return true;
}

int CrawlerImpl::Run(const char *URL)
{
    ASSERT(IsClientThread());

    GURL u(URL);
    if (!u.SchemeIsHTTPOrHTTPS())
    {
        BKLOG("Invalid URL: %s", URL);
        return BK_ERR_URI;
    }

    auto task = [this, u]
    {
        FrameLoadRequest request(nullptr, ResourceRequest(u));
        request.GetResourceRequest().SetCrawler(this);
        request.GetResourceRequest().SetHijackType(HijackType::kMainHTML);
        m_frame->Loader().StartNavigation(request);
    };
    m_appCaller.Call(FROM_HERE, std::move(task));
    return BK_ERR_SUCCESS;
}

bool CrawlerImpl::ScriptEnabled(const std::string &URL)
{
    bool ret = false;
    do {
        if (nullptr == m_client.GetScriptMode)
            break;

        auto it = m_scriptModeMap.find(URL);
        if (std::end(m_scriptModeMap) != it)
        {
            ret = it->second;
            break;
        }

        int mode = m_client.GetScriptMode(URL.c_str(), m_client.UserData);
        switch (mode)
        {
            case BK_ALWAYS_ENABLE_SCRIPT:
                m_scriptModeMap[URL] = true;
                [[fallthrough]];
            case BK_ENABLE_SCRIPT_ONCE:
                break;
            case BK_ALWAYS_DISABLE_SCRIPT:
                m_scriptModeMap[URL] = false;
                [[fallthrough]];
            case BK_DISABLE_SCRIPT_ONCE:
                ret = false;
                break;
        }
    } while (false);
    return ret;
}

void CrawlerImpl::SetCookieJar(CookieJarImpl *cookieJar)
{
    cookieJar->Retain();

    std::unique_lock<BkSharedMutex> lock(m_mutex);
    if (nullptr != m_cookieJar)
        m_cookieJar->Release();
    m_cookieJar = cookieJar;
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
        m_client.GetConfig(BK_CFG_USER_AGENT, BufferImpl::Wrap(userAgent), m_client.UserData);
        if (!userAgent.empty())
            return String::FromStdUTF8(userAgent);
    }
    return LocalFrameClientImpl::UserAgent();
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
        ret = new CrawlerImpl(*client, clientCaller);
    };
    app.GetAppCaller().SyncCall(FROM_HERE, task);
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
