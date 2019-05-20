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

#include "public/platform/WebHTTPHeaderVisitor.h"

#include "app/app_impl.h"
#include "crawler/crawler_impl.h"
#include "loader_tasks/http_response_task.h"

using namespace blink;

namespace BlinKit {

HTTPLoaderTask::HTTPLoaderTask(CrawlerImpl &crawler, const WebURLRequest &request, WebURLLoaderClient *client)
    : LoaderTask(request.url(), client), m_crawler(crawler), m_method(request.GetHTTPMethod())
{
    request.VisitHTTPHeaderFields([this](const std::string &name, const std::string &value) {
        m_headers[name] = value;
    });
}

int HTTPLoaderTask::LoadRemoteData(void)
{
    HTTPResponseTask *responseTask = new HTTPResponseTask(m_crawler, m_loader, m_client, *m_responseData);
    if (nullptr == responseTask)
    {
        assert(nullptr != responseTask);
        return BkError::UnknownError;
    }

    std::string URL = m_responseData->URI.string().to_string();
    BkRequest *request = AppImpl::Get().CreateRequest(URL.c_str(), *responseTask);
    if (nullptr == request)
    {
        assert(nullptr != request);
        delete responseTask;
        return BkError::UnknownError;
    }

    request->SetMethod(m_method.c_str());
    request->SetHeader("Accept-Encoding", "gzip, deflate");
    request->SetHeader("Connection", "Keep-Alive");
    for (const auto &it : m_headers)
        request->SetHeader(it.first.c_str(), it.second.c_str());

    std::string locale = Platform::current()->defaultLocale().utf8();
    request->SetHeader("Accept-Language", locale.c_str());

    std::string cookie = m_crawler.GetCookie(URL);
    if (!cookie.empty())
    {
        BKLOG("Apply cookie: %s", cookie.c_str());
        request->SetHeader("Cookie", cookie.c_str());
    }

    responseTask->Setup(m_taskRunner);
    return request->Perform();
}

void HTTPLoaderTask::run(void)
{
    int r = LoadRemoteData();
    if (BkError::Success != r)
        ReportErrorToLoader(r);
}

} // namespace BlinKit
