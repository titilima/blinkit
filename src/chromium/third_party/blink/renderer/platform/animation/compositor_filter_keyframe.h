// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_filter_keyframe.h
// Description: CompositorFilterKeyframe Class
//      Author: Ziming Li
//     Created: 2020-09-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_FILTER_KEYFRAME_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_FILTER_KEYFRAME_H_

// BKTODO: #include "cc/animation/keyframed_animation_curve.h"
#include "third_party/blink/renderer/platform/animation/compositor_keyframe.h"
#include "third_party/blink/renderer/platform/graphics/compositor_filter_operations.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace blink {

class TimingFunction;

class PLATFORM_EXPORT CompositorFilterKeyframe { // BKTODO: : public CompositorKeyframe {
  WTF_MAKE_NONCOPYABLE(CompositorFilterKeyframe);

 public:
  CompositorFilterKeyframe(double time,
                           CompositorFilterOperations value,
                           const TimingFunction&);
  ~CompositorFilterKeyframe(); // BKTODO: override;

#if 0 // BKTODO:
  std::unique_ptr<cc::FilterKeyframe> CloneToCC() const;

  // CompositorKeyframe implementation.
  double Time() const override;
  const cc::TimingFunction* CcTimingFunction() const override;

 private:
  std::unique_ptr<cc::FilterKeyframe> filter_keyframe_;
#endif
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_FILTER_KEYFRAME_H_
