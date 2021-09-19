#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: cancellable_task_factory.h
// Description: CancellableTaskFactory Class
//      Author: Ziming Li
//     Created: 2021-09-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CANCELLABLE_TASK_FACTORY_H
#define BLINKIT_BLINKIT_CANCELLABLE_TASK_FACTORY_H

#include <functional>
#include "blinkit/blink/public/platform/WebTaskRunner.h"
#include "third_party/zed/include/zed/memory.hpp"

namespace blink {

class CancellableTaskFactory
{
public:
    template <class T>
    static std::unique_ptr<CancellableTaskFactory> create(T *thisObject, void (T::*method)())
    {
        return zed::wrap_unique(new CancellableTaskFactory(std::bind(method, thisObject)));
    }
    ~CancellableTaskFactory(void);

    WebTaskRunner::Task* cancelAndCreate(void);
private:
    using Closure = std::function<void()>;
    CancellableTaskFactory(const Closure &closure);

    class CancellableTask;

    enum TaskStatus { Pending, Canceled, Finished };
    std::shared_ptr<TaskStatus> m_lastStatus;

    std::shared_ptr<Closure> m_closure;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_CANCELLABLE_TASK_FACTORY_H
