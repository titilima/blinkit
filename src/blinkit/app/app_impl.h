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
#include "blinkit/app/caller.h"
#include "blinkit/blink/impl/thread.h"
#include "blinkit/blink/public/platform/Platform.h"

class CrawlerImpl;

namespace BlinKit {

class LoaderThread;

class AppImpl : public blink::Platform, public Thread
{
public:
#ifdef BLINKIT_CRAWLER_ONLY
    static std::unique_ptr<AppImpl> CreateInstanceForExclusiveMode(BkAppClient *client);
#endif
    static bool InitializeForBackgroundMode(BkAppClient *client);
    virtual ~AppImpl(void);

    static AppImpl& Get(void);
    virtual int RunMessageLoop(void) = 0;
    virtual void Exit(int code) = 0;

    AppCaller& GetAppCaller(void) { return *m_appCaller; }
    virtual ClientCaller& AcquireCallerForClient(void) = 0;
    LoaderThread& GetLoaderThread(void);
    void Log(const char *s);
protected:
    AppImpl(BkAppClient *client);

    virtual void Initialize(void);
    void OnExit(void);

    std::unique_ptr<AppCaller> m_appCaller;
private:
    // blink::Platform
    blink::WebURLLoader* createURLLoader(void) final;
    blink::WebThread* currentThread(void) override final;
    double currentTimeSeconds(void) override final;
    double monotonicallyIncreasingTimeSeconds(void) override final;

    BkAppClient m_client;
    double m_firstMonotonicallyIncreasingTime;
    std::unordered_map<blink::PlatformThreadId, Thread *> m_threads;
    std::unique_ptr<LoaderThread> m_loaderThread;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_IMPL_H
