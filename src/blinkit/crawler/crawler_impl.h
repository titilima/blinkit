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

#include <string_view>
#include <tuple>
#include "sdk/include/BlinKit.h"
#include "frame_loader_client_impl.h"

namespace BlinKit {

class CrawlerImpl final : public BkCrawler, public FrameLoaderClientImpl
{
public:
    CrawlerImpl(BkCrawlerClient &client);
    ~CrawlerImpl(void);

    std::tuple<int, std::string> Initialize(void);

    BkCrawlerClient& Client(void) const { return m_client; }
    std::string GetCookies(const std::string &URL) const;

    void CancelLoading(void);
private:
    // BkCrawler
    void BKAPI Destroy(void) override { delete this; }
    int BKAPI Load(const char *URL) override;
    int BKAPI Eval(const char *code, size_t length, BkCallback *callback) override;
    int BKAPI CallFunction(const char *name, BkCallback *callback) override;
    int BKAPI CallCrawler(const char *method, BkCallback *callback) override;
    int BKAPI RegisterCrawlerFunction(const char *name, BkCallback &functionImpl) override;
    int BKAPI AccessCrawlerMember(const char *name, BkCallback &callback) override;
    // blink::FrameClient
    bool IsCrawler(void) const override { return true; }
    // blink::FrameLoaderClient
    void dispatchDidFailProvisionalLoad(const blink::ResourceError &error, blink::HistoryCommitType) override;
    void dispatchDidFinishLoad(void) override;
    String userAgent(void) override;

    BkCrawlerClient &m_client;
    RefPtr<blink::LocalFrame> m_frame;
};

DEFINE_TYPE_CASTS(CrawlerImpl, ::blink::FrameClient, client, client->IsCrawler(), client.IsCrawler());

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_IMPL_H
