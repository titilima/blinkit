// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: app_impl.h
// Description: AppImpl Class
//      Author: Ziming Li
//     Created: 2019-03-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_APP_IMPL_H
#define BLINKIT_BLINKIT_APP_IMPL_H

#pragma once

#include "sdk/include/BlinKit.h"
#include "public/platform/Platform.h"
#include "blinkit/blink_impl/thread_impl.h"

namespace blink {
class Settings;
}

namespace BlinKit {

class CookieJarImpl;
class MimeRegistryImpl;

class AppImpl : public BkApp, public blink::Platform, public ThreadImpl
{
public:
    static void CreateInstance(void);
    virtual ~AppImpl(void);

    void Initialize(BkAppClient *client);
    static AppImpl& Get(void);

    BkRequest* CreateRequest(const char *URL, BkRequestClient &client);

    ThreadImpl* CurrentThreadImpl(void);
    blink::WebThread& IOThread(void);
    CookieJarImpl& CookieJar(void);
    blink::Settings& CrawlerSettings(void);

    // blink::Platform
    blink::WebCookieJar* cookieJar(void) override final;
    blink::WebThread* currentThread(void) override final;
protected:
    AppImpl(void);
private:
    // BkApp
    void BKAPI Exit(void) override final;
    BkCrawler* BKAPI CreateCrawler(BkCrawlerClient &client, BkBuffer *errorMessage) override final;
    BkView* BKAPI CreateView(BkViewClient &client) override;
    // blink::Platform
    blink::WebMimeRegistry* mimeRegistry(void) override final;
    blink::WebURLLoader* createURLLoader(void) override final;
    blink::WebString userAgent(void) override final;
    blink::WebURLError cancelledError(const blink::WebURL &url) const override final;
    blink::WebThread* createThread(const char *name) override final;
    double currentTimeSeconds(void) override final;
    double monotonicallyIncreasingTimeSeconds(void) override final;

    BkAppClient *m_client = nullptr;
    std::unique_ptr<CookieJarImpl> m_cookieJar;
    std::unique_ptr<MimeRegistryImpl> m_mimeRegistry;
    std::unique_ptr<blink::WebThread> m_IOThread;
    OwnPtr<blink::Settings> m_crawlerSettings;
    double m_firstMonotonicallyIncreasingTime;
    std::unordered_map<blink::PlatformThreadId, ThreadImpl *> m_threads;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_IMPL_H
