#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: sampled_effect.h
// Description: SampledEffect Class
//      Author: Ziming Li
//     Created: 2022-02-28
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SampledEffect_h
#define SampledEffect_h

#include "blinkit/blink/renderer/core/animation/Animation.h"
#include "blinkit/blink/renderer/core/animation/Interpolation.h"
#include "blinkit/blink/renderer/core/animation/KeyframeEffect.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"
#include "blinkit/blink/renderer/wtf/BitArray.h"

namespace blink {

class SVGElement;

// TODO(haraken): Drop Finalized once we ship Oilpan and the OwnPtrWillBeMember
// is gone.
class SampledEffect : public BlinKit::GCObject
{
    WTF_MAKE_NONCOPYABLE(SampledEffect);
public:
    static GCRefPtr<SampledEffect> create(KeyframeEffect* animation)
    {
        return GCWrapShared(new SampledEffect(animation));
    }

    void clear();

    const std::vector<RefPtr<Interpolation>>& interpolations() const { return m_interpolations; }
    std::vector<RefPtr<Interpolation>>& mutableInterpolations() { return m_interpolations; }

    KeyframeEffect* effect() const { return m_effect; }
    unsigned sequenceNumber() const { return m_sequenceNumber; }
    KeyframeEffect::Priority priority() const { return m_priority; }

    DECLARE_TRACE();

private:
    SampledEffect(KeyframeEffect*);

    WeakMember<KeyframeEffect> m_effect;
    Member<Animation> m_animation;
    std::vector<RefPtr<Interpolation>> m_interpolations;
    const unsigned m_sequenceNumber;
    KeyframeEffect::Priority m_priority;
};

} // namespace blink

#endif // SampledEffect_h
