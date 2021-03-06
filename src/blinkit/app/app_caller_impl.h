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

namespace base {
class SingleThreadTaskRunner;
}

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

class AppCallerImpl final : public AppCaller
{
public:
    AppCallerImpl(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner);
    ~AppCallerImpl(void) override;
private:
    void Call(const base::Location &loc, std::function<void()> &&task) override;
    void SyncCall(const base::Location &loc, std::function<void()> &&task) override;

    std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_CALLER_IMPL_H
