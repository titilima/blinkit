// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: frame_scheduler_impl.cpp
// Description: FrameSchedulerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "frame_scheduler_impl.h"

#include "blink_impl/thread_impl.h"

using namespace blink;

namespace BlinKit {

FrameSchedulerImpl::FrameSchedulerImpl(ThreadImpl &thread) : m_thread(thread)
{
    // Nothing
}

FrameSchedulerImpl* FrameSchedulerImpl::CreateInstance(ThreadImpl &thread)
{
    return new FrameSchedulerImpl(thread);
}

WebTaskRunner* FrameSchedulerImpl::loadingTaskRunner(void)
{
    return m_thread.taskRunner();
}

void FrameSchedulerImpl::setFrameOrigin(const WebSecurityOrigin &securityOrigin)
{
    assert(false); // BKTODO:
}

void FrameSchedulerImpl::setFrameVisible(bool visible)
{
    assert(false); // BKTODO:
}

WebTaskRunner* FrameSchedulerImpl::timerTaskRunner(void)
{
    return m_thread.taskRunner();
}

} // namespace BlinKit
