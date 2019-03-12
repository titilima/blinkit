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

using namespace blink;

namespace BlinKit {

ThreadImpl::ThreadImpl(void) : m_scheduler(std::make_unique<SchedulerImpl>(*this))
{
    // Nothing
}

ThreadImpl::~ThreadImpl(void)
{
    // Nothing, just for std::unique_ptrs
}

void ThreadImpl::ApplyThreadId(PlatformThreadId threadId)
{
    assert(0 == m_threadId);
    m_threadId = threadId;
}

WebScheduler* ThreadImpl::scheduler(void) const
{
    return m_scheduler.get();
}

} // namespace BlinKit
