// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_impl.h
// Description: CrawlerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CRAWLER_IMPL_H
#define BLINKIT_BLINKIT_CRAWLER_IMPL_H

#pragma once

#include "sdk/include/BlinKit.h"

#include "browser/browser_impl.h"

namespace BlinKit {

class CrawlerImpl final : public BkCrawler, public BrowserImpl
{
public:
    CrawlerImpl(BkCrawlerClient &client);
private:
    // BkCrawler
    void BKAPI Destroy(void) override { delete this; }
    int BKAPI Load(const char *URI) override;
    void BKAPI SetUserAgent(const char *userAgent) override { m_userAgent = userAgent; }
    // blink::WebFrameClient
    BlinKit::CrawlerImpl* GetCrawler(void) { return this; }
    void didFinishLoad(blink::WebLocalFrame *frame) override;
    blink::WebString userAgentOverride(blink::WebLocalFrame *frame) override;

    BkCrawlerClient &m_client;
    std::string m_userAgent;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_IMPL_H
