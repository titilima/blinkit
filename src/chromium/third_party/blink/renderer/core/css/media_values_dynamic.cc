// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: media_values_dynamic.cc
// Description: MediaValuesDynamic Class
//      Author: Ziming Li
//     Created: 2020-09-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/media_values_dynamic.h"

#include "third_party/blink/renderer/core/css/css_primitive_value.h"
#include "third_party/blink/renderer/core/css/css_resolution_units.h"
#include "third_party/blink/renderer/core/css/css_to_length_conversion_data.h"
#include "third_party/blink/renderer/core/css/media_values_cached.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

namespace blink {

MediaValues* MediaValuesDynamic::Create(Document& document) {
  return MediaValuesDynamic::Create(document.GetFrameOfMasterDocument());
}

MediaValues* MediaValuesDynamic::Create(LocalFrame* frame) {
  if (!frame || !frame->View() || !frame->GetDocument() ||
      !frame->GetDocument()->GetLayoutView())
    return MediaValuesCached::Create();
  return new MediaValuesDynamic(frame);
}

MediaValuesDynamic::MediaValuesDynamic(LocalFrame* frame)
    : frame_(frame),
      viewport_dimensions_overridden_(false),
      viewport_width_override_(0),
      viewport_height_override_(0) {
  DCHECK(frame_);
}

MediaValuesDynamic::MediaValuesDynamic(LocalFrame* frame,
                                       bool overridden_viewport_dimensions,
                                       double viewport_width,
                                       double viewport_height)
    : frame_(frame),
      viewport_dimensions_overridden_(overridden_viewport_dimensions),
      viewport_width_override_(viewport_width),
      viewport_height_override_(viewport_height) {
  DCHECK(frame_);
}

MediaValues* MediaValuesDynamic::Copy() const {
  return new MediaValuesDynamic(frame_, viewport_dimensions_overridden_,
                                viewport_width_override_,
                                viewport_height_override_);
}

bool MediaValuesDynamic::ComputeLength(double value,
                                       CSSPrimitiveValue::UnitType type,
                                       int& result) const {
  return MediaValues::ComputeLength(
      value, type, CalculateDefaultFontSize(frame_),
      CalculateViewportWidth(frame_), CalculateViewportHeight(frame_), result);
}

bool MediaValuesDynamic::ComputeLength(double value,
                                       CSSPrimitiveValue::UnitType type,
                                       double& result) const {
  return MediaValues::ComputeLength(
      value, type, CalculateDefaultFontSize(frame_),
      CalculateViewportWidth(frame_), CalculateViewportHeight(frame_), result);
}

double MediaValuesDynamic::ViewportWidth() const {
  if (viewport_dimensions_overridden_)
    return viewport_width_override_;
  return CalculateViewportWidth(frame_);
}

double MediaValuesDynamic::ViewportHeight() const {
  if (viewport_dimensions_overridden_)
    return viewport_height_override_;
  return CalculateViewportHeight(frame_);
}

int MediaValuesDynamic::DeviceWidth() const {
  return CalculateDeviceWidth(frame_);
}

int MediaValuesDynamic::DeviceHeight() const {
  return CalculateDeviceHeight(frame_);
}

float MediaValuesDynamic::DevicePixelRatio() const {
  return CalculateDevicePixelRatio(frame_);
}

int MediaValuesDynamic::ColorBitsPerComponent() const {
  return CalculateColorBitsPerComponent(frame_);
}

int MediaValuesDynamic::MonochromeBitsPerComponent() const {
  return CalculateMonochromeBitsPerComponent(frame_);
}

PointerType MediaValuesDynamic::PrimaryPointerType() const {
  return CalculatePrimaryPointerType(frame_);
}

int MediaValuesDynamic::AvailablePointerTypes() const {
  return CalculateAvailablePointerTypes(frame_);
}

HoverType MediaValuesDynamic::PrimaryHoverType() const {
  return CalculatePrimaryHoverType(frame_);
}

int MediaValuesDynamic::AvailableHoverTypes() const {
  return CalculateAvailableHoverTypes(frame_);
}

bool MediaValuesDynamic::ThreeDEnabled() const {
  return CalculateThreeDEnabled(frame_);
}

bool MediaValuesDynamic::InImmersiveMode() const {
  return CalculateInImmersiveMode(frame_);
}

const String MediaValuesDynamic::MediaType() const {
  return CalculateMediaType(frame_);
}

#if 0 // BKTODO:
WebDisplayMode MediaValuesDynamic::DisplayMode() const {
  return CalculateDisplayMode(frame_);
}
#endif

bool MediaValuesDynamic::StrictMode() const {
  return CalculateStrictMode(frame_);
}

DisplayShape MediaValuesDynamic::GetDisplayShape() const {
  return CalculateDisplayShape(frame_);
}

ColorSpaceGamut MediaValuesDynamic::ColorGamut() const {
  return CalculateColorGamut(frame_);
}

Document* MediaValuesDynamic::GetDocument() const {
  return frame_->GetDocument();
}

bool MediaValuesDynamic::HasValues() const {
  return frame_;
}

void MediaValuesDynamic::OverrideViewportDimensions(double width,
                                                    double height) {
  viewport_dimensions_overridden_ = true;
  viewport_width_override_ = width;
  viewport_height_override_ = height;
}

}  // namespace blink
