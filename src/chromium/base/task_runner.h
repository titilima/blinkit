// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: task_runner.h
// Description: TaskRunner Class
//      Author: Ziming Li
//     Created: 2019-10-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_TASK_RUNNER_H
#define BLINKIT_BASE_TASK_RUNNER_H

#pragma once

#include <functional>
#include "base/location.h"
#include "base/time/time.h"

namespace base {

class TaskRunner
{
public:
    virtual ~TaskRunner(void) = default;

    // Posts the given task to be run.  Returns true if the task may be
    // run at some point in the future, and false if the task definitely
    // will not be run.
    //
    // Equivalent to PostDelayedTask(from_here, task, 0).
    bool PostTask(const Location &fromHere, const std::function<void()> &task);

    // Like PostTask, but tries to run the posted task only after |delay_ms|
    // has passed. Implementations should use a tick clock, rather than wall-
    // clock time, to implement |delay|.
    virtual bool PostDelayedTask(const Location &fromHere, const std::function<void()> &task, TimeDelta delay) = 0;
protected:
    TaskRunner(void) = default;
};

} // namespace base 

#endif // BLINKIT_BASE_TASK_RUNNER_H
