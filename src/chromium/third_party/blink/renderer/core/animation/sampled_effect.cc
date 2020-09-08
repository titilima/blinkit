// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: sampled_effect.cc
// Description: SampledEffect Class
//      Author: Ziming Li
//     Created: 2020-09-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/animation/sampled_effect.h"

namespace blink {

SampledEffect::SampledEffect(KeyframeEffect* effect, unsigned sequence_number)
    : effect_(effect),
      sequence_number_(sequence_number),
      priority_(effect->GetPriority()) {}

void SampledEffect::Clear() {
  effect_ = nullptr;
  interpolations_.clear();
}

// Design doc:
// https://docs.google.com/document/d/1NomOWRrGQHlynQGO64CgdqRPAAEHhi3fSa8sf0Ip6xE
bool SampledEffect::WillNeverChange() const {
  return !effect_ || !effect_->HasAnimation();
}

void SampledEffect::RemoveReplacedInterpolations(
    const HashSet<PropertyHandle>& replaced_properties) {
  auto new_end = std::remove_if(
      interpolations_.begin(), interpolations_.end(),
      [&](const auto& interpolation) {
        return replaced_properties.Contains(interpolation->GetProperty());
      });
  interpolations_.erase(new_end, interpolations_.end());
  interpolations_.shrink_to_fit();
}

void SampledEffect::UpdateReplacedProperties(
    HashSet<PropertyHandle>& replaced_properties) {
  for (const auto& interpolation : interpolations_) {
    if (!interpolation->DependsOnUnderlyingValue())
      replaced_properties.insert(interpolation->GetProperty());
  }
}

void SampledEffect::Trace(blink::Visitor* visitor) {
  visitor->Trace(effect_);
  visitor->Trace(interpolations_);
}

}  // namespace blink
