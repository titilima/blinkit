// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_task_runner.h
// Description: PosixTaskRunner Class
//      Author: Ziming Li
//     Created: 2020-04-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_POSIX_TASK_RUNNER_H
#define BLINKIT_BLINKIT_POSIX_TASK_RUNNER_H

#pragma once

#include "base/single_thread_task_runner.h"

namespace BlinKit {

class PosixTaskRunner final : public base::SingleThreadTaskRunner
{
public:
    typedef std::function<void(const base::Location &, const std::function<void()> &)> TaskPoster;

    PosixTaskRunner(const TaskPoster &taskPoster);
private:
    // TaskRunner overrides
    bool PostDelayedTask(const base::Location &fromHere, const std::function<void()> &task, base::TimeDelta delay) override;

    const TaskPoster m_taskPoster;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_POSIX_TASK_RUNNER_H
