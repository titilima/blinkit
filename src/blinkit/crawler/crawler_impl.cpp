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

#include "core/loader/FrameLoadRequest.h"

#include "app/app_impl.h"
#include "blink_impl/cookie_jar_impl.h"
#include "crawler/crawler_frame.h"

using namespace blink;

namespace BlinKit {

CrawlerImpl::CrawlerImpl(BkCrawlerClient &client)
    : m_client(client), m_frame(CrawlerFrame::Create(this))
{
    m_frame->init();
}

CrawlerImpl::~CrawlerImpl(void)
{
    // Nothing
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

    FrameLoadRequest request(nullptr, ResourceRequest(u));
    m_frame->loader().load(request);
    return BkError::Success;
}

String CrawlerImpl::userAgent(void)
{
    if (!m_userAgent.empty())
        return String::fromUTF8(m_userAgent.data(), m_userAgent.length());
    return FrameLoaderClientImpl::userAgent();
}

} // namespace BlinKit
