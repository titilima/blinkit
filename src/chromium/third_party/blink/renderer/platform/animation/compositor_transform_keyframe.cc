// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_transform_keyframe.cc
// Description: CompositorTransformKeyframe Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/animation/compositor_transform_keyframe.h"

#include <memory>

namespace blink {

CompositorTransformKeyframe::CompositorTransformKeyframe(
    double time,
    CompositorTransformOperations value,
    const TimingFunction& timing_function)
#if 0 // BKTODO:
    : transform_keyframe_(
          cc::TransformKeyframe::Create(base::TimeDelta::FromSecondsD(time),
                                        value.ReleaseCcTransformOperations(),
                                        timing_function.CloneToCC())) {}
#else
{}
#endif

CompositorTransformKeyframe::~CompositorTransformKeyframe() = default;

double CompositorTransformKeyframe::Time() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return transform_keyframe_->Time().InSecondsF();
#endif
}

const cc::TimingFunction* CompositorTransformKeyframe::CcTimingFunction()
    const {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return transform_keyframe_->timing_function();
#endif
}

#if 0 // BKTODO:
std::unique_ptr<cc::TransformKeyframe> CompositorTransformKeyframe::CloneToCC()
    const {
  return transform_keyframe_->Clone();
}
#endif

}  // namespace blink
