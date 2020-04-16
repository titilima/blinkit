// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_task_runner.cpp
// Description: Task Runner Classes
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "apple_task_runner.h"

namespace BlinKit {

class TaskData
{
public:
    TaskData(const base::Location &location, const std::function<void()> &task) : m_location(location), m_task(task) {}
    ~TaskData(void) { m_task(); }
private:
    base::Location m_location;
    std::function<void()> m_task;
};

bool MainThreadTaskRunner::PostDelayedTask(const base::Location &fromHere, const std::function<void()> &task, base::TimeDelta delay)
{
    dispatch_time_t when = dispatch_time(DISPATCH_TIME_NOW, delay.InMilliseconds() * NSEC_PER_MSEC);
    dispatch_after_f(when, dispatch_get_main_queue(), new TaskData(fromHere, task), Worker);
    return true;
}

void MainThreadTaskRunner::Worker(void *data)
{
    delete reinterpret_cast<TaskData *>(data);
}

} // namespace BlinKit
