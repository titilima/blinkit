// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: app_caller_impl.cpp
// Description: AppCall Implementations
//      Author: Ziming Li
//     Created: 2021-01-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "app_caller_impl.h"

#include "third_party/zed/include/zed/threading/signal.hpp"
#include "base/single_thread_task_runner.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"

namespace BlinKit {

AppCallerImpl::AppCallerImpl(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner)
    : m_taskRunner(taskRunner)
{
}

AppCallerImpl::~AppCallerImpl(void) = default;

void AppCallerImpl::Call(const base::Location &loc, std::function<void()> &&task)
{
    m_taskRunner->PostTask(loc, std::move(task));
}

void AppCallerImpl::SyncCall(const base::Location &loc, std::function<void()> &&task)
{
    if (isMainThread())
    {
        task();
    }
    else
    {
        zed::signal waiter;
        auto waitableTask = [&waiter, task = std::move(task)]
        {
            task();
            waiter.notify();
        };
        m_taskRunner->PostTask(loc, std::move(waitableTask));
        waiter.wait();
    }
}

} // namespace BlinKit
