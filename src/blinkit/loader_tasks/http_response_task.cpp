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

#include "crawler/crawler_impl.h"

#include "platform/network/HTTPParsers.h"
#include "public/platform/WebTraceLocation.h"

using namespace blink;

namespace BlinKit {

HTTPResponseTask::HTTPResponseTask(CrawlerImpl &crawler, WebURLLoader *loader, WebURLLoaderClient *client, ResponseData &responseData)
    : ResponseTask(loader, client, responseData), m_crawler(crawler)
{
    // Nothing
}

void BKAPI HTTPResponseTask::Continue(void)
{
    m_taskRunner->postTask(BLINK_FROM_HERE, this);
}

unsigned BKAPI HTTPResponseTask::CookiesCount(void) const
{
    assert(false); // BKTODO:
    return 0;
}

static std::string ExtractMIMEType(const BkResponse &response)
{
    std::string contentType;
    response.GetHeader("Content-Type", BkMakeBuffer(contentType).Wrap());

    AtomicString mediaType = AtomicString::fromUTF8(contentType.data(), contentType.length());
    return extractMIMETypeFromMediaType(mediaType).lower().to_string();
}

int BKAPI HTTPResponseTask::GetBody(BkBuffer &body) const
{
    const auto &src = m_responseData->Body;
    body.Assign(src.data(), src.size());
    return BkError::Success;
}

int BKAPI HTTPResponseTask::GetCookie(unsigned i, BkBuffer &cookie) const
{
    assert(false); // BKTODO:
    return BkError::Forbidden;
}

int BKAPI HTTPResponseTask::GetCurrentURL(BkBuffer &URL) const
{
    URL.Assign(m_currentURL);
    return BkError::Success;
}

int BKAPI HTTPResponseTask::GetHeader(const char *name, BkBuffer &value) const
{
    assert(false); // BKTODO:
    return BkError::Forbidden;
}

void BKAPI HTTPResponseTask::RequestComplete(const BkResponse &response)
{
    response.GetCurrentURL(BkMakeBuffer(m_currentURL).Wrap());
    m_responseData->StatusCode = response.StatusCode();
    m_responseData->MimeType = ExtractMIMEType(response);
    response.GetBody(BkMakeBuffer(m_responseData->Body).Wrap());

    const auto callback = [this]()
    {
        m_crawler.Client().RequestComplete(&m_crawler, this);
    };
    m_taskRunner->postTask(BLINK_FROM_HERE, callback);
}

void BKAPI HTTPResponseTask::RequestFailed(int errorCode)
{
    BkCrawlerClient *client = &(m_crawler.Client());
    m_taskRunner->postTask(BLINK_FROM_HERE, [client, errorCode] {
        client->RequestFailed(errorCode);
    });
    delete this;
}

void BKAPI HTTPResponseTask::SetBody(const char *body, size_t length)
{
    auto &dst = m_responseData->Body;
    dst.resize(length);
    if (length > 0)
        memcpy(dst.data(), body, length);
}

int BKAPI HTTPResponseTask::StatusCode(void) const
{
    return m_responseData->StatusCode;
}

} // namespace BlinKit
