// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_app.h
// Description: AppleApp Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_APPLE_APP_H
#define BLINKIT_BLINKIT_APPLE_APP_H

#pragma once

#include <pthread.h>
#include "blinkit/app/app_impl.h"

namespace BlinKit {

class RunLoop;

class AppleApp final : public AppImpl
{
public:
    ~AppleApp(void) override;
private:
    friend AppImpl;
    AppleApp(BkAppClient *client);

    bool InitializeForBackgroundMode(void);
    static void BackgroundThread(void *param);

    // Thread
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const override;
    // AppImpl
    int RunMessageLoop(void) override;
    void Exit(int code) override;
    ClientCaller& AcquireCallerForClient(void) override;
    void Initialize(void) override;

    int m_exitCode = EXIT_SUCCESS;
    pthread_t m_thread = nullptr; // nullptr for exclusive mode
    std::unique_ptr<RunLoop> m_runLoop;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APPLE_APP_H
