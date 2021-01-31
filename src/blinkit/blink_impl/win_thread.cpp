// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_thread.cpp
// Description: WinThread Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "blinkit/blink_impl/thread_impl.h"

using namespace blink;

namespace BlinKit {

PlatformThreadId ThreadImpl::CurrentThreadId(void)
{
    return ::GetCurrentThreadId();
}

#ifndef NDEBUG
void ThreadImpl::SetName(const char *name)
{
    struct THREADNAME_INFO {
        DWORD dwType = 0x1000;
        PCSTR szName;
        DWORD dwThreadID;
        DWORD dwFlags = 0;
    } info;

    info.szName = name;
    info.dwThreadID = GetCurrentThreadId();
    __try
    {
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), reinterpret_cast<ULONG_PTR *>(&info));
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {
    }
}
#endif

} // namespace BlinKit
