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

#include "blinkit/blink_impl/win_single_thread_task_runner.h"

using namespace blink;

namespace BlinKit {

WinThread::~WinThread(void) = default;

std::shared_ptr<base::SingleThreadTaskRunner> WinThread::GetTaskRunner(void) const
{
    if (!m_taskRunner)
        m_taskRunner = std::make_shared<WinSingleThreadTaskRunner>();
    return m_taskRunner;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PlatformThreadId ThreadImpl::CurrentThreadId(void)
{
    return ::GetCurrentThreadId();
}

} // namespace BlinKit
