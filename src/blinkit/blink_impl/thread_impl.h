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

#include "third_party/blink/renderer/platform/scheduler/public/thread.h"

namespace BlinKit {

class SchedulerImpl;

class ThreadImpl : public blink::Thread
{
public:
    static blink::PlatformThreadId CurrentThreadId(void);
#ifndef NDEBUG
    static void SetName(const char *name);
#endif

    blink::PlatformThreadId ThreadId(void) const final
    {
        ASSERT(0 != m_threadId); // BKTODO: Initialize id in other threads.
        return m_threadId;
    }
protected:
    ThreadImpl(void) = default;
    ~ThreadImpl(void) override;

    bool IsCurrentThread(void) const final;

    blink::PlatformThreadId m_threadId = 0;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_THREAD_IMPL_H
