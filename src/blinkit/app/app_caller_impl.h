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

namespace blink {
class WebTaskRunner;
}

namespace BlinKit {

#ifdef BLINKIT_CRAWLER_ONLY
class SyncAppCallerImpl final : public AppCaller
{
public:
    void Call(const blink::WebTraceLocation &, std::function<void()> &&task) override
    {
        task();
    }
    void SyncCall(const blink::WebTraceLocation &, const std::function<void()> &callback) override
    {
        callback();
    }
};
#endif

class AppCallerImpl final : public AppCaller
{
public:
    AppCallerImpl(const std::shared_ptr<blink::WebTaskRunner> &taskRunner);
    ~AppCallerImpl(void) override;
private:
    void Call(const blink::WebTraceLocation &loc, std::function<void()> &&task) override;
    void SyncCall(const blink::WebTraceLocation &loc, const std::function<void()> &callback) override;

    std::shared_ptr<blink::WebTaskRunner> m_taskRunner;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APP_CALLER_IMPL_H
