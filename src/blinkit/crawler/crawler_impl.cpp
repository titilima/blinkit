// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_impl.cpp
// Description: CrawlerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "crawler_impl.h"

#include "platform/weborigin/KURL.h"

#include "app/app_impl.h"
#include "blink_impl/cookie_jar_impl.h"

using namespace blink;

namespace BlinKit {

CrawlerImpl::CrawlerImpl(BkCrawlerClient &client) : m_client(client)
{
    // Nothing
}

void CrawlerImpl::didFinishLoad(WebLocalFrame *)
{
    m_client.DocumentReady(this);
}

std::string CrawlerImpl::GetCookie(const std::string &URL) const
{
    std::string ret;
    m_client.GetCookie(URL.c_str(), BkMakeBuffer(ret).Wrap());
    if (ret.empty())
        ret = AppImpl::Get().CookieJar().GetCookie(URL);
    return ret;
}

int BKAPI CrawlerImpl::Load(const char *URI)
{
    KURL u(ParsedURLString, URI);
    if (!u.protocolIsInHTTPFamily())
        return BkError::URIError;
    return BrowserImpl::Load(u);
}

WebString CrawlerImpl::userAgentOverride(WebLocalFrame *)
{
    return m_userAgent.empty() ? WebString() : WebString::fromUTF8(m_userAgent);
}

} // namespace BlinKit
