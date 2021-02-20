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

#include "loader_thread.h"

#include <mutex>
#include "blinkit/blink_impl/thread_impl.h"
#include "blinkit/loader/loader_task.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace BlinKit {

void LoaderThread::AddTask(LoaderTask *task)
{
    ASSERT(IsMainThread());
    {
        std::unique_lock<BkMutex> lock(m_mutex);
        m_tasks.push(task);
    }
    m_signal.Signal();
}

void LoaderThread::Run(void)
{
    ASSERT(!IsMainThread());
#ifndef NDEBUG
    ThreadImpl::SetName("Loader Thread");
#endif
    for (;;)
    {
        LoaderTask *task = nullptr;
        m_signal.Wait();
        {
            std::unique_lock<BkMutex> lock(m_mutex);
            task = m_tasks.front();
            m_tasks.pop();
        }

        if (nullptr == task)
            break;

        task->Run();
    }
}

} // namespace BlinKit
