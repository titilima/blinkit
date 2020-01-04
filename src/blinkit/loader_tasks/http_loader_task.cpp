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

#include "base/single_thread_task_runner.h"
#include "blinkit/crawler/crawler_impl.h"
#include "blinkit/http/request_impl.h"
#include "blinkit/http/response_impl.h"
#include "net/http/http_util.h"
#include "third_party/blink/public/platform/web_url_loader_client.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
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

HTTPLoaderTask::~HTTPLoaderTask(void) = default;

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

void HTTPLoaderTask::DoCancel(void)
{
    ASSERT(IsMainThread());
    ASSERT(false); // BKTODO:
}

void HTTPLoaderTask::DoContinue(void)
{
    ASSERT(IsMainThread());

    ResourceResponse response;
    PopulateResourceResponse(response);
    m_client->DidReceiveResponse(response);
    m_client->DidReceiveData(m_response->BodyData(), m_response->BodyLength());
    m_client->DidFinishLoading();
    delete this;
}

AtomicString HTTPLoaderTask::GetResponseHeader(const AtomicString &name) const
{
    std::string ret = m_response->Headers().Get(name.StdUtf8());
    return ret.empty() ? g_null_atom : AtomicString::FromStdUTF8(ret);
}

void HTTPLoaderTask::PopulateResourceResponse(ResourceResponse &response) const
{
    response.SetHTTPStatusCode(m_response->StatusCode());

    bool hasCharset = false;
    std::string mimeType, charset;
    const std::string contentType = m_response->Headers().Get(http_names::kContentType.StdUtf8());
    net::HttpUtil::ParseContentType(contentType, &mimeType, &charset, &hasCharset, nullptr);
    response.SetMimeType(AtomicString::FromStdUTF8(mimeType));
    if (hasCharset)
        response.SetTextEncodingName(AtomicString::FromStdUTF8(charset));
}

void HTTPLoaderTask::ProcessRequestComplete(void)
{
    ASSERT(m_response);

    m_callingCrawler = true;
    m_crawler->ProcessRequestComplete(m_response.get(), this);
    m_callingCrawler = false;

    if (!m_cancel.has_value())
        return;

    if (m_cancel.value())
        DoCancel();
    else
        DoContinue();
}

void HTTPLoaderTask::RequestComplete(BkResponse response)
{
    m_response = response->shared_from_this();

    std::function<void()> callback = std::bind(&HTTPLoaderTask::ProcessRequestComplete, this);
    m_taskRunner->PostTask(FROM_HERE, callback);
}

void HTTPLoaderTask::RequestFailed(int errorCode)
{
    BKLOG("HTTPLoaderTask::RequestFailed: %d.", errorCode);
    LoaderTask::ReportError(m_taskRunner.get(), errorCode);
    delete this;
}

int HTTPLoaderTask::Run(const ResourceRequest &request)
{
    const std::string URL = request.Url().AsString();

    BkRequest req = BkCreateRequest(URL.c_str(), *this);
    if (nullptr == req)
    {
        ASSERT(nullptr != req);
        return BK_ERR_UNKNOWN;
    }

    req->SetMethod(request.HttpMethod().StdUtf8());
    req->SetHeaders(request.AllHeaders());
    BKLOG("// BKTODO: Add body.");

    int r = req->Perform();
    if (BK_ERR_SUCCESS != r)
    {
        ASSERT(BK_ERR_SUCCESS == r);
        delete req;
    }
    return r;
}

} // namespace BlinKit
