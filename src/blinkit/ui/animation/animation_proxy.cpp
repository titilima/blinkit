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

void AnimationProxy::ScheduleAnimation(void)
{
    m_scheduler->Register(this);
}

void AnimationProxy::SetNeedsAnimate(void)
{
    if (!m_scheduler->IsProxyRegistered(this))
        return;

    if (m_animateRequested)
        return;
    m_animateRequested = true;

    double tick = monotonicallyIncreasingTime();
    GetView()->BeginFrame(tick);
}

void AnimationProxy::Update(void)
{
    WebViewImpl *view = GetView();
    if (!m_animateRequested)
    {
        double tick = monotonicallyIncreasingTime();
        view->BeginFrame(tick);
    }
    view->UpdateLifecycle();
}

} // namespace BlinKit
