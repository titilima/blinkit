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
        LoaderTask *task = nullptr;
        m_signal.wait();

        if (auto _ = m_mutex.guard())
        {
            task = m_tasks.front();
            m_tasks.pop();
        }
        m_signal.reset();

        if (nullptr == task)
            break;

        task->Run();
    }
}

} // namespace BlinKit
