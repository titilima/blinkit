// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: bitmap_animation_controller.h
// Description: BitmapAnimationController Class
//      Author: Ziming Li
//     Created: 2021-01-22
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_BITMAP_ANIMATION_CONTROLLER_H
#define BLINKIT_BLINKIT_BITMAP_ANIMATION_CONTROLLER_H

#pragma once

#include "third_party/blink/renderer/platform/graphics/paint/paint_image.h"
#include "third_party/blink/renderer/platform/timer.h"

namespace blink {
class BitmapImage;
}

namespace BlinKit {

class BitmapAnimationController
{
public:
    BitmapAnimationController(blink::BitmapImage &image);

    // Animation begins whenever someone draws the image, so startAnimation() is not normally called.
    // It will automatically pause once all observers no longer want to render the image anywhere.
    enum CatchUpAnimation { DoNotCatchUp, CatchUp };
    void Start(CatchUpAnimation = CatchUp);
    void Stop(void) { m_frameTimer.reset(); }

    blink::PaintImage PaintImageForCurrentFrame(void);
private:
    bool FrameIsCompleteAtIndex(size_t index) const;
    TimeDelta FrameDurationAtIndex(size_t index) const;

    void Advance(blink::TimerBase *timer);
    // Function that does the real work of advancing the animation.  When
    // skippingFrames is true, we're in the middle of a loop trying to skip over
    // a bunch of animation frames, so we should not do things like decode each
    // one or notify our observers.
    // Returns whether the animation was advanced.
    bool InternalAdvance(bool skippingFrames);

    blink::BitmapImage &m_image;
    std::vector<blink::PaintImage> m_otherFrames;
    std::unique_ptr<blink::TaskRunnerTimer<BitmapAnimationController>> m_frameTimer;

    size_t m_currentFrame = 0;         // The index of the current frame of animation.
    int m_repetitionsComplete = 0;     // How many repetitions we've finished.
    TimeTicks m_desiredFrameStartTime; // The system time at which we hope to see the next call to startAnimation().
    bool m_animationFinished : 1;      // Whether or not we've completed the entire animation.
    bool m_allDataReceived   : 1;      // Whether or not we've received all our data.
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_BITMAP_ANIMATION_CONTROLLER_H
