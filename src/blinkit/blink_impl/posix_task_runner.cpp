// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_task_runner.cpp
// Description: PosixTaskRunner Class
//      Author: Ziming Li
//     Created: 2020-04-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "posix_task_runner.h"

#include <pthread.h>

namespace BlinKit {

class DelayData
{
public:
    DelayData(const PosixTaskRunner::TaskPoster &taskPoster, const base::Location &location, const std::function<void()> &task, base::TimeDelta delay)
        : m_taskPoster(taskPoster)
        , m_location(location)
        , m_task(task)
        , m_delayInMs(delay.InMilliseconds())
    {
    }
    ~DelayData(void)
    {
        timespec t;
        t.tv_sec = m_delayInMs / 1000;
        t.tv_nsec = (m_delayInMs % 1000) * 1000000;
        nanosleep(&t, nullptr);
        m_taskPoster(m_location, m_task);
    }
private:
    const PosixTaskRunner::TaskPoster m_taskPoster;
    const base::Location m_location;
    const std::function<void()> m_task;
    int64_t m_delayInMs;
};

PosixTaskRunner::PosixTaskRunner(const TaskPoster &taskPoster) : m_taskPoster(taskPoster)
{
}

static void* DelayThread(void *arg)
{
    delete reinterpret_cast<DelayData *>(arg);
    return nullptr;
}

bool PosixTaskRunner::PostDelayedTask(const base::Location &fromHere, const std::function<void()> &task, base::TimeDelta delay)
{
    if (delay.is_zero())
    {
        m_taskPoster(fromHere, task);
    }
    else
    {
        DelayData *delayData = new DelayData(m_taskPoster, fromHere, task, delay);

        pthread_t t;
        if (0 != pthread_create(&t, nullptr, DelayThread, delayData))
        {
            ASSERT(false); // Error: Cannot create the delay thread!
            delete delayData;
            return false;
        }

        pthread_detach(t);
    }
    return true;
}

} // namespace BlinKit
