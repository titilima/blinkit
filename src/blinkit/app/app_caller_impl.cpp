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

#include "blinkit/blink/public/platform/WebTaskRunner.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "third_party/zed/include/zed/threading/signal.hpp"

using namespace blink;

namespace BlinKit {

AppCallerImpl::AppCallerImpl(const std::shared_ptr<WebTaskRunner> &taskRunner) : m_taskRunner(taskRunner)
{
}

AppCallerImpl::~AppCallerImpl(void) = default;

void AppCallerImpl::Call(const WebTraceLocation &loc, std::function<void()> &&task)
{
    m_taskRunner->postTask(loc, std::move(task));
}

void AppCallerImpl::SyncCall(const WebTraceLocation &loc, std::function<void()> &&task)
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
        m_taskRunner->postTask(loc, std::move(waitableTask));
        waiter.wait();
    }
}

} // namespace BlinKit
