// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: worklet_animation_base.h
// Description: WorkletAnimationBase Class
//      Author: Ziming Li
//     Created: 2020-08-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_ANIMATION_WORKLET_ANIMATION_BASE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_ANIMATION_WORKLET_ANIMATION_BASE_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
// BKTODO: #include "third_party/blink/renderer/platform/graphics/animation_worklet_mutators_state.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"

namespace blink {

class AnimationTimeline;
class Document;
class KeyframeEffect;

class CORE_EXPORT WorkletAnimationBase : public ScriptWrappable {
 public:
  ~WorkletAnimationBase() override = default;

  // Asks the animation to update its effect inherited time.
  virtual void Update(TimingUpdateReason) = 0;

  // Updates the animation on the compositor side to reflect the main thread
  // state.
  virtual void UpdateCompositingState() = 0;
  virtual void InvalidateCompositingState() = 0;

  virtual Document* GetDocument() const = 0;
  virtual KeyframeEffect* GetEffect() const = 0;
#if 0 // BKTODO:
  virtual const WorkletAnimationId& GetWorkletAnimationId() const = 0;
#endif
  virtual bool IsActiveAnimation() const = 0;
#if 0 // BKTODO:
  virtual void UpdateInputState(AnimationWorkletDispatcherInput*) = 0;
  virtual void SetOutputState(
      const AnimationWorkletOutput::AnimationState&) = 0;
#endif
  virtual AnimationTimeline* GetTimeline() const = 0;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_ANIMATION_WORKLET_ANIMATION_BASE_H_
