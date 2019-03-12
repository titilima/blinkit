// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: frame_scheduler_impl.h
// Description: FrameSchedulerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_FRAME_SCHEDULER_IMPL_H
#define BLINKIT_BLINKIT_FRAME_SCHEDULER_IMPL_H

#pragma once

#include "public/platform/WebFrameScheduler.h"

namespace BlinKit {

class ThreadImpl;

class FrameSchedulerImpl final : public blink::WebFrameScheduler
{
public:
    static FrameSchedulerImpl* CreateInstance(ThreadImpl &thread);
private:
    FrameSchedulerImpl(ThreadImpl &thread);

    // blink::WebFrameScheduler
    void setFrameVisible(bool) override;
    blink::WebTaskRunner* loadingTaskRunner(void) override;
    blink::WebTaskRunner* timerTaskRunner(void) override;
    void setFrameOrigin(const blink::WebSecurityOrigin &securityOrigin) override;

    ThreadImpl &m_thread;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_FRAME_SCHEDULER_IMPL_H
