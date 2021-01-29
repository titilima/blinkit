// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: app_caller_impl.h
// Description: AppCall Implementations
//      Author: Ziming Li
//     Created: 2021-01-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_APP_CALLER_IMPL_H
#define BLINKIT_BLINKIT_APP_CALLER_IMPL_H

#pragma once

#include "blinkit/app/caller.h"

namespace BlinKit {

#ifdef BLINKIT_CRAWLER_ONLY
class SyncAppCallerImpl final : public AppCaller
{
public:
    void Call(const base::Location &, std::function<void()> &&task) override
    {
        task();
    }
    void SyncCall(const base::Location &, std::function<void()> &&task) override
    {
        task();
    }
};
#endif

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_CALLER_IMPL_H
