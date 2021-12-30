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

class Compositor;
class LoaderThread;

class AppImpl : public Platform, public Thread
{
public:
    virtual ~AppImpl(void);

    static AppImpl& Get(void);

    void Log(const char *s);

#ifdef BLINKIT_UI_ENABLED
    LoaderThread& GetLoaderThread(void);
    Compositor& GetCompositor(void) { return *m_compositor; }

    bool HasClientLoader(void) const { return nullptr != m_client.LoadResource; }
    bool LoadResourceFromClient(const char *URI, std::string &dst) const;
#endif
protected:
    AppImpl(BkAppClient *client);

#ifdef BLINKIT_UI_ENABLED
#   ifndef NDEBUG
    WebData loadResource(const char *name) override;
#   endif
#endif
private:
    // Platform
    WebURLLoader* createURLLoader(void) final;
    WebThread* currentThread(void) final;
    double currentTimeSeconds(void) final;
    double monotonicallyIncreasingTimeSeconds(void) final;
#ifdef BLINKIT_UI_ENABLED
    WebCompositorSupport* compositorSupport(void);
#endif

    BkAppClient m_client;
    double m_firstMonotonicallyIncreasingTime;
#ifdef BLINKIT_UI_ENABLED
    std::unique_ptr<LoaderThread> m_loaderThread;
    std::unique_ptr<Compositor> m_compositor;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_IMPL_H
