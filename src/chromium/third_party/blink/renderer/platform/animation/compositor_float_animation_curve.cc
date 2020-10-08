// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_float_animation_curve.cc
// Description: CompositorFloatAnimationCurve Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/animation/compositor_float_animation_curve.h"

#include <memory>
#include <utility>

#include "base/memory/ptr_util.h"
#if 0 // BKTODO:
#include "cc/animation/animation_curve.h"
#include "cc/animation/keyframed_animation_curve.h"
#endif
#include "cc/animation/timing_function.h"

namespace blink {

CompositorFloatAnimationCurve::CompositorFloatAnimationCurve()
#if 0 // BKTODO:
    : curve_(cc::KeyframedFloatAnimationCurve::Create()) {}
#else
{}
#endif

#if 0 // BKTODO:
CompositorFloatAnimationCurve::CompositorFloatAnimationCurve(
    std::unique_ptr<cc::KeyframedFloatAnimationCurve> curve)
    : curve_(std::move(curve)) {}
#endif

CompositorFloatAnimationCurve::~CompositorFloatAnimationCurve() = default;

#if 0 // BKTODO:
std::unique_ptr<CompositorFloatAnimationCurve>
CompositorFloatAnimationCurve::CreateForTesting(
    std::unique_ptr<cc::KeyframedFloatAnimationCurve> curve) {
  return base::WrapUnique(new CompositorFloatAnimationCurve(std::move(curve)));
}

CompositorFloatAnimationCurve::Keyframes
CompositorFloatAnimationCurve::KeyframesForTesting() const {
  Keyframes keyframes;
  for (const auto& cc_keyframe : curve_->keyframes_for_testing()) {
    keyframes.push_back(
        base::WrapUnique(new CompositorFloatKeyframe(cc_keyframe->Clone())));
  }
  return keyframes;
}

scoped_refptr<TimingFunction>
CompositorFloatAnimationCurve::GetTimingFunctionForTesting() const {
  return CreateCompositorTimingFunctionFromCC(
      curve_->timing_function_for_testing());
}
#endif

void CompositorFloatAnimationCurve::AddKeyframe(
    const CompositorFloatKeyframe& keyframe) {
  ASSERT(false); // BKTODO: curve_->AddKeyframe(keyframe.CloneToCC());
}

void CompositorFloatAnimationCurve::SetTimingFunction(
    const TimingFunction& timing_function) {
  ASSERT(false); // BKTODO: curve_->SetTimingFunction(timing_function.CloneToCC());
}

void CompositorFloatAnimationCurve::SetScaledDuration(double scaled_duration) {
  ASSERT(false); // BKTODO: curve_->set_scaled_duration(scaled_duration);
}

float CompositorFloatAnimationCurve::GetValue(double time) const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return curve_->GetValue(base::TimeDelta::FromSecondsD(time));
#endif
}

#if 0 // BKTODO:
std::unique_ptr<cc::AnimationCurve>
CompositorFloatAnimationCurve::CloneToAnimationCurve() const {
  return curve_->Clone();
}
#endif

}  // namespace blink
