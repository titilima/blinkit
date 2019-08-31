// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: http_loader.cpp
// Description: HTTPLoader Class
//      Author: Ziming Li
//     Created: 2019-08-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "http_loader.h"

#include "app/app_impl.h"
#include "blink_impl/cookie_jar_impl.h"
#include "crawler/crawler_impl.h"

using namespace blink;

namespace BlinKit {

HTTPLoader::HTTPLoader(CrawlerImpl &crawler, WebURLLoader *loader, WebURLLoaderClient *client, ResponseData &responseData)
    : ResponseTask(loader, client, responseData), m_crawler(crawler)
{
    AddHeader("Accept-Encoding", "gzip, deflate");
    AddHeader("Accept-Language", Platform::current()->defaultLocale().utf8());
    AddHeader("Connection", "Keep-Alive");
}

void HTTPLoader::ApplyCookies(const BkResponse &response)
{
    BkCrawlerClient &client = m_crawler.Client();
    CookieJarImpl &cookieJar = AppImpl::Get().CookieJar();

    unsigned n = response.CookiesCount();
    for (unsigned i = 0; i < n; ++i)
    {
        std::string cookie;
        response.GetCookie(i, BkMakeBuffer(cookie).Wrap());
        m_cookies.push_back(cookie);

        cookieJar.AddCookieEntry(m_currentURL, cookie);

        client.SetCookie(cookie.c_str());
    }
}

void BKAPI HTTPLoader::Cancel(void)
{
    m_crawler.CancelLoading();
}

void BKAPI HTTPLoader::Continue(void)
{
    m_taskRunner->postTask(BLINK_FROM_HERE, this);
}


unsigned BKAPI HTTPLoader::CookiesCount(void) const
{
    return m_cookies.size();
}

int BKAPI HTTPLoader::GetBody(BkBuffer &body) const
{
    const auto &src = m_responseData->Body;
    body.Assign(src.data(), src.size());
    return BkError::Success;
}

int BKAPI HTTPLoader::GetCookie(unsigned i, BkBuffer &cookie) const
{
    if (m_cookies.size() <= i)
    {
        assert(i < m_cookies.size());
        return BkError::NotFound;
    }

    cookie.Assign(m_cookies.at(i));
    return BkError::Success;
}

int BKAPI HTTPLoader::GetCurrentURL(BkBuffer &URL) const
{
    URL.Assign(m_currentURL);
    return BkError::Success;
}

int BKAPI HTTPLoader::GetHeader(const char *name, BkBuffer &value) const
{
    assert(false); // BKTODO:
    return BkError::Forbidden;
}

int HTTPLoader::Load(void)
{
    BkRequest *request = AppImpl::Get().CreateRequest(m_currentURL.c_str(), *this);
    if (nullptr == request)
        return BkError::UnknownError;

    request->SetMethod(m_method.c_str());
    for (const auto &it : m_headers)
        request->SetHeader(it.first.c_str(), it.second.c_str());

    std::string cookie = m_crawler.GetCookies(m_currentURL);
    if (!cookie.empty())
    {
        BKLOG("Apply cookie: %s", cookie.c_str());
        request->SetHeader("Cookie", cookie.c_str());
    }

    return request->Perform();
}

void BKAPI HTTPLoader::RequestComplete(const BkResponse &response)
{
    ApplyCookies(response);

    response.GetCurrentURL(BkMakeBuffer(m_currentURL).Wrap());
    m_responseData->StatusCode = response.StatusCode();
    response.GetHeader("Content-Type", BkMakeBuffer(m_responseData->ContentType).Wrap());
    response.GetBody(BkMakeBuffer(m_responseData->Body).Wrap());

    const auto callback = [this]()
    {
        m_crawler.Client().RequestComplete(&m_crawler, this);
    };
    m_taskRunner->postTask(BLINK_FROM_HERE, callback);
}

void BKAPI HTTPLoader::RequestFailed(int errorCode)
{
    m_taskRunner->postTask(BLINK_FROM_HERE, CreateErrorTask(errorCode));
    delete this;
}

bool BKAPI HTTPLoader::RequestRedirect(const BkResponse &response)
{
    ApplyCookies(response);

    AddHeader("Referer", m_currentURL);
    response.GetCurrentURL(BkMakeBuffer(m_currentURL).Wrap());

    int r = Load();
    if (BkError::Success != r)
        RequestFailed(r);

    return false;
}

void BKAPI HTTPLoader::SetBody(const char *body, size_t length)
{
    auto &dst = m_responseData->Body;
    dst.resize(length);
    if (length > 0)
        memcpy(dst.data(), body, length);
}

int BKAPI HTTPLoader::StatusCode(void) const
{
    return m_responseData->StatusCode;
}

} // namespace BlinKit
