// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: communicator_manager.cpp
// Description: CommunicatorManager Classes
//      Author: Ziming Li
//     Created: 2021-01-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "communicator_manager.h"

#include "base/single_thread_task_runner.h"
#include "blinkit/crawler/crawler_communicator.h"

namespace BlinKit {

#ifdef BLINKIT_CRAWLER_ONLY
class ExclusiveCommunicatorManager::CommunicatorImpl : public CrawlerCommunicator
{
public:
    CommunicatorImpl(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner) : m_taskRunner(taskRunner) {}
private:
    void CallCrawler(const base::Location &, std::function<void()> &&worker, CallMode) override
    {
        worker();
    }
    void PostToClient(const base::Location &loc, std::function<void()> &&worker) override
    {
        m_taskRunner->PostTask(loc, std::move(worker));
    }

    std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;
};

ExclusiveCommunicatorManager::ExclusiveCommunicatorManager(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner)
    : m_communicator(std::make_shared<CommunicatorImpl>(taskRunner))
{
}

std::shared_ptr<CrawlerCommunicator> ExclusiveCommunicatorManager::AcquireCommunicator(void)
{
    return m_communicator;
}
#endif

} // namespace BlinKit
