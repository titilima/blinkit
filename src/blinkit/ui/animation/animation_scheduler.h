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
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

class AnimationProxy;

class AnimationScheduler
{
public:
    AnimationScheduler(void);

    bool IsProxyRegistered(AnimationProxy *proxy) const {
        return zed::key_exists(m_registeredProxies, proxy);
    }
    void Register(AnimationProxy *proxy) {
        m_registeredProxies.emplace(proxy);
    }

    void ScheduleAnimations(void);
private:
    std::unordered_set<AnimationProxy *> m_registeredProxies;
};

} // namespace BlinKit

#endif // BLINKIT_ANIMATION_SCHEDULER_H
