// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: bitmap_animation_controller.cpp
// Description: BitmapAnimationController Class
//      Author: Ziming Li
//     Created: 2021-01-22
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "bitmap_animation_controller.h"

#include "blinkit/ui/rendering_scheduler.h"
#include "third_party/blink/renderer/platform/graphics/bitmap_image.h"

using namespace blink;

namespace BlinKit {

BitmapAnimationController::BitmapAnimationController(BitmapImage &image)
    : m_image(image)
    , m_animationFinished(false)
    , m_allDataReceived(true)
{
}

void BitmapAnimationController::Advance(TimerBase *timer)
{
    ScopedRenderingScheduler scheduler;
    ASSERT(timer == m_frameTimer.get());
    InternalAdvance(false);
    // At this point the image region has been marked dirty, and if it's
    // onscreen, we'll soon make a call to draw(), which will call
    // startAnimation() again to keep the animation moving.
}

TimeDelta BitmapAnimationController::FrameDurationAtIndex(size_t index) const
{
    return m_image.decoder_->FrameDurationAtIndex(index);
}

bool BitmapAnimationController::FrameIsCompleteAtIndex(size_t index) const
{
    return m_image.decoder_->FrameIsReceivedAtIndex(index);
}

bool BitmapAnimationController::InternalAdvance(bool skippingFrames)
{
    Stop();

    // See if anyone is still paying attention to this animation.  If not, we don't
    // advance and will remain suspended at the current frame until the animation is resumed.
    if (!skippingFrames && m_image.GetImageObserver()->ShouldPauseAnimation(&m_image))
        return false;

    ++m_currentFrame;
    bool advancedAnimation = true;
    if (m_currentFrame >= m_image.FrameCount())
    {
        ++m_repetitionsComplete;

        // Get the repetition count again.  If we weren't able to get a
        // repetition count before, we should have decoded the whole image by
        // now, so it should now be available.
        // Note that we don't need to special-case cAnimationLoopOnce here
        // because it is 0 (see comments on its declaration in ImageAnimation.h).
        if (m_image.RepetitionCount() != kAnimationLoopInfinite && m_repetitionsComplete > m_image.RepetitionCount())
        {
            m_animationFinished = true;
            m_desiredFrameStartTime = TimeTicks();
            --m_currentFrame;
            advancedAnimation = false;
        }
        else
        {
            m_currentFrame = 0;
        }
    }

    // We need to draw this frame if we advanced to it while not skipping, or if
    // while trying to skip frames we hit the last frame and thus had to stop.
    if (skippingFrames != advancedAnimation)
        m_image.GetImageObserver()->AnimationAdvanced(&m_image);
    return advancedAnimation;
}

void BitmapAnimationController::Start(CatchUpAnimation catchUpIfNecessary)
{
    if (m_frameTimer || m_animationFinished)
        return;

    // If we aren't already animating, set now as the animation start time.
    const TimeTicks time = CurrentTimeTicks();
    if (m_desiredFrameStartTime.is_null())
        m_desiredFrameStartTime = time;

    // Don't advance the animation to an incomplete frame.
    size_t nextFrame = (m_currentFrame + 1) % m_image.FrameCount();
    if (!m_allDataReceived && !FrameIsCompleteAtIndex(nextFrame))
        return;

    // Don't advance past the last frame if we haven't decoded the whole image
    // yet and our repetition count is potentially unset.  The repetition count
    // in a GIF can potentially come after all the rest of the image data, so
    // wait on it.
    if (!m_allDataReceived && m_image.RepetitionCount() == kAnimationLoopOnce
        && m_currentFrame >= (m_image.FrameCount() - 1))
    {
        return;
    }

    // Determine time for next frame to start.  By ignoring paint and timer lag
    // in this calculation, we make the animation appear to run at its desired
    // rate regardless of how fast it's being repainted.
    const TimeDelta currentDuration = FrameDurationAtIndex(m_currentFrame);
    m_desiredFrameStartTime += currentDuration;

    // When an animated image is more than five minutes out of date, the
    // user probably doesn't care about resyncing and we could burn a lot of
    // time looping through frames below.  Just reset the timings.
    const TimeDelta kAnimationResyncCutoff = TimeDelta::FromSeconds(5 * 60);
    if ((time - m_desiredFrameStartTime) > kAnimationResyncCutoff)
        m_desiredFrameStartTime = time + currentDuration;

    // The image may load more slowly than it's supposed to animate, so that by
    // the time we reach the end of the first repetition, we're well behind.
    // Clamp the desired frame start time in this case, so that we don't skip
    // frames (or whole iterations) trying to "catch up".  This is a tradeoff:
    // It guarantees users see the whole animation the second time through and
    // don't miss any repetitions, and is closer to what other browsers do; on
    // the other hand, it makes animations "less accurate" for pages that try to
    // sync an image and some other resource (e.g. audio), especially if users
    // switch tabs (and thus stop drawing the animation, which will pause it)
    // during that initial loop, then switch back later.
    if (nextFrame == 0 && m_repetitionsComplete == 0 && m_desiredFrameStartTime < time)
        m_desiredFrameStartTime = time;

    if (catchUpIfNecessary == DoNotCatchUp || time < m_desiredFrameStartTime)
    {
        auto taskRunner = Platform::Current()->CurrentThread()->GetTaskRunner();
        // Haven't yet reached time for next frame to start; delay until then.
        m_frameTimer = std::make_unique<TaskRunnerTimer<BitmapAnimationController>>(taskRunner, this, &BitmapAnimationController::Advance);
        m_frameTimer->StartOneShot(std::max(m_desiredFrameStartTime - time, TimeDelta()), FROM_HERE);
    }
    else
    {
        // We've already reached or passed the time for the next frame to start.
        // See if we've also passed the time for frames after that to start, in
        // case we need to skip some frames entirely.  Remember not to advance
        // to an incomplete frame.
        for (size_t frameAfterNext = (nextFrame + 1) % m_image.FrameCount();
            FrameIsCompleteAtIndex(frameAfterNext);
            frameAfterNext = (nextFrame + 1) % m_image.FrameCount())
        {
            // Should we skip the next frame?
            TimeTicks frameAfterNextStartTime = m_desiredFrameStartTime + FrameDurationAtIndex(nextFrame);
            if (time < frameAfterNextStartTime)
                break;

            // Yes; skip over it without notifying our observers.
            if (!InternalAdvance(true))
                return;

            m_desiredFrameStartTime = frameAfterNextStartTime;
            nextFrame = frameAfterNext;
        }

        // Draw the next frame immediately.  Note that m_desiredFrameStartTime
        // may be in the past, meaning the next time through this function we'll
        // kick off the next advancement sooner than this frame's duration would
        // suggest.
        if (InternalAdvance(false))
        {
            // The image region has been marked dirty, but once we return to our
            // caller, draw() will clear it, and nothing will cause the
            // animation to advance again.  We need to start the timer for the
            // next frame running, or the animation can hang.  (Compare this
            // with when advanceAnimation() is called, and the region is dirtied
            // while draw() is not in the callstack, meaning draw() gets called
            // to update the region and thus startAnimation() is reached again.)
            // NOTE: For large images with slow or heavily-loaded systems,
            // throwing away data as we go (see destroyDecodedData()) means we
            // can spend so much time re-decoding data above that by the time we
            // reach here we're behind again.  If we let startAnimation() run
            // the catch-up code again, we can get long delays without painting
            // as we race the timer, or even infinite recursion.  In this
            // situation the best we can do is to simply change frames as fast
            // as possible, so force startAnimation() to set a zero-delay timer
            // and bail out if we're not caught up.
            Start(DoNotCatchUp);
        }
    }
}

} // namespace BlinKit
