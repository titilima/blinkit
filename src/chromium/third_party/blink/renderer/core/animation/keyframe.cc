// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: keyframe.cc
// Description: Keyframe Class
//      Author: Ziming Li
//     Created: 2020-09-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/animation/keyframe.h"

#include "third_party/blink/renderer/core/animation/effect_model.h"
#include "third_party/blink/renderer/core/animation/invalidatable_interpolation.h"

namespace blink {

Keyframe::PropertySpecificKeyframe::PropertySpecificKeyframe(
    double offset,
    scoped_refptr<TimingFunction> easing,
    EffectModel::CompositeOperation composite)
    : offset_(offset), easing_(std::move(easing)), composite_(composite) {
  DCHECK(!IsNull(offset));
  if (!easing_)
    easing_ = LinearTimingFunction::Shared();
}

Interpolation* Keyframe::PropertySpecificKeyframe::CreateInterpolation(
    const PropertyHandle& property_handle,
    const Keyframe::PropertySpecificKeyframe& end) const {
  // const_cast to take refs.
  return InvalidatableInterpolation::Create(
      property_handle, const_cast<PropertySpecificKeyframe*>(this),
      const_cast<PropertySpecificKeyframe*>(&end));
}

void Keyframe::AddKeyframePropertiesToV8Object(
    V8ObjectBuilder& object_builder) const {
  ASSERT(false); // BKTODO:
#if 0
  if (offset_) {
    object_builder.Add("offset", offset_.value());
  } else {
    object_builder.AddNull("offset");
  }
  object_builder.Add("easing", easing_->ToString());
  object_builder.AddString("composite",
                           EffectModel::CompositeOperationToString(composite_));
#endif
}

}  // namespace blink
