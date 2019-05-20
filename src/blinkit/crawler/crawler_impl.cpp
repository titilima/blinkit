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

#include "core/frame/LocalFrame.h"
#include "core/loader/FrameLoadRequest.h"

#include "app/app_impl.h"
#include "blink_impl/cookie_jar_impl.h"

#include "js/public/script_controller.h"

using namespace blink;

namespace BlinKit {

CrawlerImpl::CrawlerImpl(BkCrawlerClient &client)
    : m_client(client), m_frame(LocalFrame::create(this, nullptr))
{
    m_frame->init();
}

CrawlerImpl::~CrawlerImpl(void)
{
    m_frame->detach(FrameDetachType::Remove);
}

int BKAPI CrawlerImpl::CallCrawler(const char *method, BkCallerContext::Callback callback, void *userData)
{
    return m_frame->script().CallCrawler(method, callback, userData);
}

int BKAPI CrawlerImpl::CallFunction(const char *name, BkCallerContext::Callback callback, void *userData)
{
    return m_frame->script().CallFunction(name, callback, userData);
}

int BKAPI CrawlerImpl::CreateCrawlerObject(const char *script, size_t length)
{
    if (0 == length)
        length = strlen(script);
    return m_frame->script().CreateCrawlerObject(script, length);
}

void CrawlerImpl::dispatchDidFinishLoad(void)
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

int BKAPI CrawlerImpl::Load(const char *URL)
{
    KURL u(ParsedURLString, URL);
    if (!u.protocolIsInHTTPFamily())
        return BkError::URIError;

    FrameLoadRequest request(nullptr, ResourceRequest(u));
    m_frame->loader().load(request);
    return BkError::Success;
}

int BKAPI CrawlerImpl::RegisterCrawlerFunction(const char *name, BkFunction *functionImpl)
{
    assert(false); // BKTODO:
    return BkError::UnknownError;
}

String CrawlerImpl::userAgent(void)
{
    if (!m_userAgent.empty())
        return String::fromUTF8(m_userAgent.data(), m_userAgent.length());
    return FrameLoaderClientImpl::userAgent();
}

} // namespace BlinKit
