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
#include "blinkit/blink_impl/local_frame_client_impl.h"

class CookieJarImpl;

class CrawlerImpl final : public BlinKit::LocalFrameClientImpl
{
public:
    CrawlerImpl(const BkCrawlerClient &client);
    ~CrawlerImpl(void);

    // BkCrawlerClient Wrappers
    bool GetConfig(int cfg, std::string &dst) const;
    void ApplyProxyToRequest(BkRequest req);
    std::string GetCookies(const std::string &URL) const;
    bool ScriptEnabled(const std::string &URL);
    void ProcessRequestComplete(BkResponse response, BkWorkController controller);
    bool HijackRequest(const char *URL, std::string &dst) const;
    void HijackResponse(BkResponse response);
    bool ApplyConsoleMessager(std::function<void(int, const char *)> &dst) const;
    void ProcessDocumentReset(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    int Run(const char *URL);
    CookieJarImpl* GetCookieJar(void) { return m_cookieJar; }
    void SetCookieJar(CookieJarImpl *cookieJar, int mode);
    BkJSContext GetScriptContext(void);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CancelLoading(void);
private:
    // LocalFrameClient
    bool IsCrawler(void) const override { return true; }
    String UserAgent(void) override;
    void TransitionToCommittedForNewPage(void) override;
    void DispatchDidReceiveTitle(const String &title) override {}
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) override;
    void DispatchDidFinishLoad(void) override;

    BkCrawlerClient m_client;
    std::unique_ptr<blink::LocalFrame> m_frame;

    int m_proxyType = BK_PROXY_RESERVED;
    std::string m_proxy;
    int m_cookieJarMode = BK_CJM_SHARED;
    CookieJarImpl *m_cookieJar = nullptr;
    std::unordered_map<std::string, bool> m_scriptModeMap;
};

DEFINE_TYPE_CASTS(CrawlerImpl, ::blink::LocalFrameClient, client, client->IsCrawler(), client.IsCrawler());

#endif // BLINKIT_BLINKIT_CRAWLER_IMPL_H
