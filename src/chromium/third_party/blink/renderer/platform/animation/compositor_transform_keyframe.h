// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_transform_keyframe.h
// Description: CompositorTransformKeyframe Class
//      Author: Ziming Li
//     Created: 2020-09-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_TRANSFORM_KEYFRAME_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_TRANSFORM_KEYFRAME_H_

// BKTODO: #include "cc/animation/keyframed_animation_curve.h"
#include "third_party/blink/renderer/platform/animation/compositor_keyframe.h"
#include "third_party/blink/renderer/platform/animation/compositor_transform_operations.h"
#include "third_party/blink/renderer/platform/animation/timing_function.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace blink {

class PLATFORM_EXPORT CompositorTransformKeyframe : public CompositorKeyframe {
  WTF_MAKE_NONCOPYABLE(CompositorTransformKeyframe);

 public:
  CompositorTransformKeyframe(double time,
                              CompositorTransformOperations value,
                              const TimingFunction&);
  ~CompositorTransformKeyframe() override;

#if 0 // BKTODO:
  std::unique_ptr<cc::TransformKeyframe> CloneToCC() const;
#endif

  // CompositorKeyframe implementation.
  double Time() const override;
  const cc::TimingFunction* CcTimingFunction() const override;

 private:
#if 0 // BKTODO:
  std::unique_ptr<cc::TransformKeyframe> transform_keyframe_;
#endif
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_TRANSFORM_KEYFRAME_H_
