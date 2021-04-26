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

#include <functional>
#include <string>
#include <unordered_map>
#include "bk_crawler.h"
#include "bkcommon/bk_shared_mutex.hpp"
#include "blinkit/blink_impl/local_frame_client_impl.h"

class CookieJarImpl;

class CrawlerImpl final : public BlinKit::LocalFrameClientImpl
{
public:
    CrawlerImpl(const BkCrawlerClient &client, BlinKit::ClientCaller &clientCaller);
    ~CrawlerImpl(void);

    // BkCrawlerClient Wrappers
    bool GetConfig(int cfg, std::string &dst) const;
    std::string GetCookies(const std::string &URL) const;
    bool ScriptEnabled(const std::string &URL) const;
    bool ProcessRequestComplete(BkResponse response, BkWorkController controller);
    bool HijackRequest(const char *URL, std::string &dst) const;
    void ModifyRequest(const char *URL, BkRequest req);
    void HijackResponse(BkResponse response);
    bool ProcessConsoleMessage(int type, const char *msg);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    void Destroy(void);
    int Run(const char *URL);
    CookieJarImpl* GetCookieJar(bool createIfNotExists);
    void SetCookieJar(CookieJarImpl *cookieJar);
    int CallJS(BkJSCallback callback, void *userData);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool DirtyFlag(void) const { return m_dirty; }
    void CleanupDirtyFlag(void) { m_dirty = false; }
    void CancelLoading(void);
private:
    // LocalFrameClient
    bool IsCrawler(void) const override { return true; }
    String UserAgent(void) override;
    void DidFinishLoad(void) override;
    void TransitionToCommittedForNewPage(void) override;
    void DispatchDidReceiveTitle(const String &title) override {}
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) override;

    mutable BlinKit::BkSharedMutex m_mutex;

    BkCrawlerClient m_client;
    std::unique_ptr<blink::LocalFrame> m_frame;
    bool m_dirty = false;

    CookieJarImpl *m_cookieJar = nullptr;
};

DEFINE_TYPE_CASTS(CrawlerImpl, ::blink::LocalFrameClient, client, client->IsCrawler(), client.IsCrawler());

#endif // BLINKIT_BLINKIT_CRAWLER_IMPL_H
