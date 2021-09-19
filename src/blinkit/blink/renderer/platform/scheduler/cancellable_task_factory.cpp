// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: cancellable_task_factory.cpp
// Description: CancellableTaskFactory Class
//      Author: Ziming Li
//     Created: 2021-09-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./cancellable_task_factory.h"

namespace blink {

class CancellableTaskFactory::CancellableTask final : public WebTaskRunner::Task
{
public:
    CancellableTask(const std::shared_ptr<TaskStatus> &status, const std::shared_ptr<Closure> &closure)
        : m_status(status), m_closure(closure)
    {
        ASSERT(Pending == *m_status);
    }
private:
    void run(void) override
    {
        if (Pending == *m_status)
        {
            (*m_closure)();
            *m_status = Finished;
        }
        else
        {
            ASSERT(Canceled == *m_status);
        }
    }

    std::shared_ptr<TaskStatus> m_status;
    const std::shared_ptr<Closure> m_closure;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CancellableTaskFactory::CancellableTaskFactory(const std::function<void()> &closure)
    : m_closure(std::make_shared<Closure>(closure))
{
}

CancellableTaskFactory::~CancellableTaskFactory(void)
{
    if (m_lastStatus)
        *m_lastStatus = Canceled;
}

WebTaskRunner::Task* CancellableTaskFactory::cancelAndCreate(void)
{
    if (m_lastStatus)
    {
        switch (*m_lastStatus)
        {
            case Pending:
                *m_lastStatus = Canceled;
                [[fallthrough]];
            case Canceled:
                m_lastStatus = std::make_shared<TaskStatus>(Pending);
                break;
            case Finished:
                *m_lastStatus = Pending;
                break;
            default:
                NOTREACHED();
        }
    }
    else
    {
        m_lastStatus = std::make_shared<TaskStatus>(Pending);
    }

    return new CancellableTask(m_lastStatus, m_closure);
}

} // namespace blink
