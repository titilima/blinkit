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

#include "base/single_thread_task_runner.h"

namespace BlinKit {

class MessageLoop
{
public:
    MessageLoop(void);
    ~MessageLoop(void);

    int Run(void);

    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const;
    bool PostTask(const base::Location &fromHere, std::function<void()> &&task);
private:
    void NewTimer(void);
    void ProcessTimer(size_t slot);

    struct TimerData;
    void InstallTimer(TimerData *timerData);

    const DWORD m_threadId;
    std::vector<HANDLE> m_timers;
    std::vector<std::function<void()>> m_tasks;

    class TaskRunnerImpl;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_MESSAGE_LOOP_H
