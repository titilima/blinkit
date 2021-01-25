// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_communicator.h
// Description: CrawlerCommunicator Class
//      Author: Ziming Li
//     Created: 2021-01-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CRAWLER_COMMUNICATOR_H
#define BLINKIT_BLINKIT_CRAWLER_COMMUNICATOR_H

#pragma once

#include <functional>
#include "base/location.h"

namespace BlinKit {

class CrawlerCommunicator
{
public:
    virtual void CallCrawler(const base::Location &loc, std::function<void()> &&worker) = 0;
    virtual void PostToClient(const base::Location &loc, std::function<void()> &&worker) = 0;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_COMMUNICATOR_H
