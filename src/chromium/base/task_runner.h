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
    // Like PostTask, but tries to run the posted task only after |delay_ms|
    // has passed. Implementations should use a tick clock, rather than wall-
    // clock time, to implement |delay|.
    virtual bool PostDelayedTask(const Location &fromHere, const std::function<void()> &task, TimeDelta delay) = 0;
};

} // namespace base 

#endif // BLINKIT_BASE_TASK_RUNNER_H
