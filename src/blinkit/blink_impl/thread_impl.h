// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: thread_impl.h
// Description: ThreadImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_THREAD_IMPL_H
#define BLINKIT_BLINKIT_THREAD_IMPL_H

#pragma once

#include <mutex>
#include "public/platform/WebThread.h"

namespace BlinKit {

class SchedulerImpl;

class ThreadImpl : public blink::WebThread
{
public:
    static blink::PlatformThreadId CurrentThreadId(void);
protected:
    ThreadImpl(void);
    ~ThreadImpl(void);
    void ApplyThreadId(blink::PlatformThreadId threadId);

    typedef std::lock_guard<std::recursive_mutex> AutoLock;
    std::recursive_mutex m_lock;
private:
    // blink::WebThread
    bool isCurrentThread(void) const override final { return CurrentThreadId() == threadId(); }
    blink::PlatformThreadId threadId(void) const override final { return m_threadId; }
    blink::WebScheduler* scheduler(void) const override final;

    blink::PlatformThreadId m_threadId = 0;
    std::unique_ptr<SchedulerImpl> m_scheduler;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_THREAD_IMPL_H
