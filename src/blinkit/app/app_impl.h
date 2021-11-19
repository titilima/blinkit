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
#include "blinkit/blink/impl/thread.h"
#include "blinkit/blink/public/platform/Platform.h"

class CrawlerImpl;

namespace BlinKit {

class LoaderThread;

class AppImpl : public blink::Platform, public Thread
{
public:
    virtual ~AppImpl(void);

    static AppImpl& Get(void);

    LoaderThread& GetLoaderThread(void);
    void Log(const char *s);

    bool HasClientLoader(void) const { return nullptr != m_client.LoadResource; }
    bool LoadResourceFromClient(const char *URI, std::string &dst) const;
protected:
    AppImpl(BkAppClient *client);

    void OnExit(void);
private:
    // blink::Platform
    blink::WebURLLoader* createURLLoader(void) final;
    blink::WebThread* currentThread(void) final;
    double currentTimeSeconds(void) final;
    double monotonicallyIncreasingTimeSeconds(void) final;

    BkAppClient m_client;
    double m_firstMonotonicallyIncreasingTime;
    std::unique_ptr<LoaderThread> m_loaderThread;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_IMPL_H
