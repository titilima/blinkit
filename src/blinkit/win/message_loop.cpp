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
#include "blinkit/ui/animation/animation_scheduler.h"
#include "chromium/base/time/time.h"
#include "third_party/zed/include/zed/container_utilites.hpp"
#include "third_party/zed/include/zed/float.hpp"

using namespace blink;

namespace BlinKit {

struct MessageLoop::TimerData {
    TimerData(WebTaskRunner::Task *t, const base::TimeTicks &desiredFireTick)
        : task(t), m_fireTick(desiredFireTick)
    {
    }

    LARGE_INTEGER CalculateDueTime(const base::TimeTicks &tick = base::TimeTicks::Now()) const
    {
        base::TimeDelta delta = m_fireTick - tick;

        LARGE_INTEGER ret = { 0 };
        ret.QuadPart -= delta.InMicroseconds() * 10;
        return ret;
    }

    std::unique_ptr<WebTaskRunner::Task> task;
private:
    base::TimeTicks m_fireTick;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MessageLoop::TimerTaskData {
    TimerTaskData(MessageLoop &theLoop, HANDLE hTimer, std::unique_ptr<WebTaskRunner::Task> &&task)
        : m_theLoop(theLoop), m_hTimer(hTimer), m_task(std::move(task))
    {
    }

    void Run(const LARGE_INTEGER &tick)
    {
        m_task->run();
        m_theLoop.OnTimerFired(m_hTimer, tick);
        delete this;
    }
private:
    MessageLoop &m_theLoop;
    HANDLE m_hTimer;
    std::unique_ptr<WebTaskRunner::Task> m_task;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MessageLoop::TaskRunnerImpl final : public WebTaskRunner
{
public:
    TaskRunnerImpl(MessageLoop &loop) : m_loop(loop), m_threadId(::GetCurrentThreadId()){}
private:
    // TaskRunner overrides
    void postTask(const WebTraceLocation &, Task *task) override
    {
        m_loop.AddTask(task);
    }
    void postDelayedTask(const WebTraceLocation &loc, Task *task, double delayMs) override
    {
        if (zed::almost_equals(delayMs, 0.0))
        {
            m_loop.AddTask(task);
            return;
        }

        base::TimeTicks desiredTick = base::TimeTicks::Now();
        desiredTick += base::TimeDelta::FromMillisecondsD(delayMs);

        TimerData *timerData = new TimerData(task, desiredTick);
        if (GetCurrentThreadId() == m_threadId)
        {
            m_loop.InstallTimer(timerData);
            return;
        }
        else
        {
            ASSERT(false); // BKTODO:
#if 0
            auto timerTask = std::bind(&MessageLoop::InstallTimer, &m_loop, timerData);
            if (m_loop.PostTask(loc, std::move(timerTask)))
                return;
#endif
        }

        delete timerData;
    }

    MessageLoop &m_loop;
    const DWORD m_threadId;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MessageLoop::MessageLoop(void)
    : m_taskEvent(::CreateEvent(nullptr, FALSE, FALSE, nullptr))
    , m_hSchedulerTimer(CreateWaitableTimer(nullptr, FALSE, nullptr))
#ifdef BLINKIT_UI_ENABLED
    , m_animationScheduler(std::make_unique<AnimationScheduler>())
#endif
{
}

MessageLoop::~MessageLoop(void)
{
    for (HANDLE hTimer : m_activeTimers)
    {
        CancelWaitableTimer(hTimer);
        CloseHandle(hTimer);
    }

    for (const auto &[hTimer, _] : m_alternateTimers)
        CloseHandle(hTimer);

    if (m_schedulerRunning)
        CancelWaitableTimer(m_hSchedulerTimer);
    CloseHandle(m_hSchedulerTimer);

    CloseHandle(m_taskEvent);
}

void MessageLoop::AddTask(WebTaskRunner::Task *task)
{
    auto _ = m_taskLock.guard();
    m_tasks.emplace(task);
}

void MessageLoop::Cleanup(const LARGE_INTEGER &tick)
{
    auto it = m_alternateTimers.begin();
    while (m_alternateTimers.end() != it)
    {
        LONGLONG idle = tick.QuadPart - it->second.QuadPart;
        idle /= 1000 * 10; // => ms
        if (idle < 2000)
        {
            ++it;
        }
        else
        {
            CloseHandle(it->first);
            it = m_alternateTimers.erase(it);
        }
    }

    CloseHandle(m_hCleanupTimer);
    m_hCleanupTimer = nullptr;
}

void APIENTRY MessageLoop::CleanupCallback(PVOID arg, DWORD low, DWORD high)
{
    LARGE_INTEGER tick;
    tick.LowPart = low;
    tick.HighPart = high;
    reinterpret_cast<MessageLoop *>(arg)->Cleanup(tick);
}

std::shared_ptr<WebTaskRunner> MessageLoop::GetTaskRunner(void) const
{
    if (!m_taskRunner)
        m_taskRunner = std::make_shared<TaskRunnerImpl>(const_cast<MessageLoop &>(*this));
    return m_taskRunner;
}

void MessageLoop::InstallTimer(TimerData *timerData)
{
    LARGE_INTEGER dueTime = timerData->CalculateDueTime();
    if (IsLowPriority(dueTime))
    {
        m_lowPriorityTimers.emplace(timerData);
        if (!m_schedulerRunning)
            ResumeTimerScheduler();
    }
    else
    {
        std::unique_ptr<TimerData> _(timerData);
        SetTimer(dueTime, std::move(timerData->task));
    }
}

bool MessageLoop::IsLowPriority(const LARGE_INTEGER &dueTime)
{
    static constexpr LONGLONG limit = 110 * 1000 * 10; // 110ms
    return limit + dueTime.QuadPart < 0;
}

void MessageLoop::OnTimerFired(HANDLE hTimer, const LARGE_INTEGER &tick)
{
    ASSERT(zed::key_exists(m_activeTimers, hTimer));
    m_activeTimers.erase(hTimer);
    m_alternateTimers.emplace_front(hTimer, tick);
}

HANDLE MessageLoop::RequireTimer(void)
{
    HANDLE hTimer;

    if (!m_alternateTimers.empty())
    {
        auto it = m_alternateTimers.begin();
        hTimer = it->first;
        m_alternateTimers.erase(it);
    }
    else
    {
        hTimer = CreateWaitableTimer(nullptr, FALSE, nullptr);
    }

    ASSERT(nullptr != hTimer);
    m_activeTimers.emplace(hTimer);
    return hTimer;
}

void MessageLoop::ResumeTimerScheduler(void)
{
    LARGE_INTEGER dueTime = { 0 };
    dueTime.QuadPart = -(100 * 1000 * 10); // 100ms
    SetWaitableTimer(m_hSchedulerTimer, &dueTime, 0, TimerSchedulerCallback, this, FALSE);

    m_schedulerRunning = true;
}

int MessageLoop::Run(BkMessageFilter filter, void *userData)
{
    std::optional<int> ret;
    while (!ret.has_value())
    {
        DWORD dwWait = MsgWaitForMultipleObjectsEx(1, &m_taskEvent, INFINITE, QS_ALLINPUT,
            MWMO_ALERTABLE | MWMO_INPUTAVAILABLE);
#ifndef NDEBUG
        if (WAIT_FAILED == dwWait)
        {
            DWORD dwError = GetLastError();
            ASSERT(WAIT_FAILED != dwWait);
        }
#endif
        RunTasks();

        bool animationScheduled = false;

        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
            {
                ret = msg.wParam;
                break;
            }

            if (filter(&msg, userData))
                continue;

            TranslateMessage(&msg);
            DispatchMessage(&msg);

            m_animationScheduler->ScheduleAnimations();
            animationScheduled = true;
        }

        if (!animationScheduled)
            m_animationScheduler->ScheduleAnimations();
    }
    return ret.value();
}

void MessageLoop::RunTasks(void)
{
    TaskQueue tasks;
    {
        auto _ = m_taskLock.guard();
        if (m_tasks.empty())
            return;

        tasks.swap(m_tasks);
    }

    while (!tasks.empty())
    {
        tasks.front()->run();
        tasks.pop();
    }
}

void MessageLoop::ScheduleTimers(void)
{
    const base::TimeTicks now = base::TimeTicks::Now();

    ASSERT(!m_lowPriorityTimers.empty());
    auto it = m_lowPriorityTimers.begin();
    while (m_lowPriorityTimers.end() != it)
    {
        TimerData *data = *it;

        LARGE_INTEGER dueTime = data->CalculateDueTime(now);
        if (!IsLowPriority(dueTime))
        {
            std::unique_ptr<TimerData> _(data);
            SetTimer(dueTime, std::move(data->task));
            it = m_lowPriorityTimers.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (!m_lowPriorityTimers.empty())
    {
        ResumeTimerScheduler();
        return;
    }

    m_schedulerRunning = false;
    if (nullptr == m_hCleanupTimer)
    {
        m_hCleanupTimer = CreateWaitableTimer(nullptr, FALSE, nullptr);

        LARGE_INTEGER dueTime = { 0 };
        dueTime.QuadPart = -(5 * 1000 * 1000 * 10); // 5s
        SetWaitableTimer(m_hCleanupTimer, &dueTime, 0, CleanupCallback, this, FALSE);
    }
}

void MessageLoop::SetTimer(const LARGE_INTEGER &dueTime, std::unique_ptr<WebTaskRunner::Task> &&task)
{
    HANDLE hTimer = RequireTimer();

    TimerTaskData *taskData = new TimerTaskData(*this, hTimer, std::move(task));
    SetWaitableTimer(hTimer, &dueTime, 0, TimerCallback, taskData, FALSE);
}

MessageLoop::TaskQueue MessageLoop::TakeTasks(void)
{
    TaskQueue ret;
    auto _ = m_taskLock.guard();
    ret.swap(m_tasks);
    return ret;
}

void APIENTRY MessageLoop::TimerCallback(PVOID arg, DWORD low, DWORD high)
{
    LARGE_INTEGER tick;
    tick.HighPart = high;
    tick.LowPart = low;
    reinterpret_cast<TimerTaskData *>(arg)->Run(tick);
}

void APIENTRY MessageLoop::TimerSchedulerCallback(PVOID arg, DWORD, DWORD)
{
    reinterpret_cast<MessageLoop *>(arg)->ScheduleTimers();
}

} // namespace BlinKit
