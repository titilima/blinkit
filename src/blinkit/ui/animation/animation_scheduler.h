#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: animation_scheduler.h
// Description: AnimationScheduler Class
//      Author: Ziming Li
//     Created: 2022-02-05
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_ANIMATION_SCHEDULER_H
#define BLINKIT_ANIMATION_SCHEDULER_H

#include <unordered_set>

namespace BlinKit {

class AnimationProxy;

class AnimationScheduler
{
public:
    AnimationScheduler(void);

    void Register(AnimationProxy *proxy);
    void Unregister(AnimationProxy *proxy);

    void ScheduleAnimations(void);
private:
    std::unordered_set<AnimationProxy *> m_registeredProxies;
};

} // namespace BlinKit

#endif // BLINKIT_ANIMATION_SCHEDULER_H
