#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: AnimationEffectTiming.h
// Description: AnimationEffectTiming Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef AnimationEffectTiming_h
#define AnimationEffectTiming_h

#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"
#include "blinkit/blink/renderer/core/animation/AnimationEffect.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class UnrestrictedDoubleOrString;

class AnimationEffectTiming : public GarbageCollected<AnimationEffectTiming>, public ScriptWrappable
{
    DEFINE_WRAPPERTYPEINFO();
public:
    static AnimationEffectTiming* create(AnimationEffect* parent);
    double delay();
    double endDelay();
    String fill();
    double iterationStart();
    double iterations();
    void duration(UnrestrictedDoubleOrString&);
    double playbackRate();
    String direction();
    String easing();

    void setDelay(double);
    void setEndDelay(double);
    void setFill(String);
    void setIterationStart(double);
    void setIterations(double);
    void setDuration(const UnrestrictedDoubleOrString&);
    void setPlaybackRate(double);
    void setDirection(String);
    void setEasing(String);

    DECLARE_TRACE();

private:
    Member<AnimationEffect> m_parent;
    explicit AnimationEffectTiming(AnimationEffect*);
};

} // namespace blink

#endif
