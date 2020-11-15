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
class GCHeap;
class LoaderThread;
#ifndef BLINKIT_CRAWLER_ONLY
class HeapStorage;
#endif

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

    LoaderThread& GetLoaderThread(void);
    void Log(const char *s);
protected:
    AppImpl(int mode, BkAppClient *client);
private:
    // blink::Platform
    std::unique_ptr<blink::WebURLLoader> CreateURLLoader(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner) final;

    const int m_mode;
    BkAppClient m_client;
    std::unique_ptr<GCHeap> m_gcHeap;
    std::unique_ptr<blink::scheduler::WebThreadScheduler> m_mainThreadScheduler;
    std::unique_ptr<LoaderThread> m_loaderThread;
#ifndef BLINKIT_CRAWLER_ONLY
    std::unique_ptr<HeapStorage> m_heapStorage;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_IMPL_H
