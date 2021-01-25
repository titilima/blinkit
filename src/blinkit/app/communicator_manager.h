// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: communicator_manager.h
// Description: CommunicatorManager Classes
//      Author: Ziming Li
//     Created: 2021-01-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_COMMUNICATOR_MANAGER_H
#define BLINKIT_BLINKIT_COMMUNICATOR_MANAGER_H

#pragma once

namespace base {
class SingleThreadTaskRunner;
}

namespace BlinKit {

class CrawlerCommunicator;

class CommunicatorManager
{
public:
    virtual ~CommunicatorManager(void) = default;

    virtual std::shared_ptr<CrawlerCommunicator> AcquireCommunicator(void) = 0;
protected:
    CommunicatorManager(void) = default;
};

#ifdef BLINKIT_CRAWLER_ONLY
class ExclusiveCommunicatorManager final : public CommunicatorManager
{
public:
    ExclusiveCommunicatorManager(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner);
private:
    std::shared_ptr<CrawlerCommunicator> AcquireCommunicator(void) override;

    class CommunicatorImpl;
    std::shared_ptr<CommunicatorImpl> m_communicator;
};
#endif

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_COMMUNICATOR_MANAGER_H
