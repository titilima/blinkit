// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: AnimationEffectTiming.cpp
// Description: AnimationEffectTiming Class
//      Author: Ziming Li
//     Created: 2019-02-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/AnimationEffectTiming.h"

#include "core/animation/AnimationEffect.h"
#include "core/animation/KeyframeEffect.h"
#include "platform/animation/TimingFunction.h"

namespace blink {

AnimationEffectTiming* AnimationEffectTiming::create(AnimationEffect* parent)
{
    return new AnimationEffectTiming(parent);
}

AnimationEffectTiming::AnimationEffectTiming(AnimationEffect* parent)
    : m_parent(parent)
{
}

double AnimationEffectTiming::delay()
{
    return m_parent->specifiedTiming().startDelay * 1000;
}

double AnimationEffectTiming::endDelay()
{
    return m_parent->specifiedTiming().endDelay * 1000;
}

String AnimationEffectTiming::fill()
{
    return Timing::fillModeString(m_parent->specifiedTiming().fillMode);
}

double AnimationEffectTiming::iterationStart()
{
    return m_parent->specifiedTiming().iterationStart;
}

double AnimationEffectTiming::iterations()
{
    return m_parent->specifiedTiming().iterationCount;
}

void AnimationEffectTiming::duration(UnrestrictedDoubleOrString& returnValue)
{
    assert(false); // Not reached!
}

double AnimationEffectTiming::playbackRate()
{
    return m_parent->specifiedTiming().playbackRate;
}

String AnimationEffectTiming::direction()
{
    return Timing::playbackDirectionString(m_parent->specifiedTiming().direction);
}

String AnimationEffectTiming::easing()
{
    return m_parent->specifiedTiming().timingFunction->toString();
}

void AnimationEffectTiming::setDelay(double delay)
{
    Timing timing = m_parent->specifiedTiming();
    TimingInput::setStartDelay(timing, delay);
    m_parent->updateSpecifiedTiming(timing);
}

void AnimationEffectTiming::setEndDelay(double endDelay)
{
    Timing timing = m_parent->specifiedTiming();
    TimingInput::setEndDelay(timing, endDelay);
    m_parent->updateSpecifiedTiming(timing);
}

void AnimationEffectTiming::setFill(String fill)
{
    Timing timing = m_parent->specifiedTiming();
    TimingInput::setFillMode(timing, fill);
    m_parent->updateSpecifiedTiming(timing);
}

void AnimationEffectTiming::setIterationStart(double iterationStart)
{
    Timing timing = m_parent->specifiedTiming();
    TimingInput::setIterationStart(timing, iterationStart);
    m_parent->updateSpecifiedTiming(timing);
}

void AnimationEffectTiming::setIterations(double iterations)
{
    Timing timing = m_parent->specifiedTiming();
    TimingInput::setIterationCount(timing, iterations);
    m_parent->updateSpecifiedTiming(timing);
}

void AnimationEffectTiming::setDuration(const UnrestrictedDoubleOrString& durationOrAuto)
{
    assert(false); // Not reached!
}

void AnimationEffectTiming::setPlaybackRate(double playbackRate)
{
    Timing timing = m_parent->specifiedTiming();
    TimingInput::setPlaybackRate(timing, playbackRate);
    m_parent->updateSpecifiedTiming(timing);
}

void AnimationEffectTiming::setDirection(String direction)
{
    Timing timing = m_parent->specifiedTiming();
    TimingInput::setPlaybackDirection(timing, direction);
    m_parent->updateSpecifiedTiming(timing);
}

void AnimationEffectTiming::setEasing(String easing)
{
    Timing timing = m_parent->specifiedTiming();
    TimingInput::setTimingFunction(timing, easing);
    m_parent->updateSpecifiedTiming(timing);
}

DEFINE_TRACE(AnimationEffectTiming)
{
    visitor->trace(m_parent);
}

} // namespace blink
