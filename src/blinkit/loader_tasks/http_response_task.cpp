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

#include "app/app_impl.h"
#include "blink_impl/cookie_jar_impl.h"
#include "crawler/crawler_impl.h"

using namespace blink;

namespace BlinKit {

HTTPResponseTask::HTTPResponseTask(CrawlerImpl &crawler, WebURLLoader *loader, WebURLLoaderClient *client, ResponseData &responseData)
    : ResponseTask(loader, client, responseData), m_crawler(crawler)
{
    // Nothing
}

void BKAPI HTTPResponseTask::Cancel(void)
{
    m_crawler.CancelLoading();
}

void BKAPI HTTPResponseTask::Continue(void)
{
    m_taskRunner->postTask(BLINK_FROM_HERE, this);
}

unsigned BKAPI HTTPResponseTask::CookiesCount(void) const
{
    return m_cookies.size();
}

int BKAPI HTTPResponseTask::GetBody(BkBuffer &body) const
{
    const auto &src = m_responseData->Body;
    body.Assign(src.data(), src.size());
    return BkError::Success;
}

int BKAPI HTTPResponseTask::GetCookie(unsigned i, BkBuffer &cookie) const
{
    if (m_cookies.size() <= i)
    {
        assert(i < m_cookies.size());
        return BkError::NotFound;
    }

    cookie.Assign(m_cookies.at(i));
    return BkError::Success;
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
    response.GetHeader("Content-Type", BkMakeBuffer(m_responseData->ContentType).Wrap());
    response.GetBody(BkMakeBuffer(m_responseData->Body).Wrap());

    const unsigned cookiesCount = response.CookiesCount();
    for (unsigned i = 0; i < cookiesCount; ++i)
    {
        std::string cookie;
        response.GetCookie(i, BkMakeBuffer(cookie).Wrap());
        m_cookies.push_back(cookie);
    }

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

void HTTPResponseTask::run(void)
{
    BkCrawlerClient &client = m_crawler.Client();
    CookieJarImpl &cookieJar = AppImpl::Get().CookieJar();

    for (const std::string &cookie : m_cookies)
    {
        if (!client.SetCookie(cookie.c_str()))
            cookieJar.AddCookieEntry(m_currentURL, cookie);
    }

    ResponseTask::run();
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
