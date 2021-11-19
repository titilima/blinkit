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
    void NewTimer(void);
    void ProcessTimer(size_t slot);

    struct TimerData;
    void InstallTimer(TimerData *timerData);

    const DWORD m_threadId;
    std::vector<HANDLE> m_timers;
    std::vector<std::function<void()>> m_tasks;

    class TaskRunnerImpl;
    mutable std::shared_ptr<TaskRunnerImpl> m_taskRunner;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_MESSAGE_LOOP_H
