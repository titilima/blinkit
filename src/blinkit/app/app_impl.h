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

#include "bk_app.h"
#include "third_party/blink/public/platform/platform.h"
#include "blinkit/blink_impl/thread_impl.h"

namespace BlinKit {

class CookieJarImpl;
class MimeRegistryImpl;

class AppImpl : public blink::Platform, public ThreadImpl
{
public:
    static AppImpl* CreateInstance(int mode, BkAppClient *client);
    virtual ~AppImpl(void);

    static void InitializeBackgroundInstance(BkAppClient *client);

    static AppImpl& Get(void);
    int Mode(void) const { return m_mode; }
    virtual void Initialize(BkAppClient *client);
    virtual int RunAndFinalize(void) = 0;
    virtual void Exit(int code) = 0;

#if 0 // BKTODO:
    ThreadImpl* CurrentThreadImpl(void);
    blink::WebThread& IOThread(void);
    CookieJarImpl& CookieJar(void);
#endif

    void Log(const char *s);

#if 0 // BKTODO:
    // blink::Platform
    blink::WebCookieJar* cookieJar(void) final;
    blink::WebThread* currentThread(void) final;
#endif
protected:
    AppImpl(int mode, BkAppClient *client);
private:
    // blink::Platform
    std::unique_ptr<blink::WebURLLoader> CreateURLLoader(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner) final;
#if 0 // BKTODO:
    // blink::Platform
    blink::WebMimeRegistry* mimeRegistry(void) final;
    blink::WebURLError cancelledError(const blink::WebURL &url) const final;
    blink::WebThread* createThread(const char *name) final;
    double currentTimeSeconds(void) final;
    double monotonicallyIncreasingTimeSeconds(void) final;

    std::unique_ptr<CookieJarImpl> m_cookieJar;
    std::unique_ptr<MimeRegistryImpl> m_mimeRegistry;
    std::unique_ptr<blink::WebThread> m_IOThread;
    double m_firstMonotonicallyIncreasingTime;
    std::unordered_map<blink::PlatformThreadId, ThreadImpl *> m_threads;
#endif
    const int m_mode;
    BkAppClient m_client;
    std::unique_ptr<blink::scheduler::WebThreadScheduler> m_mainThreadScheduler;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_IMPL_H
