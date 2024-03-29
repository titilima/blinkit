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
#include "blinkit/blink/renderer/core/loader/frame_loader_client.h"
#include "third_party/zed/include/zed/shared_mutex.hpp"

class CookieJarImpl;

class CrawlerImpl final : public blink::FrameLoaderClient
{
public:
    CrawlerImpl(const BkCrawlerClient &client);
    ~CrawlerImpl(void);

    static CrawlerImpl* From(const blink::Document &document);

    // BkCrawlerClient Wrappers
    bool ScriptEnabled(const std::string &URL) const;
    bool ProcessRequestComplete(BkResponse response, BkWorkController controller);
    bool HijackScript(const char *URL, std::string &dst) const;
    void ModifyRequest(const char *URL, BkRequest req);
    void HijackResponse(BkResponse response);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    void Destroy(void);
    int Run(const char *URL);
    CookieJarImpl* GetCookieJar(bool createIfNotExists);
    void SetCookieJar(CookieJarImpl *cookieJar);
    BkJSContext GetJSContext(void);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool DirtyFlag(void) const { return m_dirty; }
    void CleanupDirtyFlag(void) { m_dirty = false; }
    void CancelLoading(void);
private:
    // FrameClient
#ifdef BLINKIT_UI_ENABLED
    Type GetType(void) const override { return Type::Crawler; }
#endif
    // FrameLoaderClient
    void dispatchDidFinishLoad(void) override;
    BkContext RequireScriptContext(void) const override;

    mutable zed::shared_mutex m_mutex;

    const BkCrawlerClient m_client;
    BlinKit::GCUniquePtr<blink::LocalFrame> m_frame;
    bool m_dirty = false;

    CookieJarImpl *m_cookieJar = nullptr;
};

#endif // BLINKIT_BLINKIT_CRAWLER_IMPL_H
