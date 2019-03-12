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

#include "win_thread.h"

using namespace blink;

namespace BlinKit {

PlatformThreadId ThreadImpl::CurrentThreadId(void)
{
    return ::GetCurrentThreadId();
}

} // namespace BlinKit
