// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: message_loop.cpp
// Description: MessageLoop Class
//      Author: Ziming Li
//     Created: 2021-01-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./message_loop.h"

#include <optional>
#include "blinkit/win/message_task.h"
#include "chromium/base/time/time.h"

using namespace blink;

namespace BlinKit {

struct MessageLoop::TimerData {
    TimerData(std::function<void()> &&t, const base::TimeTicks &desiredFireTick)
        : task(std::move(t)), fireTick(desiredFireTick)
    {
    }

    std::function<void()> task;
    base::TimeTicks fireTick;
};

class MessageLoop::TaskRunnerImpl final : public WebTaskRunner
{
public:
    TaskRunnerImpl(MessageLoop &loop) : m_loop(loop) {}
private:
    static void RunTask(Task *task)
    {
        task->run();
        delete task;
    }
    // TaskRunner overrides
    void postTask(const WebTraceLocation &loc, Task *task) override
    {
        m_loop.PostTask(loc, std::bind(&TaskRunnerImpl::RunTask, task));
    }
    void postDelayedTask(const WebTraceLocation &loc, Task *task, double delayMs) override
    {
        base::TimeTicks desiredTick = base::TimeTicks::Now();
        desiredTick += base::TimeDelta::FromMillisecondsD(delayMs);

        TimerData *timerData = new TimerData(std::bind(&TaskRunnerImpl::RunTask, task), desiredTick);
        if (GetCurrentThreadId() == m_loop.m_threadId)
        {
            m_loop.InstallTimer(timerData);
            return;
        }
        else
        {
            auto timerTask = std::bind(&MessageLoop::InstallTimer, &m_loop, timerData);
            if (m_loop.PostTask(loc, std::move(timerTask)))
                return;
        }

        delete timerData;
    }

    MessageLoop &m_loop;
};

MessageLoop::MessageLoop(void) : m_threadId(::GetCurrentThreadId())
{
    NewTimer();
}

MessageLoop::~MessageLoop(void)
{
    for (size_t i = 0; i < m_tasks.size(); ++i)
    {
        HANDLE hTimer = m_timers.at(i);
        if (m_tasks.at(i))
            CancelWaitableTimer(hTimer);
        CloseHandle(hTimer);
    }
}

std::shared_ptr<WebTaskRunner> MessageLoop::GetTaskRunner(void) const
{
    if (!m_taskRunner)
        m_taskRunner = std::make_shared<TaskRunnerImpl>(const_cast<MessageLoop &>(*this));
    return m_taskRunner;
}

void MessageLoop::InstallTimer(TimerData *timerData)
{
    ASSERT(m_timers.size() == m_tasks.size());

    size_t slot;
    for (slot = 0; slot < m_tasks.size(); ++slot)
    {
        if (!m_tasks.at(slot))
            break;
    }

    if (m_timers.size() == slot)
        NewTimer();
    m_tasks[slot] = std::move(timerData->task);

    base::TimeDelta delta = base::TimeTicks::Now() - timerData->fireTick;
    LARGE_INTEGER delay = { 0 };
    delay.QuadPart -= delta.InMicroseconds() * 10;
    SetWaitableTimer(m_timers.at(slot), &delay, 0, nullptr, nullptr, FALSE);

    delete timerData;
}

void MessageLoop::NewTimer(void)
{
    HANDLE hTimer = CreateWaitableTimer(nullptr, FALSE, nullptr);
    m_timers.push_back(hTimer);
    m_tasks.push_back(std::function<void()>());
}

bool MessageLoop::PostTask(const WebTraceLocation &, std::function<void()> &&task)
{
    return MessageTask::Post(m_threadId, std::move(task));
}

void MessageLoop::ProcessTimer(size_t slot)
{
    ASSERT(slot < m_tasks.size());

    std::function<void()> task;
    std::swap(task, m_tasks.at(slot));
    task();
}

int MessageLoop::Run(void)
{
    std::optional<int> ret;
    while (!ret.has_value())
    {
        DWORD timerCount = m_timers.size();
        DWORD dwWait = MsgWaitForMultipleObjects(timerCount, m_timers.data(), FALSE, INFINITE, QS_ALLINPUT);
        if (WAIT_OBJECT_0 <= dwWait && dwWait < WAIT_OBJECT_0 + timerCount)
        {
            ProcessTimer(dwWait - WAIT_OBJECT_0);
            continue;
        }

        ASSERT(dwWait == timerCount);

        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
            {
                ret = msg.wParam;
                break;
            }

            if (MessageTask::Process(msg.message, msg.wParam, msg.lParam))
                continue;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return ret.value();
}

} // namespace BlinKit
