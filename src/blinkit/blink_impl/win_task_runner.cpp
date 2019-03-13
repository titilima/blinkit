// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_task_runner.cpp
// Description: WinTaskRunner Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "win_task_runner.h"

#include "app/app_impl.h"
#include "blink_impl/thread_impl.h"

using namespace blink;

namespace BlinKit {

struct TaskData {
    DWORD startTick;
    WebTaskRunner::Task *threadTask;
    DWORD delayMs;
};

static const UINT TaskMessage = RegisterWindowMessage(TEXT("BkTaskMessage"));

WinTaskRunner::WinTaskRunner(ThreadImpl &thread) : TaskRunnerImpl(thread)
{
    // Nothing
}

WebTaskRunner* WinTaskRunner::clone(void)
{
    return new WinTaskRunner(m_thread);
}

LPARAM WinTaskRunner::CreateTaskData(Task *task, DWORD delayMs)
{
    TaskData *td = new TaskData;
    td->startTick = 0 == delayMs ? 0 : GetTickCount();
    td->threadTask = task;
    td->delayMs = delayMs;
    return reinterpret_cast<LPARAM>(td);
}

void CALLBACK WinTaskRunner::OnTimer(HWND, UINT, UINT_PTR idEvent, DWORD)
{
    WinTaskRunner *This = static_cast<WinTaskRunner *>(AppImpl::Get().currentThread()->taskRunner());
    This->ProcessTimer(idEvent);
}

void WinTaskRunner::postDelayedTask(const WebTraceLocation &location, Task *task, double delayMs)
{
    const DWORD threadId = m_thread.threadId();
    if (GetCurrentThreadId() == threadId)
        SetTimer(static_cast<UINT>(delayMs), task);
    else
        PostThreadMessage(threadId, TaskMessage, 0, CreateTaskData(task, static_cast<DWORD>(delayMs)));
}

void WinTaskRunner::postTask(const WebTraceLocation &location, Task *task)
{
    PostThreadMessage(m_thread.threadId(), TaskMessage, 0, CreateTaskData(task, 0));
}

bool WinTaskRunner::ProcessMessage(const MSG &msg)
{
    if (TaskMessage != msg.message)
        return false;

    TaskData *td = reinterpret_cast<TaskData *>(msg.lParam);
    if (0 == td->delayMs)
    {
        assert(0 == td->startTick);
        Run(td->threadTask);
    }
    else
    {
        DWORD delta = GetTickCount() - td->startTick;
        SetTimer(td->delayMs - delta, td->threadTask);
    }
    delete td;
    return true;
}

void WinTaskRunner::ProcessTimer(UINT_PTR idEvent)
{
    auto it = m_tasks.find(idEvent);
    if (std::end(m_tasks) != it)
    {
        ::KillTimer(nullptr, idEvent);

        Run(it->second);
    }
    else
    {
        assert(std::end(m_tasks) != it);
    }
}

void WinTaskRunner::Run(Task *task)
{
    m_thread.WillProcessTask();
    task->run();
    m_thread.DidProcessTask();
}

void WinTaskRunner::SetTimer(UINT uElapse, Task *task)
{
    assert(GetCurrentThreadId() == m_thread.threadId());
    UINT_PTR idEvent = ::SetTimer(nullptr, 0, uElapse, OnTimer);
    m_tasks[idEvent] = task;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<WebTaskRunner> TaskRunnerImpl::CreateInstance(ThreadImpl &thread)
{
    return std::make_unique<WinTaskRunner>(thread);
}

} // namespace BlinKit
