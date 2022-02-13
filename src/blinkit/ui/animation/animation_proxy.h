#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: animation_proxy.h
// Description: AnimationProxy Class
//      Author: Ziming Li
//     Created: 2022-02-06
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_ANIMATION_PROXY_H
#define BLINKIT_ANIMATION_PROXY_H

#include "blinkit/blink/renderer/platform/geometry/int_rect.h"

class WebViewImpl;

namespace BlinKit {

class AnimationFrame;
class AnimationScheduler;

class AnimationProxy
{
public:
    virtual WebViewImpl* GetView(void) const = 0;

    void SetDeferCommits(bool deferCommits) {
        m_deferCommits = deferCommits;
    }
    void SetNeedsAnimate(void);
    void SetNeedsCommit(void) {
        m_commitRequested = true;
    }

    void Update(void);
    virtual void Commit(void) = 0;

    virtual std::unique_ptr<AnimationFrame> CreateAnimationFrame(const IntSize &size) = 0;
    virtual void Flush(std::unique_ptr<AnimationFrame> &frame, const IntRect &rect) = 0;
protected:
    AnimationProxy(void) = default;

    void ScheduleAnimation(void);
private:
    friend class AnimationScheduler;

    static AnimationScheduler *m_scheduler;
    bool m_deferCommits = true;
    bool m_animateRequested = false, m_commitRequested = false;
};

} // namespace BlinKit

#endif // BLINKIT_ANIMATION_PROXY_H
