// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_task_runner.h
// Description: WinTaskRunner Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_TASK_RUNNER_H
#define BLINKIT_BLINKIT_WIN_TASK_RUNNER_H

#pragma once

#include "task_runner_impl.h"

namespace BlinKit {

class WinTaskRunner final : public TaskRunnerImpl
{
public:
    WinTaskRunner(ThreadImpl &thread);

    bool ProcessMessage(const MSG &msg);
    static LPARAM CreateTaskData(Task *task, DWORD delayMs);
private:
    void SetTimer(UINT uElapse, Task *task);
    void ProcessTimer(UINT_PTR idEvent);
    static void CALLBACK OnTimer(HWND h, UINT msg, UINT_PTR idEvent, DWORD dwTime);

    // blink::WebTaskRunner
    void postTask(const blink::WebTraceLocation &location, Task *task) override;
    void postDelayedTask(const blink::WebTraceLocation &location, Task *task, double delayMs) override;
    blink::WebTaskRunner* clone(void) override;

    std::unordered_map<UINT_PTR, Task *> m_tasks;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_TASK_RUNNER_H
