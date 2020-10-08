// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_float_keyframe.cc
// Description: CompositorFloatKeyframe Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/animation/compositor_float_keyframe.h"

#include "third_party/blink/renderer/platform/animation/timing_function.h"

namespace blink {

CompositorFloatKeyframe::CompositorFloatKeyframe(
    double time,
    float value,
    const TimingFunction& timing_function)
#if 0 // BKTODO:
    : float_keyframe_(
          cc::FloatKeyframe::Create(base::TimeDelta::FromSecondsD(time),
                                    value,
                                    timing_function.CloneToCC())) {}
#else
{}
#endif

#if 0 // BKTODO:
CompositorFloatKeyframe::CompositorFloatKeyframe(
    std::unique_ptr<cc::FloatKeyframe> float_keyframe)
    : float_keyframe_(std::move(float_keyframe)) {}
#endif

CompositorFloatKeyframe::~CompositorFloatKeyframe() = default;

double CompositorFloatKeyframe::Time() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return float_keyframe_->Time().InSecondsF();
#endif
}

const cc::TimingFunction* CompositorFloatKeyframe::CcTimingFunction() const {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return float_keyframe_->timing_function();
#endif
}

#if 0 // BKTODO:
std::unique_ptr<cc::FloatKeyframe> CompositorFloatKeyframe::CloneToCC() const {
  return float_keyframe_->Clone();
}
#endif

}  // namespace blink
