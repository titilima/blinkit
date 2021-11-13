// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: loader_thread.cpp
// Description: LoaderThread Class
//      Author: Ziming Li
//     Created: 2020-11-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./loader_thread.h"

#include "blinkit/blink/impl/thread.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/loader/loader_task.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

using namespace blink;

namespace BlinKit {

void LoaderThread::AddTask(LoaderTask *task)
{
    ASSERT(isMainThread());
    if (auto _ = m_mutex.guard())
        m_tasks.push(task);
    m_signal.notify();
}

void LoaderThread::Run(void)
{
    ASSERT(!isMainThread());
#ifndef NDEBUG
    Thread::SetName("Loader Thread");
#endif

    for (;;)
    {
        std::queue<LoaderTask *> tasks;

        m_signal.wait();
        if (auto _ = m_mutex.guard())
        {
            ASSERT(!m_tasks.empty());
            m_tasks.swap(tasks);
        }
        m_signal.reset();

        while (!tasks.empty())
        {
            LoaderTask *task = zed::pop(tasks);
            if (nullptr == task)
            {
                ASSERT(tasks.empty()); // The null task SHOULD BE the last one.
                return;
            }
            task->Run();
        }
    }
}

} // namespace BlinKit
