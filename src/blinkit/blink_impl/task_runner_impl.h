// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: task_runner_impl.h
// Description: TaskRunnerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_TASK_RUNNER_IMPL_H
#define BLINKIT_BLINKIT_TASK_RUNNER_IMPL_H

#pragma once

#include "public/platform/WebTaskRunner.h"

namespace BlinKit {

class ThreadImpl;

class TaskRunnerImpl : public blink::WebTaskRunner
{
public:
    static std::unique_ptr<blink::WebTaskRunner> CreateInstance(ThreadImpl &thread);
protected:
    TaskRunnerImpl(ThreadImpl &thread);

    ThreadImpl &m_thread;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_TASK_RUNNER_IMPL_H
