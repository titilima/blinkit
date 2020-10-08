// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_transform_operations.cc
// Description: CompositorTransformOperations Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/animation/compositor_transform_operations.h"

// BKTODO: #include "ui/gfx/transform.h"

namespace blink {

#if 0 // BKTODO:
const cc::TransformOperations&
CompositorTransformOperations::AsCcTransformOperations() const {
  return transform_operations_;
}

cc::TransformOperations
CompositorTransformOperations::ReleaseCcTransformOperations() {
  return std::move(transform_operations_);
}
#endif

bool CompositorTransformOperations::CanBlendWith(
    const blink::CompositorTransformOperations& other) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return transform_operations_.CanBlendWith(other.transform_operations_);
#endif
}

void CompositorTransformOperations::AppendTranslate(double x,
                                                    double y,
                                                    double z) {
  ASSERT(false); // BKTODO: transform_operations_.AppendTranslate(x, y, z);
}

void CompositorTransformOperations::AppendRotate(double x,
                                                 double y,
                                                 double z,
                                                 double degrees) {
  ASSERT(false); // BKTODO: transform_operations_.AppendRotate(x, y, z, degrees);
}

void CompositorTransformOperations::AppendScale(double x, double y, double z) {
  ASSERT(false); // BKTODO: transform_operations_.AppendScale(x, y, z);
}

void CompositorTransformOperations::AppendSkew(double x, double y) {
  ASSERT(false); // BKTODO: transform_operations_.AppendSkew(x, y);
}

void CompositorTransformOperations::AppendPerspective(double depth) {
  ASSERT(false); // BKTODO: transform_operations_.AppendPerspective(depth);
}

void CompositorTransformOperations::AppendMatrix(const SkMatrix44& matrix) {
  ASSERT(false); // BKTODO:
#if 0
  gfx::Transform transform(gfx::Transform::kSkipInitialization);
  transform.matrix() = matrix;
  transform_operations_.AppendMatrix(transform);
#endif
}

void CompositorTransformOperations::AppendIdentity() {
  ASSERT(false); // BKTODO: transform_operations_.AppendIdentity();
}

bool CompositorTransformOperations::IsIdentity() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return transform_operations_.IsIdentity();
#endif
}

}  // namespace blink
