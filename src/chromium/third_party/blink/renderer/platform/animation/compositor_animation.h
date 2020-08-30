// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_animation.h
// Description: CompositorAnimation Class
//      Author: Ziming Li
//     Created: 2020-08-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_ANIMATION_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_ANIMATION_H_

#include <memory>
#include "base/memory/scoped_refptr.h"
#include "base/optional.h"
#if 0 // BKTODO:
#include "cc/animation/animation_delegate.h"
#include "cc/animation/scroll_timeline.h"
#include "cc/animation/single_keyframe_effect_animation.h"
#include "cc/animation/worklet_animation.h"
#endif
#include "third_party/blink/renderer/platform/graphics/compositor_element_id.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace cc {
class AnimationCurve;
}

namespace blink {

// BKTODO: using CompositorScrollTimeline = cc::ScrollTimeline;

class CompositorAnimationDelegate;
class CompositorKeyframeModel;

// A compositor representation for Animation.
class PLATFORM_EXPORT CompositorAnimation { // BKTODO: : public cc::AnimationDelegate {
  WTF_MAKE_NONCOPYABLE(CompositorAnimation);

 public:
  static std::unique_ptr<CompositorAnimation> Create();
#if 0 // BKTODO:
  static std::unique_ptr<CompositorAnimation> CreateWorkletAnimation(
      cc::WorkletAnimationId,
      const String& name,
      std::unique_ptr<CompositorScrollTimeline>,
      std::unique_ptr<cc::AnimationOptions>);

  explicit CompositorAnimation(
      scoped_refptr<cc::SingleKeyframeEffectAnimation>);
  ~CompositorAnimation() override;

  cc::SingleKeyframeEffectAnimation* CcAnimation() const;
#endif

  // An animation delegate is notified when animations are started and stopped.
  // The CompositorAnimation does not take ownership of the delegate, and
  // it is the responsibility of the client to reset the layer's delegate before
  // deleting the delegate.
  void SetAnimationDelegate(CompositorAnimationDelegate*);

  void AttachElement(const CompositorElementId&);
  void DetachElement();
  bool IsElementAttached() const;

  void AddKeyframeModel(std::unique_ptr<CompositorKeyframeModel>);
  void RemoveKeyframeModel(int keyframe_model_id);
  void PauseKeyframeModel(int keyframe_model_id, double time_offset);
  void AbortKeyframeModel(int keyframe_model_id);

  void UpdateScrollTimelineId(base::Optional<cc::ElementId>);

 private:
#if 0 // BKTODO:
     // cc::AnimationDelegate implementation.
  void NotifyAnimationStarted(base::TimeTicks monotonic_time,
                              int target_property,
                              int group) override;
  void NotifyAnimationFinished(base::TimeTicks monotonic_time,
                               int target_property,
                               int group) override;
  void NotifyAnimationAborted(base::TimeTicks monotonic_time,
                              int target_property,
                              int group) override;
  void NotifyAnimationTakeover(base::TimeTicks monotonic_time,
                               int target_property,
                               base::TimeTicks animation_start_time,
                               std::unique_ptr<cc::AnimationCurve>) override;

  scoped_refptr<cc::SingleKeyframeEffectAnimation> animation_;
#endif
  CompositorAnimationDelegate* delegate_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_ANIMATION_H_
