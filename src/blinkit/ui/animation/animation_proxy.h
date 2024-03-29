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
#include "blinkit/blink/renderer/platform/Timer.h"

class SkBitmap;
class SkPaint;
class WebViewImpl;

namespace BlinKit {

class AnimationFrame;
class AnimationScheduler;

class AnimationProxy
{
public:
    virtual ~AnimationProxy(void);

    virtual WebViewImpl* GetView(void) const = 0;

    void SetDeferCommits(bool deferCommits) {
        m_deferCommits = deferCommits;
    }
    void SetNeedsAnimate(void);
    void SetNeedsCommit(void) {
        m_commitRequested = true;
    }

    virtual void FlushFrame(const SkBitmap &bitmap, const IntPoint &position, const IntSize &size,
        const SkPaint &paint) = 0;
    virtual void SwapFrame(std::unique_ptr<AnimationFrame> &frame, const IntSize &size) = 0;
protected:
    AnimationProxy(void);

    void CommitAnimationImmediately(void);
private:
    friend class AnimationScheduler;

    void PerformAnimation(void);
    virtual void Commit(void) = 0;

    void OnAnimationTimerFired(Timer<AnimationProxy> *);

    static AnimationScheduler *m_scheduler;
    bool m_deferCommits = true;
    bool m_animateRequested = false, m_commitRequested = false;

    const std::shared_ptr<bool> m_hostAliveFlag;
    Timer<AnimationProxy> m_animationTimer;
};

} // namespace BlinKit

#endif // BLINKIT_ANIMATION_PROXY_H
