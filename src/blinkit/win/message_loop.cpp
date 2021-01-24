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

#include "message_loop.h"

#include <optional>
#include "blinkit/win/message_task.h"

namespace BlinKit {

struct MessageLoop::TimerData {
    TimerData(std::function<void()> &&t, const base::TimeDelta &delay)
        : task(std::move(t)), startTick(GetTickCount()), delayInMs(delay.InMilliseconds())
    {
    }

    std::function<void()> task;
    DWORD startTick;
    DWORD delayInMs;
};

class MessageLoop::TaskRunnerImpl : public base::SingleThreadTaskRunner
{
public:
    TaskRunnerImpl(MessageLoop &loop) : m_loop(loop) {}
private:
    // TaskRunner overrides
    bool PostDelayedTask(const base::Location &fromHere, std::function<void()> &&task, base::TimeDelta delay) override
    {
        if (delay.is_zero())
            return m_loop.PostTask(fromHere, std::move(task));

        TimerData *timerData = new TimerData(std::move(task), delay);
        auto timerTask = std::bind(&MessageLoop::InstallTimer, &m_loop, timerData);
        if (m_loop.PostTask(fromHere, std::move(timerTask)))
            return true;

        delete timerData;
        return false;
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

std::shared_ptr<base::SingleThreadTaskRunner> MessageLoop::GetTaskRunner(void) const
{
    return std::make_shared<TaskRunnerImpl>(const_cast<MessageLoop &>(*this));
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

    DWORD delta = GetTickCount() - timerData->startTick;
    LARGE_INTEGER delay = { 0 };
    delay.QuadPart -= (timerData->delayInMs - delta) * 10000;
    SetWaitableTimer(m_timers.at(slot), &delay, 0, nullptr, nullptr, FALSE);

    delete timerData;
}

void MessageLoop::NewTimer(void)
{
    HANDLE hTimer = CreateWaitableTimer(nullptr, FALSE, nullptr);
    m_timers.push_back(hTimer);
    m_tasks.push_back(std::function<void()>());
}

bool MessageLoop::PostTask(const base::Location &, std::function<void()> &&task)
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
