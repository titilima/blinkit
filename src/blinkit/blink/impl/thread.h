// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: thread.h
// Description: Thread Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_THREAD_H
#define BLINKIT_BLINKIT_THREAD_H

#pragma once

#include "blinkit/blink/public/platform/WebThread.h"

namespace BlinKit {

class Thread : public blink::WebThread
{
public:
    blink::PlatformThreadId threadId(void) const final
    {
        ASSERT(0 != m_threadId); // BKTODO: Initialize id in other threads.
        return m_threadId;
    }
protected:
    Thread(void) = default;

    bool isCurrentThread(void) const final;

    blink::PlatformThreadId m_threadId = 0;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_THREAD_H
