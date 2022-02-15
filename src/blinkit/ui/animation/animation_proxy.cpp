// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: animation_proxy.cpp
// Description: AnimationProxy Class
//      Author: Ziming Li
//     Created: 2022-02-06
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./animation_proxy.h"

#include "blinkit/ui/animation/animation_scheduler.h"
#include "blinkit/ui/web_view_impl.h"

namespace BlinKit {

AnimationScheduler *AnimationProxy::m_scheduler = nullptr;

void AnimationProxy::CommitAnimationImmediately(void)
{
    SetNeedsAnimate();
    PerformAnimation();
    m_scheduler->Unregister(this);
}

void AnimationProxy::PerformAnimation(void)
{
    ASSERT(m_animateRequested);
    if (!m_deferCommits)
    {
        GetView()->UpdateLifecycle();

        if (m_commitRequested)
            Commit();
    }
    m_animateRequested = m_commitRequested = false;
}

void AnimationProxy::SetNeedsAnimate(void)
{
    if (m_animateRequested)
        return;

    m_scheduler->Register(this);
    m_animateRequested = true;

    double tick = monotonicallyIncreasingTime();
    GetView()->BeginFrame(tick);
}

} // namespace BlinKit
