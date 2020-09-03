// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_float_keyframe.h
// Description: CompositorFloatKeyframe Class
//      Author: Ziming Li
//     Created: 2020-09-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_FLOAT_KEYFRAME_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_FLOAT_KEYFRAME_H_

// BKTODO: #include "cc/animation/keyframed_animation_curve.h"
#include "third_party/blink/renderer/platform/animation/compositor_keyframe.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace blink {

class TimingFunction;

class PLATFORM_EXPORT CompositorFloatKeyframe : public CompositorKeyframe {
  WTF_MAKE_NONCOPYABLE(CompositorFloatKeyframe);

 public:
  CompositorFloatKeyframe(double time, float value, const TimingFunction&);
#if 0 // BKTODO:
  CompositorFloatKeyframe(std::unique_ptr<cc::FloatKeyframe>);
#endif
  ~CompositorFloatKeyframe() override;

  // CompositorKeyframe implementation.
  double Time() const override;
  const cc::TimingFunction* CcTimingFunction() const override;

#if 0 // BKTODO:
  float Value() { return float_keyframe_->Value(); }
  std::unique_ptr<cc::FloatKeyframe> CloneToCC() const;

 private:
  std::unique_ptr<cc::FloatKeyframe> float_keyframe_;
#endif
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_FLOAT_KEYFRAME_H_
