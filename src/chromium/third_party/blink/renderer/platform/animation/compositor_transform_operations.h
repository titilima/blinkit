// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_transform_operations.h
// Description: CompositorTransformOperations Class
//      Author: Ziming Li
//     Created: 2020-09-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_TRANSFORM_OPERATIONS_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_TRANSFORM_OPERATIONS_H_

// BKTODO: #include "cc/animation/transform_operations.h"
#include "third_party/blink/renderer/platform/platform_export.h"

class SkMatrix44;

namespace blink {

class PLATFORM_EXPORT CompositorTransformOperations {
 public:
#if 0 // BKTODO:
  const cc::TransformOperations& AsCcTransformOperations() const;
  cc::TransformOperations ReleaseCcTransformOperations();
#endif

  // Returns true if these operations can be blended. It will only return
  // false if we must resort to matrix interpolation, and matrix interpolation
  // fails (this can happen if either matrix cannot be decomposed).
  bool CanBlendWith(const CompositorTransformOperations& other) const;

  void AppendTranslate(double x, double y, double z);
  void AppendRotate(double x, double y, double z, double degrees);
  void AppendScale(double x, double y, double z);
  void AppendSkew(double x, double y);
  void AppendPerspective(double depth);
  void AppendMatrix(const SkMatrix44&);
  void AppendIdentity();

  bool IsIdentity() const;

 private:
#if 0 // BKTODO:
  cc::TransformOperations transform_operations_;
#endif
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_ANIMATION_COMPOSITOR_TRANSFORM_OPERATIONS_H_
