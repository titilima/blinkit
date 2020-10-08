// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_filter_animation_curve.cc
// Description: CompositorFilterAnimationCurve Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/animation/compositor_filter_animation_curve.h"

// BKTODO: #include "cc/animation/keyframed_animation_curve.h"
#include "cc/animation/timing_function.h"
// BKTODO: #include "cc/paint/filter_operations.h"
#include "third_party/blink/renderer/platform/graphics/compositor_filter_operations.h"

namespace blink {

CompositorFilterAnimationCurve::CompositorFilterAnimationCurve()
#if 0 // BKTODO:
    : curve_(cc::KeyframedFilterAnimationCurve::Create()) {}
#else
{}
#endif

CompositorFilterAnimationCurve::~CompositorFilterAnimationCurve() = default;

void CompositorFilterAnimationCurve::AddKeyframe(
    const CompositorFilterKeyframe& keyframe) {
  ASSERT(false); // BKTODO: curve_->AddKeyframe(keyframe.CloneToCC());
}

void CompositorFilterAnimationCurve::SetTimingFunction(
    const TimingFunction& timing_function) {
  ASSERT(false); // BKTODO: curve_->SetTimingFunction(timing_function.CloneToCC());
}

void CompositorFilterAnimationCurve::SetScaledDuration(double scaled_duration) {
  ASSERT(false); // BKTODO: curve_->set_scaled_duration(scaled_duration);
}

#if 0 // BKTODO:
std::unique_ptr<cc::AnimationCurve>
CompositorFilterAnimationCurve::CloneToAnimationCurve() const {
  return curve_->Clone();
}
#endif

}  // namespace blink
