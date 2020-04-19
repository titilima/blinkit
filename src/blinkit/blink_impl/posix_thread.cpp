// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_thread.cpp
// Description: PosixThread Class
//      Author: Ziming Li
//     Created: 2020-04-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "posix_thread.h"
#ifdef OS_LINUX
#   include <sys/syscall.h>
#endif

using namespace blink;

namespace BlinKit {

std::shared_ptr<base::SingleThreadTaskRunner> PosixThread::GetTaskRunner(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PlatformThreadId ThreadImpl::CurrentThreadId(void)
{
#if defined(OS_LINUX)
    return syscall(__NR_gettid);
#else
#   error Not implemented!
#endif
}

} // namespace BlinKit
