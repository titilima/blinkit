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
#include "blink_impl/thread_impl.h"

namespace BlinKit {

class AppImpl : public BkApp, public blink::Platform, public ThreadImpl
{
public:
    static void CreateInstance(void);
    virtual ~AppImpl(void);

    void Initialize(BkAppClient *client);

    // blink::Platform
    blink::WebThread* currentThread(void) override final;
protected:
    AppImpl(void);
private:
    // BkApp
    void BKAPI Exit(void) override final;
    BkCrawler* BKAPI CreateCrawler(BkCrawlerClient &client) override final;
    BkView* BKAPI CreateView(BkViewClient &client) override final;

    BkAppClient *m_client = nullptr;
    std::unordered_map<blink::PlatformThreadId, blink::WebThread *> m_threads;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_IMPL_H
