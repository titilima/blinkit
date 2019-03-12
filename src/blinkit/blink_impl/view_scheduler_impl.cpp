// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: view_scheduler_impl.cpp
// Description: ViewSchedulerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "view_scheduler_impl.h"

#include "blink_impl/frame_scheduler_impl.h"

using namespace blink;

namespace BlinKit {

ViewSchedulerImpl::ViewSchedulerImpl(ThreadImpl &thread) : m_thread(thread)
{
    // Nothing
}

WebPassOwnPtr<WebFrameScheduler> ViewSchedulerImpl::createFrameScheduler(void)
{
    return adoptWebPtr(FrameSchedulerImpl::CreateInstance(m_thread));
}

ViewSchedulerImpl* ViewSchedulerImpl::CreateInstance(ThreadImpl &thread)
{
    return new ViewSchedulerImpl(thread);
}

} // namespace BlinKit
