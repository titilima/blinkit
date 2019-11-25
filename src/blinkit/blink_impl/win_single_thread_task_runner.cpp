// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_single_thread_task_runner.cpp
// Description: WinSingleThreadTaskRunner Class
//      Author: Ziming Li
//     Created: 2019-11-23
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "win_single_thread_task_runner.h"

#include "third_party/blink/public/platform/platform.h"

using namespace blink;

namespace BlinKit {

void WinSingleThreadTaskRunner::OnTimer(HWND, UINT, UINT_PTR idEvent, DWORD)
{
    std::shared_ptr<base::SingleThreadTaskRunner> currentRunner = Platform::Current()->CurrentThread()->GetTaskRunner();
    static_cast<WinSingleThreadTaskRunner *>(currentRunner.get())->ProcessTimer(idEvent);
}

bool WinSingleThreadTaskRunner::PostDelayedTask(const base::Location &fromHere, const std::function<void()> &task, base::TimeDelta delay)
{
    UINT_PTR timerId = SetTimer(nullptr, 0, delay.InMilliseconds(), OnTimer);
    m_tasks[timerId] = task;
    return true;
}

void WinSingleThreadTaskRunner::ProcessTimer(UINT_PTR timerId)
{
    auto it = m_tasks.find(timerId);
    if (std::end(m_tasks) != it)
    {
        KillTimer(nullptr, timerId);
        it->second();
        m_tasks.erase(it);
    }
    else
    {
        ASSERT(std::end(m_tasks) != it);
    }
}

} // namespace BlinKit
