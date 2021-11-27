// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: message_loop.h
// Description: MessageLoop Class
//      Author: Ziming Li
//     Created: 2021-01-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_MESSAGE_LOOP_H
#define BLINKIT_BLINKIT_MESSAGE_LOOP_H

#pragma once

#include <list>
#include <unordered_set>
#include "bk_app.h"
#include "blinkit/blink/public/platform/WebTaskRunner.h"

namespace BlinKit {

class MessageLoop
{
public:
    MessageLoop(void);
    ~MessageLoop(void);

    int Run(BkMessageFilter filter, void *userData);

    std::shared_ptr<blink::WebTaskRunner> GetTaskRunner(void) const;
    bool PostTask(const blink::WebTraceLocation &loc, std::function<void()> &&task);
private:
    struct TimerData;
    struct TimerTaskData;

    void InstallTimer(TimerData *timerData);
    void SetTimer(const LARGE_INTEGER &dueTime, std::function<void()> &&task);
    HANDLE RequireTimer(void);
    void OnTimerFired(HANDLE hTimer, const LARGE_INTEGER &tick);
    static void APIENTRY TimerCallback(PVOID arg, DWORD low, DWORD high);

    static bool IsLowPriority(const LARGE_INTEGER &dueTime);
    void ResumeTimerScheduler(void);
    void ScheduleTimers(void);
    static void APIENTRY TimerSchedulerCallback(PVOID arg, DWORD, DWORD);

    void Cleanup(const LARGE_INTEGER &tick);
    static void APIENTRY CleanupCallback(PVOID arg, DWORD low, DWORD high);

    const DWORD m_threadId;
    HANDLE m_hEvent[1]; // Just a placeholder for waiting.

    std::unordered_set<HANDLE> m_activeTimers;
    std::list<std::pair<HANDLE, LARGE_INTEGER>> m_alternateTimers; // FILO for reusage.

    HANDLE m_hSchedulerTimer;
    bool m_schedulerRunning = false;
    std::unordered_set<TimerData *> m_lowPriorityTimers;

    HANDLE m_hCleanupTimer = nullptr;

    class TaskRunnerImpl;
    mutable std::shared_ptr<TaskRunnerImpl> m_taskRunner;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_MESSAGE_LOOP_H
