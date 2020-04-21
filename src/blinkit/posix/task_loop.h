// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: task_loop.h
// Description: TaskLoop Class
//      Author: Ziming Li
//     Created: 2020-04-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_TASK_LOOP_H
#define BLINKIT_BLINKIT_TASK_LOOP_H

#pragma once

#include <optional>
#include <pthread.h>
#include <queue>

namespace base {
class SingleThreadTaskRunner;
}

namespace BlinKit {

class TaskLoop
{
public:
    TaskLoop(void);
    ~TaskLoop(void);

    int Run(void);
    void Exit(int code);

    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void);
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    std::optional<int> m_exitCode;

    class TaskData;
    std::queue<TaskData *> m_taskQueue;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_TASK_LOOP_H
