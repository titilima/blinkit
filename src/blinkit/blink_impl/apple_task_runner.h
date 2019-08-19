// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_task_runner.h
// Description: AppleTaskRunner Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_APPLE_TASK_RUNNER_H
#define BLINKIT_BLINKIT_APPLE_TASK_RUNNER_H

#pragma once

#include "task_runner_impl.h"

@class NSThread;

namespace BlinKit {

class AppleTaskRunner final : public TaskRunnerImpl
{
public:
    AppleTaskRunner(ThreadImpl &thread);

private:
    // blink::WebTaskRunner
    void postTask(const blink::WebTraceLocation &location, Task *task) override;
    void postDelayedTask(const blink::WebTraceLocation &location, Task *task, double delayMs) override;
    blink::WebTaskRunner* clone(void) override;

    NSThread *m_nativeThread;
};

} // namespace BlinKit

#endif
