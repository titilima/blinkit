// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_transform_animation_curve.cc
// Description: CompositorTransformAnimationCurve Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/animation/compositor_transform_animation_curve.h"

// BKTODO: #include "cc/animation/keyframed_animation_curve.h"
#include "cc/animation/timing_function.h"
// BKTODO: #include "cc/animation/transform_operations.h"
#include "third_party/blink/renderer/platform/animation/compositor_transform_operations.h"

namespace blink {

CompositorTransformAnimationCurve::CompositorTransformAnimationCurve()
#if 0 // BKTODO:
    : curve_(cc::KeyframedTransformAnimationCurve::Create()) {}
#else
{}
#endif

CompositorTransformAnimationCurve::~CompositorTransformAnimationCurve() =
    default;

void CompositorTransformAnimationCurve::AddKeyframe(
    const CompositorTransformKeyframe& keyframe) {
  ASSERT(false); // BKTODO: curve_->AddKeyframe(keyframe.CloneToCC());
}

void CompositorTransformAnimationCurve::SetTimingFunction(
    const TimingFunction& timing_function) {
  ASSERT(false); // BKTODO: curve_->SetTimingFunction(timing_function.CloneToCC());
}

void CompositorTransformAnimationCurve::SetScaledDuration(
    double scaled_duration) {
  ASSERT(false); // BKTODO: curve_->set_scaled_duration(scaled_duration);
}

#if 0 // BKTODO:
std::unique_ptr<cc::AnimationCurve>
CompositorTransformAnimationCurve::CloneToAnimationCurve() const {
  return curve_->Clone();
}
#endif

}  // namespace blink
