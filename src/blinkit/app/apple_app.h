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

#include "blinkit/app/app_impl.h"

namespace BlinKit {

class AppleApp final : public AppImpl
{
public:
    AppleApp(int mode);
    ~AppleApp(void) override;
private:
    // Thread
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const override;
    // AppImpl
    int RunAndFinalize(void) override;
    void Exit(int code) override;

    int m_exitCode = EXIT_SUCCESS;
    std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APPLE_APP_H
