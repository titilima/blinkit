// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_task_runner.h
// Description: Task Runner Classes
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_APPLE_TASK_RUNNER_H
#define BLINKIT_BLINKIT_APPLE_TASK_RUNNER_H

#pragma once

#include "base/single_thread_task_runner.h"

namespace BlinKit {

class MainThreadTaskRunner final : public base::SingleThreadTaskRunner
{
public:
    MainThreadTaskRunner(void) = default;
private:
    static void Worker(void *data);

    // TaskRunner overrides
    bool PostDelayedTask(const base::Location &fromHere, const std::function<void()> &task, base::TimeDelta delay) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APPLE_TASK_RUNNER_H
