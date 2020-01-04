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

struct TaskData {
    DWORD startTick;
    std::function<void()> task;
    DWORD delayInMs;
};

static const UINT TaskMessage = RegisterWindowMessage(TEXT("BkTaskMessage"));

WinSingleThreadTaskRunner::WinSingleThreadTaskRunner(void) : m_threadId(GetCurrentThreadId())
{
}

void WinSingleThreadTaskRunner::OnTimer(HWND, UINT, UINT_PTR idEvent, DWORD)
{
    std::shared_ptr<base::SingleThreadTaskRunner> currentRunner = Platform::Current()->CurrentThread()->GetTaskRunner();
    static_cast<WinSingleThreadTaskRunner *>(currentRunner.get())->ProcessTimer(idEvent);
}

bool WinSingleThreadTaskRunner::PostDelayedTask(const base::Location &fromHere, const std::function<void()> &task, base::TimeDelta delay)
{
    DWORD delayInMs = delay.InMilliseconds();
    if (GetCurrentThreadId() == m_threadId)
    {
        if (0 != delayInMs)
            return SetTimer(task, delayInMs);
    }

    std::unique_ptr<TaskData> taskData = std::make_unique<TaskData>();
    taskData->startTick = GetTickCount();
    taskData->task = task;
    taskData->delayInMs = delayInMs;
    if (PostThreadMessage(m_threadId, TaskMessage, 0, reinterpret_cast<LPARAM>(taskData.get())))
    {
        taskData.release();
        return true;
    }

    ASSERT(false); // Post message failed!
    return false;
}

bool WinSingleThreadTaskRunner::ProcessMessage(const MSG &msg)
{
    ASSERT(GetCurrentThreadId() == m_threadId);
    if (msg.message != TaskMessage)
        return false;

    std::unique_ptr<TaskData> taskData(reinterpret_cast<TaskData *>(msg.lParam));
    if (0 == taskData->delayInMs)
    {
        taskData->task();
    }
    else
    {
        DWORD delta = GetTickCount() - taskData->startTick;
        SetTimer(taskData->task, delta);
    }
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

bool WinSingleThreadTaskRunner::SetTimer(const std::function<void()> &task, DWORD delayInMs)
{
    ASSERT(GetCurrentThreadId() == m_threadId);

    UINT_PTR timerId = ::SetTimer(nullptr, 0, delayInMs, OnTimer);
    if (0 == timerId)
    {
        ASSERT(0 != timerId);
        return false;
    }

    m_tasks[timerId] = task;
    return true;
}

} // namespace BlinKit
