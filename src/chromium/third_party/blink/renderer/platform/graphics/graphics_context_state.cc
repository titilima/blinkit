// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: graphics_context_state.cc
// Description: GraphicsContextState Class
//      Author: Ziming Li
//     Created: 2020-10-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/graphics/graphics_context_state.h"

#include "third_party/blink/renderer/platform/graphics/skia/skia_utils.h"

namespace blink {

static inline SkFilterQuality FilterQualityForPaint(
    InterpolationQuality quality) {
  // The filter quality "selected" here will primarily be used when painting a
  // primitive using one of the PaintFlags below. For the most part this will
  // not affect things that are part of the Image class hierarchy (which use
  // the unmodified m_interpolationQuality.)
  return quality != kInterpolationNone ? kLow_SkFilterQuality
                                       : kNone_SkFilterQuality;
}

GraphicsContextState::GraphicsContextState()
    : text_drawing_mode_(kTextModeFill),
      interpolation_quality_(kInterpolationDefault),
      save_count_(0),
      should_antialias_(true) {
  ASSERT(false); // BKTODO:
#if 0
  stroke_flags_.setStyle(PaintFlags::kStroke_Style);
  stroke_flags_.setStrokeWidth(SkFloatToScalar(stroke_data_.Thickness()));
  stroke_flags_.setStrokeCap(PaintFlags::kDefault_Cap);
  stroke_flags_.setStrokeJoin(PaintFlags::kDefault_Join);
  stroke_flags_.setStrokeMiter(SkFloatToScalar(stroke_data_.MiterLimit()));
  stroke_flags_.setFilterQuality(FilterQualityForPaint(interpolation_quality_));
  stroke_flags_.setAntiAlias(should_antialias_);
  fill_flags_.setFilterQuality(FilterQualityForPaint(interpolation_quality_));
  fill_flags_.setAntiAlias(should_antialias_);
#endif
}

GraphicsContextState::GraphicsContextState(const GraphicsContextState& other)
#if 0 // BKTODO:
    : stroke_flags_(other.stroke_flags_),
      fill_flags_(other.fill_flags_),
#else
    :
#endif
      stroke_data_(other.stroke_data_),
      text_drawing_mode_(other.text_drawing_mode_),
      interpolation_quality_(other.interpolation_quality_),
      save_count_(0),
      should_antialias_(other.should_antialias_) {}

void GraphicsContextState::Copy(const GraphicsContextState& source) {
  this->~GraphicsContextState();
  new (this) GraphicsContextState(source);
}

#if 0 // BKTODO:
const PaintFlags& GraphicsContextState::StrokeFlags(
    const int stroked_path_length,
    const int dash_thickness) const {
  stroke_data_.SetupPaintDashPathEffect(&stroke_flags_, stroked_path_length,
                                        dash_thickness);
  return stroke_flags_;
}
#endif

void GraphicsContextState::SetStrokeStyle(StrokeStyle style) {
  stroke_data_.SetStyle(style);
}

void GraphicsContextState::SetStrokeThickness(float thickness) {
  stroke_data_.SetThickness(thickness);
  ASSERT(false); // BKTODO: stroke_flags_.setStrokeWidth(SkFloatToScalar(thickness));
}

void GraphicsContextState::SetStrokeColor(const Color& color) {
  ASSERT(false); // BKTODO:
#if 0
  stroke_flags_.setColor(color.Rgb());
  stroke_flags_.setShader(nullptr);
#endif
}

void GraphicsContextState::SetLineCap(LineCap cap) {
  ASSERT(false); // BKTODO:
#if 0
  stroke_data_.SetLineCap(cap);
  stroke_flags_.setStrokeCap((PaintFlags::Cap)cap);
#endif
}

void GraphicsContextState::SetLineJoin(LineJoin join) {
  ASSERT(false); // BKTODO:
#if 0
  stroke_data_.SetLineJoin(join);
  stroke_flags_.setStrokeJoin((PaintFlags::Join)join);
#endif
}

void GraphicsContextState::SetMiterLimit(float miter_limit) {
  ASSERT(false); // BKTODO:
#if 0
  stroke_data_.SetMiterLimit(miter_limit);
  stroke_flags_.setStrokeMiter(SkFloatToScalar(miter_limit));
#endif
}

void GraphicsContextState::SetFillColor(const Color& color) {
  ASSERT(false); // BKTODO:
#if 0
  fill_flags_.setColor(color.Rgb());
  fill_flags_.setShader(nullptr);
#endif
}

// Shadow. (This will need tweaking if we use draw loopers for other things.)
void GraphicsContextState::SetDrawLooper(sk_sp<SkDrawLooper> draw_looper) {
  ASSERT(false); // BKTODO:
#if 0
  // Grab a new ref for stroke.
  stroke_flags_.setLooper(draw_looper);
  // Pass the existing ref to fill (to minimize refcount churn).
  fill_flags_.setLooper(std::move(draw_looper));
#endif
}

void GraphicsContextState::SetLineDash(const DashArray& dashes,
                                       float dash_offset) {
  stroke_data_.SetLineDash(dashes, dash_offset);
}

void GraphicsContextState::SetColorFilter(sk_sp<SkColorFilter> color_filter) {
  ASSERT(false); // BKTODO:
#if 0
  // Grab a new ref for stroke.
  stroke_flags_.setColorFilter(color_filter);
  // Pass the existing ref to fill (to minimize refcount churn).
  fill_flags_.setColorFilter(std::move(color_filter));
#endif
}

void GraphicsContextState::SetInterpolationQuality(
    InterpolationQuality quality) {
  interpolation_quality_ = quality;
  ASSERT(false); // BKTODO:
#if 0
  stroke_flags_.setFilterQuality(FilterQualityForPaint(quality));
  fill_flags_.setFilterQuality(FilterQualityForPaint(quality));
#endif
}

void GraphicsContextState::SetShouldAntialias(bool should_antialias) {
  should_antialias_ = should_antialias;
  ASSERT(false); // BKTODO:
#if 0
  stroke_flags_.setAntiAlias(should_antialias);
  fill_flags_.setAntiAlias(should_antialias);
#endif
}

}  // namespace blink
