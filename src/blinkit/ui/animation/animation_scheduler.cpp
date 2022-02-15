// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: animation_scheduler.cpp
// Description: AnimationScheduler Class
//      Author: Ziming Li
//     Created: 2022-02-05
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./animation_scheduler.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/ui/animation/animation_proxy.h"

namespace BlinKit {

AnimationScheduler::AnimationScheduler(void)
{
    AnimationProxy::m_scheduler = this;
}

void AnimationScheduler::Register(AnimationProxy *proxy)
{
    ASSERT(isMainThread());
    m_registeredProxies.emplace(proxy);
}

void AnimationScheduler::ScheduleAnimations(void)
{
    if (m_registeredProxies.empty())
        return;

    for (AnimationProxy *proxy : m_registeredProxies)
        proxy->PerformAnimation();
    m_registeredProxies.clear();
}

void AnimationScheduler::Unregister(AnimationProxy *proxy)
{
    ASSERT(isMainThread());
    m_registeredProxies.erase(proxy);
}

} // namespace BlinKit
