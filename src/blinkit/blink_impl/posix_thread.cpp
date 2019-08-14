// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_thread.cpp
// Description: PosixThread Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "posix_thread.h"

using namespace blink;

namespace BlinKit {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ThreadImpl* ThreadImpl::CreateInstance(const char *name)
{
    assert(false); // BKTODO:
    return new PosixThread;
}

PlatformThreadId ThreadImpl::CurrentThreadId(void)
{
#if (OS_MACOSX || OS_IOS)
    return pthread_mach_thread_np(pthread_self());
#elif (OS_LINUX)
    return syscall(__NR_gettid);
#else
    return reinterpret_cast<int64_t>(pthread_self());
#endif
}

} // namespace BlinKit
