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

#include "blinkit/app/app_impl.h"
#include "blinkit/ui/animation/animation_scheduler.h"
#include "blinkit/ui/compositor/compositor.h"
#include "blinkit/ui/web_view_impl.h"

namespace BlinKit {

AnimationScheduler *AnimationProxy::m_scheduler = nullptr;

AnimationProxy::AnimationProxy(void)
{
    AppImpl::Get().GetCompositor().Attach(this);
}

AnimationProxy::~AnimationProxy(void)
{
#ifndef NDEBUG
    ASSERT(!AppImpl::Get().GetCompositor().IsProxyAttached(this)); // Should be detached already!
#endif
}

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
        WebViewImpl *view = GetView();

        double tick = monotonicallyIncreasingTime();
        view->BeginFrame(tick);
        view->UpdateLifecycle();

        if (m_commitRequested)
            Commit();
    }
    m_animateRequested = m_commitRequested = false;
}

void AnimationProxy::SetNeedsAnimate(void)
{
    if (!m_animateRequested)
    {
        m_scheduler->Register(this);
        m_animateRequested = true;
    }
}

} // namespace BlinKit
