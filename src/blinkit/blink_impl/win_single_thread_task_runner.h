// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_single_thread_task_runner.h
// Description: WinSingleThreadTaskRunner Class
//      Author: Ziming Li
//     Created: 2019-11-23
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_SINGLE_THREAD_TASK_RUNNER_H
#define BLINKIT_BLINKIT_WIN_SINGLE_THREAD_TASK_RUNNER_H

#pragma once

#include <unordered_map>
#include "base/single_thread_task_runner.h"

namespace BlinKit {

class WinSingleThreadTaskRunner final : public base::SingleThreadTaskRunner
{
public:
private:
    void ProcessTimer(UINT_PTR timerId);
    static void CALLBACK OnTimer(HWND h, UINT msg, UINT_PTR idEvent, DWORD dwTime);

    // TaskRunner overrides
    bool PostDelayedTask(const base::Location &fromHere, const std::function<void()> &task, base::TimeDelta delay) override;

    std::unordered_map<UINT_PTR, std::function<void()>> m_tasks;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_SINGLE_THREAD_TASK_RUNNER_H
