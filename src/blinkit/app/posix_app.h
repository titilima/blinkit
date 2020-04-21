// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_app.h
// Description: PosixApp Class
//      Author: Ziming Li
//     Created: 2020-04-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_POSIX_APP_H
#define BLINKIT_BLINKIT_POSIX_APP_H

#pragma once

#include "blinkit/app/app_impl.h"

namespace BlinKit {

class TaskLoop;

class PosixApp final : public AppImpl
{
public:
    PosixApp(int mode, BkAppClient *client);
    ~PosixApp(void) override;
private:
    // Thread
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const override;
    // AppImpl
    int RunAndFinalize(void) override;
    void Exit(int code) override;

    std::unique_ptr<TaskLoop> m_taskLoop;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_POSIX_APP_H
