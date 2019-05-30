// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: loader_task.cpp
// Description: LoaderTask Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "loader_task.h"

#include "public/platform/WebURLRequest.h"
#include "public/platform/WebTraceLocation.h"
#include "public/platform/WebURLLoaderClient.h"
#include "loader_tasks/file_loader_task.h"
#include "loader_tasks/http_loader_task.h"
#include "loader_tasks/res_loader_task.h"
#include "loader_tasks/response_task.h"

using namespace blink;

namespace BlinKit {

LoaderTask::LoaderTask(const KURL &URI, WebURLLoaderClient *client)
    : m_client(client), m_responseData(std::make_shared<ResponseData>())
{
    m_responseData->URI = URI;
}

LoaderTask* LoaderTask::Create(const WebURLRequest &request, WebURLLoaderClient *client)
{
    const KURL URI = request.url();

    CrawlerImpl *crawler = client->Crawler();
    if (nullptr != crawler)
    {
        if (!URI.protocolIsInHTTPFamily())
        {
            assert(URI.protocolIsInHTTPFamily());
            return nullptr;
        }
        return new HTTPLoaderTask(*crawler, request, client);
    }

#ifndef BLINKIT_CRAWLER_ONLY
    if (URI.isLocalFile())
        return new FileLoaderTask(URI, client);
    if (URI.protocolIs("res"))
        return new ResLoaderTask(URI, client);
#endif

    assert(false); // BKTODO:
    return nullptr;
}

void LoaderTask::ReportErrorToLoader(int errorCode)
{
    assert(false); // BKTODO:
}

void LoaderTask::RespondToLoader(void)
{
    m_taskRunner->postTask(BLINK_FROM_HERE, new ResponseTask(m_loader, m_client, *m_responseData));
}

} // namespace BlinKit
