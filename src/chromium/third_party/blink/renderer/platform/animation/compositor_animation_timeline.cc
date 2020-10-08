// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_animation_timeline.cc
// Description: CompositorAnimationTimeline Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/animation/compositor_animation_timeline.h"

#if 0 // BKTODO:
#include "cc/animation/animation_host.h"
#include "cc/animation/animation_id_provider.h"
#endif
#include "third_party/blink/renderer/platform/animation/compositor_animation.h"
#include "third_party/blink/renderer/platform/animation/compositor_animation_client.h"
// BKTODO: #include "third_party/blink/renderer/platform/animation/compositor_animation_host.h"

namespace blink {

CompositorAnimationTimeline::CompositorAnimationTimeline()
#if 0 // BKTODO:
    : animation_timeline_(cc::AnimationTimeline::Create(
          cc::AnimationIdProvider::NextTimelineId())) {}
#else
{}
#endif

CompositorAnimationTimeline::~CompositorAnimationTimeline() {
  ASSERT(false); // BKTODO:
#if 0
  // Detach timeline from host, otherwise it stays there (leaks) until
  // compositor shutdown.
  if (animation_timeline_->animation_host())
    animation_timeline_->animation_host()->RemoveAnimationTimeline(
        animation_timeline_);
#endif
}

#if 0 // BKTODO:
cc::AnimationTimeline* CompositorAnimationTimeline::GetAnimationTimeline()
    const {
  return animation_timeline_.get();
}

void CompositorAnimationTimeline::AnimationAttached(
    const blink::CompositorAnimationClient& client) {
  if (client.GetCompositorAnimation()) {
    animation_timeline_->AttachAnimation(
        client.GetCompositorAnimation()->CcAnimation());
  }
}

void CompositorAnimationTimeline::AnimationDestroyed(
    const blink::CompositorAnimationClient& client) {
  if (client.GetCompositorAnimation()) {
    animation_timeline_->DetachAnimation(
        client.GetCompositorAnimation()->CcAnimation());
  }
}
#endif

}  // namespace blink
