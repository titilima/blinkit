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

#include "crawler/crawler_impl.h"

using namespace blink;

namespace BlinKit {

HTTPLoaderTask::HTTPLoaderTask(CrawlerImpl &crawler, const KURL &URI, WebURLLoaderClient *client)
    : LoaderTask(URI, client), m_crawler(crawler)
{
    assert(false); // BKTODO:
}

void BKAPI HTTPLoaderTask::RequestComplete(const BkResponse &response)
{
    assert(false); // BKTODO:
}

void BKAPI HTTPLoaderTask::RequestFailed(int errorCode)
{
    assert(false); // BKTODO:
}

void HTTPLoaderTask::run(void)
{
    assert(false); // BKTODO:
}

} // namespace BlinKit
