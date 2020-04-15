// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_thread.cpp
// Description: AppleThread Class
//      Author: Ziming Li
//     Created: 2019-08-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "apple_thread.h"

#include <pthread.h>

using namespace blink;

namespace BlinKit {

std::shared_ptr<base::SingleThreadTaskRunner> AppleThread::GetTaskRunner(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PlatformThreadId ThreadImpl::CurrentThreadId(void)
{
    return pthread_mach_thread_np(pthread_self());
}

} // namespace BlinKit
