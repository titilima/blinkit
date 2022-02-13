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

#include "blinkit/ui/animation/animation_proxy.h"

namespace BlinKit {

AnimationScheduler::AnimationScheduler(void)
{
    AnimationProxy::m_scheduler = this;
}

void AnimationScheduler::ScheduleAnimations(void)
{
    if (m_registeredProxies.empty())
        return;

    for (AnimationProxy *proxy : m_registeredProxies)
    {
        if (proxy->m_deferCommits)
            continue;

        proxy->Update();
        if (proxy->m_commitRequested)
            proxy->Commit();

        proxy->m_animateRequested = false;
        proxy->m_commitRequested = false;
    }
    m_registeredProxies.clear();
}

} // namespace BlinKit
