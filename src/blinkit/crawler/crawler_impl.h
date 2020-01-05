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

// BKTODO: #include <tuple>
#include "bk_crawler.h"
#include "blinkit/blink_impl/local_frame_client_impl.h"

class CrawlerImpl final : public BlinKit::LocalFrameClientImpl
{
public:
    CrawlerImpl(const BkCrawlerClient &client);
    ~CrawlerImpl(void);

    void ProcessRequestComplete(BkResponse response, BkWorkController controller);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    int Run(const char *URL);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0 // BKTODO:
    std::tuple<int, std::string> Initialize(void);

    BkCrawlerClient& Client(void) const { return m_client; }
    std::string GetCookies(const std::string &URL) const;

    void CancelLoading(void);
#endif
private:
#if 0 // BKTODO:
    // BkCrawler
    int BKAPI Eval(const char *code, size_t length, BkCallback *callback) override;
    int BKAPI CallFunction(const char *name, BkCallback *callback) override;
    int BKAPI CallCrawler(const char *method, BkCallback *callback) override;
    int BKAPI RegisterCrawlerFunction(const char *name, BkCallback &functionImpl) override;
    int BKAPI AccessCrawlerMember(const char *name, BkCallback &callback) override;
    // blink::FrameLoaderClient
    void dispatchDidFailProvisionalLoad(const blink::ResourceError &error, blink::HistoryCommitType) override;
    void dispatchDidFinishLoad(void) override;
#endif
    // LocalFrameClient
    bool IsCrawler(void) const override { return true; }
    String UserAgent(void) override;
    void TransitionToCommittedForNewPage(void) override;
    void DispatchDidReceiveTitle(const String &title) override {}
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) override;
    void DispatchDidFinishLoad(void) override;

    BkCrawlerClient m_client;
    std::unique_ptr<blink::LocalFrame> m_frame;
};

#if 0 // BKTODO:
DEFINE_TYPE_CASTS(CrawlerImpl, ::blink::FrameClient, client, client->IsCrawler(), client.IsCrawler());
#endif

#endif // BLINKIT_BLINKIT_CRAWLER_IMPL_H
