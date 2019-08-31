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
#include "loader_tasks/http_loader.h"

using namespace blink;

namespace BlinKit {

HTTPLoaderTask::HTTPLoaderTask(CrawlerImpl &crawler, const WebURLRequest &request, WebURLLoaderClient *client)
    : LoaderTask(request.url(), client)
    , m_httpLoader(new HTTPLoader(crawler, m_loader, m_client, *m_responseData))
{
    m_httpLoader->SetMethod(request.GetHTTPMethod());
    request.VisitHTTPHeaderFields([this](const std::string &name, const std::string &value) {
        m_httpLoader->AddHeader(name, value);
    });
}

void HTTPLoaderTask::run(void)
{
    m_httpLoader->SetURL(m_responseData->URI.string().to_string());
    m_httpLoader->SetTaskRunner(m_taskRunner);

    int r = m_httpLoader->Load();
    if (BkError::Success != r)
    {
        delete m_httpLoader;
        ReportErrorToLoader(r);
    }
}

} // namespace BlinKit
