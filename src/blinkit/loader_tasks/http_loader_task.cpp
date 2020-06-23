// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: http_loader_task.cpp
// Description: HTTPLoaderTask Class
//      Author: Ziming Li
//     Created: 2019-03-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "http_loader_task.h"

#include "base/auto_reset.h"
#include "base/single_thread_task_runner.h"
#include "bkcommon/bk_strings.h"
#include "bkcommon/response_impl.h"
#include "blinkit/crawler/cookie_jar_impl.h"
#include "blinkit/crawler/crawler_impl.h"
#include "blinkit/crawler/hijack_response.h"
#include "net/http/http_util.h"
#include "third_party/blink/public/platform/web_url_loader_client.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_response.h"
#include "third_party/blink/renderer/platform/network/http_names.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace blink;

namespace BlinKit {

HTTPLoaderTask::HTTPLoaderTask(BkCrawler crawler, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTask(taskRunner, client)
    , m_crawler(crawler)
{
}

HTTPLoaderTask::~HTTPLoaderTask(void)
{
    if (nullptr != m_response)
        m_response->Release();
}

int HTTPLoaderTask::CancelWork(void)
{
    if (m_callingCrawler)
        m_cancel = true;
    else
        DoCancel();
    return BK_ERR_SUCCESS;
}

int HTTPLoaderTask::ContinueWorking(void)
{
    if (m_callingCrawler)
        m_cancel = false;
    else
        DoContinue();
    return BK_ERR_SUCCESS;
}

BkRequest HTTPLoaderTask::CreateRequest(const std::string &URL)
{
    BkRequestClient client = { 0 };
    client.SizeOfStruct = sizeof(BkRequestClient);
    client.UserData = this;
    client.RequestComplete = RequestCompleteImpl;
    client.RequestFailed = RequestFailedImpl;
    client.RequestRedirect = RequestRedirectImpl;
    return BkCreateRequest(URL.c_str(), &client);
}

void HTTPLoaderTask::DoCancel(void)
{
    ASSERT(IsMainThread());
    m_crawler->CancelLoading();
}

void HTTPLoaderTask::DoContinue(void)
{
    ASSERT(IsMainThread());

    std::string currentURL;
    m_response->GetData(BK_RESPONSE_CURRENT_URL, BkMakeBuffer(currentURL));

    if (CookieJarImpl *cookieJar = m_crawler->GetCookieJar())
    {
        size_t n = m_response->CookiesCount();
        for (size_t i = 0; i < n; ++i)
        {
            std::string cookie;
            m_response->GetCookie(i, BkMakeBuffer(cookie));
            cookieJar->Set(cookie.c_str(), currentURL.c_str());
        }
    }

    GURL u(currentURL);
    ResourceResponse response(u);
    PopulateResourceResponse(response);
    m_client->DidReceiveResponse(response);
    m_client->DidReceiveData(reinterpret_cast<const char *>(m_response->Content()), m_response->ContentLength());

    m_client->DidFinishLoading();
    delete this;
}

AtomicString HTTPLoaderTask::GetResponseHeader(const AtomicString &name) const
{
    std::string ret;
    int r = m_response->GetHeader(name.StdUtf8().c_str(), BkMakeBuffer(ret));
    return BK_ERR_SUCCESS == r ? AtomicString::FromStdUTF8(ret) : g_null_atom;
}

void HTTPLoaderTask::PopulateHijackedResponse(const std::string &URL, const std::string &hijack)
{
    FakeResponse *response = new FakeResponse(URL);
    switch (m_hijackType)
    {
        case HijackType::kScript:
            response->SetContentType("application/javascript; charset=utf-8");
            break;
        default:
            NOTREACHED();
    }
    response->HijackBody(hijack.data(), hijack.length());

    ASSERT(nullptr == m_response);
    m_response = response;
}

void HTTPLoaderTask::PopulateResourceResponse(ResourceResponse &response) const
{
    response.SetHTTPStatusCode(m_response->StatusCode());

    std::string contentType;
    m_response->GetHeader(Strings::HttpHeader::ContentType, BkMakeBuffer(contentType));

    bool hasCharset = false;
    std::string mimeType, charset;
    net::HttpUtil::ParseContentType(contentType, &mimeType, &charset, &hasCharset, nullptr);
    response.SetMimeType(AtomicString::FromStdUTF8(mimeType));
    if (hasCharset)
        response.SetTextEncodingName(AtomicString::FromStdUTF8(charset));
}

bool HTTPLoaderTask::ProcessHijackRequest(const std::string &URL)
{
    if (HijackType::kScript != m_hijackType)
        return false;

    std::string hijack;
    if (!m_crawler->HijackRequest(URL.c_str(), hijack))
        return false;

    PopulateHijackedResponse(URL, hijack);
    return true;
}

bool HTTPLoaderTask::ProcessHijackResponse(void)
{
    if (HijackType::kMainHTML == m_hijackType)
        return false;
    m_crawler->HijackResponse(m_response);
    return true;
}

void HTTPLoaderTask::ProcessRequestComplete(void)
{
    ASSERT(m_response);
    do {
        if (ProcessHijackResponse())
            break;

        {
            base::AutoReset callingCrawler(&m_callingCrawler, true);
            m_crawler->ProcessRequestComplete(m_response, this);
        }
        if (!m_cancel.has_value())
            return;

        if (!m_cancel.value())
            break;

        DoCancel();
        return;
    } while (false);
    DoContinue();
}

void HTTPLoaderTask::RequestComplete(BkResponse response)
{
    ASSERT(nullptr == m_response);
    m_response = response->Retain();

    std::function<void()> callback = std::bind(&HTTPLoaderTask::ProcessRequestComplete, this);
    m_taskRunner->PostTask(FROM_HERE, callback);
}

void BKAPI HTTPLoaderTask::RequestCompleteImpl(BkResponse response, void *userData)
{
    reinterpret_cast<HTTPLoaderTask *>(userData)->RequestComplete(response);
}

void HTTPLoaderTask::RequestFailed(int errorCode)
{
    BKLOG("HTTPLoaderTask::RequestFailed: %d.", errorCode);
    LoaderTask::ReportError(m_client, m_taskRunner.get(), errorCode, m_url);
    delete this;
}

void BKAPI HTTPLoaderTask::RequestFailedImpl(int errorCode, void *userData)
{
    reinterpret_cast<HTTPLoaderTask *>(userData)->RequestFailed(errorCode);
}

int HTTPLoaderTask::Run(const ResourceRequest &request)
{
    m_url = request.Url();
    m_hijackType = request.GetHijackType();

    const std::string URL = m_url.spec();
    if (ProcessHijackRequest(URL))
    {
        std::function<void()> callback = std::bind(&HTTPLoaderTask::DoContinue, this);
        m_taskRunner->PostTask(FROM_HERE, callback);
        return BK_ERR_SUCCESS;
    }

    BkRequest req = CreateRequest(URL);
    if (nullptr == req)
    {
        ASSERT(nullptr != req);
        return BK_ERR_UNKNOWN;
    }

    m_crawler->ApplyProxyToRequest(req);
    BkSetRequestMethod(req, request.HttpMethod().StdUtf8().c_str());
    for (const auto &it : request.AllHeaders().GetRawMap())
        BkSetRequestHeader(req, it.first.c_str(), it.second.c_str());

    std::string cookies = m_crawler->GetCookies(URL);
    if (!cookies.empty())
        BkSetRequestHeader(req, Strings::HttpHeader::Cookie, cookies.c_str());

    BKLOG("// BKTODO: Add body.");

    int r = BkPerformRequest(req, nullptr);
    ASSERT(BK_ERR_SUCCESS == r);
    return r;
}

} // namespace BlinKit
