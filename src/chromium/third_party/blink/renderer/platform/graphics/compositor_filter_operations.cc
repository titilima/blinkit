// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_filter_operations.cc
// Description: CompositorFilterOperations Class
//      Author: Ziming Li
//     Created: 2020-10-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/graphics/compositor_filter_operations.h"

#include "third_party/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/skia/include/core/SkImageFilter.h"
#include "ui/gfx/geometry/rect.h"

namespace blink {

#if 0 // BKTODO:
const cc::FilterOperations& CompositorFilterOperations::AsCcFilterOperations()
    const {
  return filter_operations_;
}

cc::FilterOperations CompositorFilterOperations::ReleaseCcFilterOperations() {
  return std::move(filter_operations_);
}
#endif

void CompositorFilterOperations::AppendGrayscaleFilter(float amount) {
  ASSERT(false); // BKTODO: filter_operations_.Append(cc::FilterOperation::CreateGrayscaleFilter(amount));
}

void CompositorFilterOperations::AppendSepiaFilter(float amount) {
  ASSERT(false); // BKTODO: filter_operations_.Append(cc::FilterOperation::CreateSepiaFilter(amount));
}

void CompositorFilterOperations::AppendSaturateFilter(float amount) {
  ASSERT(false); // BKTODO: filter_operations_.Append(cc::FilterOperation::CreateSaturateFilter(amount));
}

void CompositorFilterOperations::AppendHueRotateFilter(float amount) {
  ASSERT(false); // BKTODO: filter_operations_.Append(cc::FilterOperation::CreateHueRotateFilter(amount));
}

void CompositorFilterOperations::AppendInvertFilter(float amount) {
  ASSERT(false); // BKTODO: filter_operations_.Append(cc::FilterOperation::CreateInvertFilter(amount));
}

void CompositorFilterOperations::AppendBrightnessFilter(float amount) {
  ASSERT(false); // BKTODO:
#if 0
  filter_operations_.Append(
      cc::FilterOperation::CreateBrightnessFilter(amount));
#endif
}

void CompositorFilterOperations::AppendContrastFilter(float amount) {
  ASSERT(false); // BKTODO: filter_operations_.Append(cc::FilterOperation::CreateContrastFilter(amount));
}

void CompositorFilterOperations::AppendOpacityFilter(float amount) {
  ASSERT(false); // BKTODO: filter_operations_.Append(cc::FilterOperation::CreateOpacityFilter(amount));
}

void CompositorFilterOperations::AppendBlurFilter(float amount) {
  ASSERT(false); // BKTODO: filter_operations_.Append(cc::FilterOperation::CreateBlurFilter(amount));
}

void CompositorFilterOperations::AppendDropShadowFilter(IntPoint offset,
                                                        float std_deviation,
                                                        Color color) {
  ASSERT(false); // BKTODO:
#if 0
  gfx::Point gfx_offset(offset.X(), offset.Y());
  filter_operations_.Append(cc::FilterOperation::CreateDropShadowFilter(
      gfx_offset, std_deviation, color.Rgb()));
#endif
}

#if 0 // BKTODO:
void CompositorFilterOperations::AppendColorMatrixFilter(
    const cc::FilterOperation::Matrix& matrix) {
  filter_operations_.Append(
      cc::FilterOperation::CreateColorMatrixFilter(matrix));
}
#endif

void CompositorFilterOperations::AppendZoomFilter(float amount, int inset) {
  ASSERT(false); // BKTODO:
#if 0
  filter_operations_.Append(
      cc::FilterOperation::CreateZoomFilter(amount, inset));
#endif
}

void CompositorFilterOperations::AppendSaturatingBrightnessFilter(
    float amount) {
  ASSERT(false); // BKTODO:
#if 0
  filter_operations_.Append(
      cc::FilterOperation::CreateSaturatingBrightnessFilter(amount));
#endif
}

#if 0 // BKTODO:
void CompositorFilterOperations::AppendReferenceFilter(
    sk_sp<PaintFilter> image_filter) {
  filter_operations_.Append(
      cc::FilterOperation::CreateReferenceFilter(std::move(image_filter)));
}
#endif

void CompositorFilterOperations::Clear() {
  ASSERT(false); // BKTODO: filter_operations_.Clear();
}

bool CompositorFilterOperations::IsEmpty() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return filter_operations_.IsEmpty();
#endif
}

FloatRect CompositorFilterOperations::MapRect(
    const FloatRect& input_rect) const {
  ASSERT(false); // BKTODO:
  return FloatRect();
#if 0
  gfx::Rect result =
      filter_operations_.MapRect(EnclosingIntRect(input_rect), SkMatrix::I());
  return FloatRect(result.x(), result.y(), result.width(), result.height());
#endif
}

bool CompositorFilterOperations::HasFilterThatMovesPixels() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return filter_operations_.HasFilterThatMovesPixels();
#endif
}

bool CompositorFilterOperations::operator==(
    const CompositorFilterOperations& o) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return reference_box_ == o.reference_box_ &&
         filter_operations_ == o.filter_operations_;
#endif
}

String CompositorFilterOperations::ToString() const {
  ASSERT(false); // BKTODO:
  return String();
#if 0
  return filter_operations_.ToString().c_str();
#endif
}

}  // namespace blink
