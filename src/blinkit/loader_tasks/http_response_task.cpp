// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: http_response_task.cpp
// Description: HTTPResponseTask Class
//      Author: Ziming Li
//     Created: 2019-03-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "http_response_task.h"

#include "public/platform/WebTraceLocation.h"

using namespace blink;

namespace BlinKit {

HTTPResponseTask::HTTPResponseTask(CrawlerImpl &crawler, WebURLLoader *loader, WebURLLoaderClient *client, ResponseData &responseData)
    : ResponseTask(loader, client, responseData), m_crawler(crawler)
{
    // Nothing
}

void BKAPI HTTPResponseTask::RequestComplete(const BkResponse &response)
{
    m_responseData->StatusCode = response.StatusCode();
    response.GetHeader("Content-Type", BkMakeBuffer(m_responseData->MimeType).Wrap());
    response.GetBody(BkMakeBuffer(m_responseData->Body).Wrap());

    m_taskRunner->postTask(BLINK_FROM_HERE, this);
}

void BKAPI HTTPResponseTask::RequestFailed(int errorCode)
{
    assert(false); // BKTODO:
}

} // namespace BlinKit
