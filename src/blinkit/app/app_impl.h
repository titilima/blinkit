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

#include <unordered_map>
#include "public/platform/platform.h"

namespace BlinKit {

class BkAppClient;

class CookieJarImpl;
class MimeRegistryImpl;

class AppImpl : public blink::Platform
{
public:
    static AppImpl* CreateInstance(void);
    virtual ~AppImpl(void);

    void Initialize(BkAppClient *client);
    static AppImpl& Get(void);

#if 0 // BKTODO:
    ThreadImpl* CurrentThreadImpl(void);
    blink::WebThread& IOThread(void);
    CookieJarImpl& CookieJar(void);
#endif

    void Log(const char *s);

#if 0 // BKTODO:
    // blink::Platform
    blink::WebCookieJar* cookieJar(void) override final;
    blink::WebThread* currentThread(void) override final;
#endif
protected:
    AppImpl(void);
private:
#if 0 // BKTODO:
    // blink::Platform
    blink::WebMimeRegistry* mimeRegistry(void) override final;
    blink::WebURLLoader* createURLLoader(void) override final;
    blink::WebString userAgent(void) override final;
    blink::WebURLError cancelledError(const blink::WebURL &url) const override final;
    blink::WebThread* createThread(const char *name) override final;
    double currentTimeSeconds(void) override final;
    double monotonicallyIncreasingTimeSeconds(void) override final;

    std::unique_ptr<CookieJarImpl> m_cookieJar;
    std::unique_ptr<MimeRegistryImpl> m_mimeRegistry;
    std::unique_ptr<blink::WebThread> m_IOThread;
    double m_firstMonotonicallyIncreasingTime;
    std::unordered_map<blink::PlatformThreadId, ThreadImpl *> m_threads;
#endif
    std::unique_ptr<blink::scheduler::WebThreadScheduler> m_mainThreadScheduler;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_IMPL_H
