// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: task_loop.cpp
// Description: TaskLoop Class
//      Author: Ziming Li
//     Created: 2020-04-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "task_loop.h"

#include "blinkit/blink_impl/posix_task_runner.h"

namespace BlinKit {

class TaskLoop::TaskData
{
public:
    TaskData(const base::Location &location, const std::function<void()> &task) : m_location(location), m_task(task) {}
    ~TaskData(void)
    {
        if (!m_discard)
            m_task();
    }

    void Discard(void) { m_discard = true; }
private:
    bool m_discard = false;
    const base::Location m_location;
    const std::function<void()> m_task;
};

TaskLoop::TaskLoop(void)
{
    pthread_mutex_init(&m_mutex, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}

TaskLoop::~TaskLoop(void)
{
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);

    while (!m_taskQueue.empty())
    {
        TaskData *data = m_taskQueue.front();
        m_taskQueue.pop();

        data->Discard();
        delete data;
    }
}

void TaskLoop::Exit(int code)
{
    pthread_mutex_lock(&m_mutex);
    m_exitCode = code;
    pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);
}

std::shared_ptr<base::SingleThreadTaskRunner> TaskLoop::GetTaskRunner(void)
{
    const auto taskPoster = [this](const base::Location &location, const std::function<void()> &task)
    {
        TaskData *taskData = new TaskData(location, task);
        pthread_mutex_lock(&m_mutex);
        m_taskQueue.push(taskData);
        pthread_cond_signal(&m_cond);
        pthread_mutex_unlock(&m_mutex);
    };
    return std::make_shared<PosixTaskRunner>(taskPoster);
}

int TaskLoop::Run(void)
{
    for (;;)
    {
        TaskData *taskData = nullptr;

        pthread_mutex_lock(&m_mutex);
        while (m_taskQueue.empty() && !m_exitCode.has_value())
            pthread_cond_wait(&m_cond, &m_mutex);
        if (!m_taskQueue.empty())
        {
            taskData = m_taskQueue.front();
            m_taskQueue.pop();
        }
        pthread_mutex_unlock(&m_mutex);

        if (m_exitCode.has_value())
            break;

        ASSERT(nullptr != taskData);
        delete taskData;
    }

    ASSERT(m_exitCode.has_value());
    return m_exitCode.value();
}

} // namespace BlinKit
