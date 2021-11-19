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
#include "bk_crawler.h"
#include "blinkit/blink/renderer/core/loader/FrameLoaderClient.h"
#include "third_party/zed/include/zed/shared_mutex.hpp"

class CookieJarImpl;

class CrawlerImpl final : public blink::FrameLoaderClient
{
public:
    CrawlerImpl(const BkCrawlerClient &client);
    ~CrawlerImpl(void);

    static CrawlerImpl* From(const blink::Document &document);

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
    // FrameClient
#ifdef BLINKIT_FULL_BUILD
    Type GetType(void) const override { return Type::Crawler; }
#endif
    // FrameLoaderClient
    String userAgent(void) override;
#if 0 // BKTODO:
    void DidFinishLoad(void) override;
    void TransitionToCommittedForNewPage(void) override;
    void DispatchDidReceiveTitle(const String &title) override {}
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) override;
#endif

    mutable zed::shared_mutex m_mutex;

    BkCrawlerClient m_client;
    std::unique_ptr<blink::LocalFrame> m_frame;
    bool m_dirty = false;

    CookieJarImpl *m_cookieJar = nullptr;
};

// BKTODO: DEFINE_TYPE_CASTS(CrawlerImpl, ::blink::FrameLoaderClient, client, client->IsCrawler(), client.IsCrawler());

#endif // BLINKIT_BLINKIT_CRAWLER_IMPL_H
