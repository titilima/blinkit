// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: compositor_filter_operations.h
// Description: CompositorFilterOperations Class
//      Author: Ziming Li
//     Created: 2020-08-28
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_COMPOSITOR_FILTER_OPERATIONS_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_COMPOSITOR_FILTER_OPERATIONS_H_

// BKTODO: #include "cc/paint/filter_operations.h"
#include "third_party/blink/renderer/platform/geometry/float_rect.h"
#include "third_party/blink/renderer/platform/geometry/int_point.h"
#include "third_party/blink/renderer/platform/graphics/color.h"
// BKTODO: #include "third_party/blink/renderer/platform/graphics/paint/paint_filter.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "third_party/skia/include/core/SkScalar.h"

namespace blink {

// An ordered list of filter operations.
class PLATFORM_EXPORT CompositorFilterOperations {
 public:
#if 0 // BKTODO:
  const cc::FilterOperations& AsCcFilterOperations() const;
  cc::FilterOperations ReleaseCcFilterOperations();
#endif

  void AppendGrayscaleFilter(float amount);
  void AppendSepiaFilter(float amount);
  void AppendSaturateFilter(float amount);
  void AppendHueRotateFilter(float amount);
  void AppendInvertFilter(float amount);
  void AppendBrightnessFilter(float amount);
  void AppendContrastFilter(float amount);
  void AppendOpacityFilter(float amount);
  void AppendBlurFilter(float amount);
  void AppendDropShadowFilter(IntPoint offset, float std_deviation, Color);
#if 0 // BKTODO:
  void AppendColorMatrixFilter(const cc::FilterOperation::Matrix&);
#endif
  void AppendZoomFilter(float amount, int inset);
  void AppendSaturatingBrightnessFilter(float amount);

#if 0 // BKTODO:
  void AppendReferenceFilter(sk_sp<PaintFilter>);
#endif

  void Clear();
  bool IsEmpty() const;

  // Returns a rect covering the destination pixels that can be affected by
  // source pixels in |inputRect|.
  FloatRect MapRect(const FloatRect& input_rect) const;

  bool HasFilterThatMovesPixels() const;

  void SetReferenceBox(const FloatRect& r) { reference_box_ = r; }
  FloatRect ReferenceBox() const { return reference_box_; }

  // For reference filters, this equality operator compares pointers of the
  // image_filter fields instead of their values.
  bool operator==(const CompositorFilterOperations&) const;
  bool operator!=(const CompositorFilterOperations& o) const {
    return !(*this == o);
  }

  String ToString() const;

 private:
#if 0 // BKTODO:
  cc::FilterOperations filter_operations_;
#endif
  FloatRect reference_box_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_COMPOSITOR_FILTER_OPERATIONS_H_
