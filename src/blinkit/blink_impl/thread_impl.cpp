// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: thread_impl.cpp
// Description: ThreadImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "thread_impl.h"

#include "blink_impl/scheduler_impl.h"
#include "blink_impl/task_runner_impl.h"

using namespace blink;

namespace BlinKit {

ThreadImpl::ThreadImpl(void)
    : m_scheduler(std::make_unique<SchedulerImpl>(*this)), m_taskRunner(TaskRunnerImpl::CreateInstance(*this))
{
    // Nothing
}

ThreadImpl::~ThreadImpl(void)
{
    // Nothing, just for std::unique_ptrs
}

void ThreadImpl::addTaskObserver(WebThread::TaskObserver *observer)
{
    AutoLock lock(m_lock);
    addTaskObserverWithoutLock(observer);
}

void ThreadImpl::addTaskObserverWithoutLock(WebThread::TaskObserver *observer)
{
    m_observers.insert(observer);
}

void ThreadImpl::ApplyThreadId(PlatformThreadId threadId)
{
    assert(0 == m_threadId);
    m_threadId = threadId;
}

void ThreadImpl::DidProcessTask(void)
{
    for (auto &it : m_observers)
        it->didProcessTask();
}

void ThreadImpl::removeTaskObserver(WebThread::TaskObserver *observer)
{
    AutoLock lock(m_lock);
    removeTaskObserverWithoutLock(observer);
}

void ThreadImpl::removeTaskObserverWithoutLock(WebThread::TaskObserver *observer)
{
    m_observers.erase(observer);
}

WebScheduler* ThreadImpl::scheduler(void) const
{
    return m_scheduler.get();
}

void ThreadImpl::WillProcessTask(void)
{
    for (auto &it : m_observers)
        it->willProcessTask();
}

} // namespace BlinKit
