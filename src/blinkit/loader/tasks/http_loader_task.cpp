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
#include "bkcommon/buffer_impl.hpp"
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

HTTPLoaderTask::HTTPLoaderTask(const ResourceRequest &request, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, WebURLLoaderClient *client)
    : LoaderTask(taskRunner, client)
    , m_resourceRequest(request)
{
}

HTTPLoaderTask::~HTTPLoaderTask(void)
{
    if (nullptr != m_response)
        m_response->Release();
}

int HTTPLoaderTask::CancelWork(void)
{
    if (IsMainThread())
        DoCancel();
    else
        m_taskRunner->PostTask(FROM_HERE, std::bind(&HTTPLoaderTask::DoCancel, this));
    return BK_ERR_SUCCESS;
}

void HTTPLoaderTask::CommitHijackedResponse(void)
{
    std::string currentURL;
    m_response->GetData(BK_RESPONSE_CURRENT_URL, BufferImpl::Wrap(currentURL));

    if (CookieJarImpl *cookieJar = m_resourceRequest.Crawler()->GetCookieJar(false))
    {
        size_t n = m_response->CookiesCount();
        for (size_t i = 0; i < n; ++i)
        {
            std::string cookie;
            m_response->GetCookie(i, BufferImpl::Wrap(cookie));
            cookieJar->Set(cookie.c_str(), currentURL.c_str());
        }
    }

    GURL URL(currentURL);
    ResourceResponse resourceResponse(URL);
    PopulateResourceResponse(resourceResponse);

    m_client->DidReceiveResponse(resourceResponse);
    m_client->DidReceiveData(reinterpret_cast<const char *>(m_response->Content()), m_response->ContentLength());
    m_client->DidFinishLoading();

    delete this;
}

int HTTPLoaderTask::ContinueWorking(void)
{
    if (IsMainThread())
        CommitHijackedResponse();
    else
        m_taskRunner->PostTask(FROM_HERE, std::bind(&HTTPLoaderTask::CommitHijackedResponse, this));
    return BK_ERR_SUCCESS;
}

bool HTTPLoaderTask::CreateRequest(const std::string &URL)
{
    ASSERT(nullptr == m_request);

    BkRequestClient client = { 0 };
    client.SizeOfStruct = sizeof(BkRequestClient);
    client.UserData = this;
    client.RequestComplete = RequestCompleteImpl;
    client.RequestFailed = RequestFailedImpl;
    client.RequestRedirect = RequestRedirectImpl;
    m_request = BkCreateRequest(URL.c_str(), &client);
    return nullptr != m_request;
}

void HTTPLoaderTask::DoCancel(void)
{
    m_resourceRequest.Crawler()->CancelLoading();
    delete this;
}

AtomicString HTTPLoaderTask::GetResponseHeader(const AtomicString &name) const
{
    std::string ret;
    int r = m_response->GetHeader(name.StdUtf8().c_str(), BufferImpl::Wrap(ret));
    return BK_ERR_SUCCESS == r ? AtomicString::FromStdUTF8(ret) : g_null_atom;
}

int HTTPLoaderTask::PerformRequest(void)
{
    BkPerformRequestSynchronously(m_request);
    m_request = nullptr;
    return m_errorCode;
}

void HTTPLoaderTask::PopulateHijackedResponse(const std::string &URL, const std::string &hijack)
{
    FakeResponse *response = new FakeResponse(URL);
    switch (m_resourceRequest.GetHijackType())
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
    std::string currentURL;
    m_response->GetData(BK_RESPONSE_CURRENT_URL, BufferImpl::Wrap(currentURL));
    response.SetURL(GURL(currentURL));

    response.SetHTTPStatusCode(m_response->StatusCode());

    std::string contentType;
    m_response->GetHeader(Strings::HttpHeader::ContentType, BufferImpl::Wrap(contentType));

    bool hasCharset = false;
    std::string mimeType, charset;
    net::HttpUtil::ParseContentType(contentType, &mimeType, &charset, &hasCharset, nullptr);
    response.SetMimeType(AtomicString::FromStdUTF8(mimeType));
    if (hasCharset)
        response.SetTextEncodingName(AtomicString::FromStdUTF8(charset));
}

int HTTPLoaderTask::PopulateResponse(ResourceResponse &resourceResponse, std::string_view &body) const
{
    CrawlerImpl *crawler = m_resourceRequest.Crawler();

    switch (m_resourceRequest.GetHijackType())
    {
        case HijackType::kMainHTML:
        {
            if (crawler->ProcessRequestComplete(m_response, const_cast<HTTPLoaderTask *>(this)))
                return BK_ERR_CANCELLED;
            break;
        }

        default:
            ASSERT(HijackType::kScript == m_resourceRequest.GetHijackType());
            crawler->HijackResponse(m_response);
            break;
    }

    PopulateResourceResponse(resourceResponse);
    body = std::string_view(reinterpret_cast<const char *>(m_response->Content()), m_response->ContentLength());
    return BK_ERR_SUCCESS;
}

int HTTPLoaderTask::PreProcess(void)
{
    const std::string &URL = m_resourceRequest.Url().spec();
    if (ProcessHijackRequest(URL))
    {
        m_taskRunner->PostTask(FROM_HERE,
            std::bind(&HTTPLoaderTask::CommitHijackedResponse, this)
        );
        return BK_ERR_CANCELLED;
    }

    if (!CreateRequest(URL))
    {
        ASSERT(false);
        return BK_ERR_UNKNOWN;
    }

    BkSetRequestMethod(m_request, m_resourceRequest.HttpMethod().StdUtf8().c_str());
    for (const auto &it : m_resourceRequest.AllHeaders().GetRawMap())
        BkSetRequestHeader(m_request, it.first.c_str(), it.second.c_str());

    CrawlerImpl *crawler = m_resourceRequest.Crawler();

    std::string cookies = crawler->GetCookies(URL);
    if (!cookies.empty())
        BkSetRequestHeader(m_request, Strings::HttpHeader::Cookie, cookies.c_str());

    BKLOG("// BKTODO: Add body.");

    crawler->ModifyRequest(URL.c_str(), m_request);
    return BK_ERR_SUCCESS;
}

bool HTTPLoaderTask::ProcessHijackRequest(const std::string &URL)
{
    ASSERT(IsMainThread());

    if (HijackType::kScript != m_resourceRequest.GetHijackType())
        return false;

    std::string hijack;
    if (!m_resourceRequest.Crawler()->HijackRequest(URL.c_str(), hijack))
        return false;

    PopulateHijackedResponse(URL, hijack);
    return true;
}

void HTTPLoaderTask::RequestComplete(BkResponse response)
{
    ASSERT(nullptr == m_response);
    m_response = response->Retain();
}

void BKAPI HTTPLoaderTask::RequestCompleteImpl(BkResponse response, void *userData)
{
    reinterpret_cast<HTTPLoaderTask *>(userData)->RequestComplete(response);
}

void HTTPLoaderTask::RequestFailed(int errorCode)
{
    BKLOG("HTTPLoaderTask::RequestFailed: %d.", errorCode);
    m_errorCode = errorCode;
}

void BKAPI HTTPLoaderTask::RequestFailedImpl(int errorCode, void *userData)
{
    reinterpret_cast<HTTPLoaderTask *>(userData)->RequestFailed(errorCode);
}

bool HTTPLoaderTask::RequestRedirect(BkResponse response, BkRequest request)
{
    ASSERT(false); // BKTODO:
#if 0
    size_t n = response->CookiesCount();
    if (n > 0)
    {
        if (CookieJarImpl *cookieJar = m_crawler->GetCookieJar(false))
        {
            std::unique_lock<CookieJarImpl> lock(*cookieJar);
            const char *URL = m_request.Url().spec().c_str();
            for (size_t i = 0; i < n; ++i)
            {
                std::string cookie;
                response->GetCookie(i, BufferImpl::Wrap(cookie));
                cookieJar->Set(cookie.c_str(), URL);
            }

            std::string cookies = cookieJar->Get(URL);
            if (!cookies.empty())
                BkSetRequestHeader(request, Strings::HttpHeader::Cookie, cookies.c_str());
        }
    }
#endif
    return true;
}

bool_t BKAPI HTTPLoaderTask::RequestRedirectImpl(BkResponse response, BkRequest request, void *userData)
{
    return reinterpret_cast<HTTPLoaderTask *>(userData)->RequestRedirect(response, request);
}

} // namespace BlinKit
