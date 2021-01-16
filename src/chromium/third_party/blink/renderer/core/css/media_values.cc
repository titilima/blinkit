// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: media_values.cc
// Description: MediaValues Class
//      Author: Ziming Li
//     Created: 2020-09-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/media_values.h"

// BKTODO: #include "third_party/blink/public/platform/web_screen_info.h"
#include "third_party/blink/renderer/core/css/css_resolution_units.h"
#include "third_party/blink/renderer/core/css/media_values_cached.h"
#include "third_party/blink/renderer/core/css/media_values_dynamic.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/core/layout/layout_object.h"
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/paint/compositing/paint_layer_compositor.h"
#include "third_party/blink/renderer/platform/graphics/color_space_gamut.h"

namespace blink {

MediaValues* MediaValues::CreateDynamicIfFrameExists(LocalFrame* frame) {
  if (frame)
    return MediaValuesDynamic::Create(frame);
  return MediaValuesCached::Create();
}

double MediaValues::CalculateViewportWidth(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->View());
  DCHECK(frame->GetDocument());
  return frame->View()->ViewportSizeForMediaQueries().Width();
}

double MediaValues::CalculateViewportHeight(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->View());
  DCHECK(frame->GetDocument());
  return frame->View()->ViewportSizeForMediaQueries().Height();
}

int MediaValues::CalculateDeviceWidth(LocalFrame* frame) {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  DCHECK(frame && frame->View() && frame->GetSettings() && frame->GetPage());
  blink::WebScreenInfo screen_info =
      frame->GetPage()->GetChromeClient().GetScreenInfo();
  int device_width = screen_info.rect.width;
  if (frame->GetSettings()->GetReportScreenSizeInPhysicalPixelsQuirk()) {
    device_width = static_cast<int>(
        lroundf(device_width * screen_info.device_scale_factor));
  }
  return device_width;
#endif
}

int MediaValues::CalculateDeviceHeight(LocalFrame* frame) {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  DCHECK(frame && frame->View() && frame->GetSettings() && frame->GetPage());
  blink::WebScreenInfo screen_info =
      frame->GetPage()->GetChromeClient().GetScreenInfo();
  int device_height = screen_info.rect.height;
  if (frame->GetSettings()->GetReportScreenSizeInPhysicalPixelsQuirk()) {
    device_height = static_cast<int>(
        lroundf(device_height * screen_info.device_scale_factor));
  }
  return device_height;
#endif
}

bool MediaValues::CalculateStrictMode(LocalFrame* frame) {
  DCHECK(frame);
  DCHECK(frame->GetDocument());
  return !frame->GetDocument()->InQuirksMode();
}

float MediaValues::CalculateDevicePixelRatio(LocalFrame* frame) {
  return frame->DevicePixelRatio();
}

int MediaValues::CalculateColorBitsPerComponent(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  DCHECK(frame->GetPage());
  if (frame->GetPage()->GetChromeClient().GetScreenInfo().is_monochrome)
    return 0;
  return frame->GetPage()
      ->GetChromeClient()
      .GetScreenInfo()
      .depth_per_component;
#endif
}

int MediaValues::CalculateMonochromeBitsPerComponent(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  DCHECK(frame->GetPage());
  if (!frame->GetPage()->GetChromeClient().GetScreenInfo().is_monochrome)
    return 0;
  return frame->GetPage()
      ->GetChromeClient()
      .GetScreenInfo()
      .depth_per_component;
#endif
}

int MediaValues::CalculateDefaultFontSize(LocalFrame* frame) {
  return Settings::DefaultFontSize;
}

const String MediaValues::CalculateMediaType(LocalFrame* frame) {
  DCHECK(frame);
  if (!frame->View())
    return g_empty_atom;
  ASSERT(false); // BKTODO:
  return g_empty_atom;
#if 0
  return frame->View()->MediaType();
#endif
}

#if 0 // BKTODO:
WebDisplayMode MediaValues::CalculateDisplayMode(LocalFrame* frame) {
  DCHECK(frame);
  WebDisplayMode mode =
      frame->GetPage()->GetSettings().GetDisplayModeOverride();

  if (mode != kWebDisplayModeUndefined)
    return mode;

  if (!frame->View())
    return kWebDisplayModeBrowser;

  return frame->View()->DisplayMode();
}
#endif

bool MediaValues::CalculateThreeDEnabled(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
#if 0
  DCHECK(frame->ContentLayoutObject());
  DCHECK(frame->ContentLayoutObject()->Compositor());
#endif
  bool three_d_enabled = false;
#if 0 // BKTODO:
  if (LayoutView* view = frame->ContentLayoutObject())
    three_d_enabled = view->Compositor()->HasAcceleratedCompositing();
#endif
  return three_d_enabled;
}

bool MediaValues::CalculateInImmersiveMode(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
  return false;
#if 0
  DCHECK(frame->GetSettings());
  return frame->GetSettings()->GetImmersiveModeEnabled();
#endif
}

PointerType MediaValues::CalculatePrimaryPointerType(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
  return kPointerTypeNone;
#if 0
  DCHECK(frame->GetSettings());
  return frame->GetSettings()->GetPrimaryPointerType();
#endif
}

int MediaValues::CalculateAvailablePointerTypes(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  DCHECK(frame->GetSettings());
  return frame->GetSettings()->GetAvailablePointerTypes();
#endif
}

HoverType MediaValues::CalculatePrimaryHoverType(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
  return kHoverTypeNone;
#if 0
  DCHECK(frame->GetSettings());
  return frame->GetSettings()->GetPrimaryHoverType();
#endif
}

int MediaValues::CalculateAvailableHoverTypes(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  DCHECK(frame->GetSettings());
  return frame->GetSettings()->GetAvailableHoverTypes();
#endif
}

DisplayShape MediaValues::CalculateDisplayShape(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
  return kDisplayShapeRect;
#if 0
  DCHECK(frame->GetPage());
  return frame->GetPage()->GetChromeClient().GetScreenInfo().display_shape;
#endif
}

ColorSpaceGamut MediaValues::CalculateColorGamut(LocalFrame* frame) {
  DCHECK(frame);
  ASSERT(false); // BKTODO:
  return ColorSpaceGamut::kUnknown;
#if 0
  DCHECK(frame->GetPage());
  return color_space_utilities::GetColorSpaceGamut(
      frame->GetPage()->GetChromeClient().GetScreenInfo());
#endif
}

bool MediaValues::ComputeLengthImpl(double value,
                                    CSSPrimitiveValue::UnitType type,
                                    unsigned default_font_size,
                                    double viewport_width,
                                    double viewport_height,
                                    double& result) {
  // The logic in this function is duplicated from
  // CSSToLengthConversionData::ZoomedComputedPixels() because
  // MediaValues::ComputeLength() needs nearly identical logic, but we haven't
  // found a way to make CSSToLengthConversionData::ZoomedComputedPixels() more
  // generic (to solve both cases) without hurting performance.
  // FIXME - Unite the logic here with CSSToLengthConversionData in a performant
  // way.
  switch (type) {
    case CSSPrimitiveValue::UnitType::kEms:
    case CSSPrimitiveValue::UnitType::kRems:
      result = value * default_font_size;
      return true;
    case CSSPrimitiveValue::UnitType::kPixels:
    case CSSPrimitiveValue::UnitType::kUserUnits:
      result = value;
      return true;
    case CSSPrimitiveValue::UnitType::kExs:
    // FIXME: We have a bug right now where the zoom will be applied twice to EX
    // units.
    case CSSPrimitiveValue::UnitType::kChs:
      // FIXME: We don't seem to be able to cache fontMetrics related values.
      // Trying to access them is triggering some sort of microtask. Serving the
      // spec's default instead.
      result = (value * default_font_size) / 2.0;
      return true;
    case CSSPrimitiveValue::UnitType::kViewportWidth:
      result = (value * viewport_width) / 100.0;
      return true;
    case CSSPrimitiveValue::UnitType::kViewportHeight:
      result = (value * viewport_height) / 100.0;
      return true;
    case CSSPrimitiveValue::UnitType::kViewportMin:
      result = (value * std::min(viewport_width, viewport_height)) / 100.0;
      return true;
    case CSSPrimitiveValue::UnitType::kViewportMax:
      result = (value * std::max(viewport_width, viewport_height)) / 100.0;
      return true;
    case CSSPrimitiveValue::UnitType::kCentimeters:
      result = value * kCssPixelsPerCentimeter;
      return true;
    case CSSPrimitiveValue::UnitType::kMillimeters:
      result = value * kCssPixelsPerMillimeter;
      return true;
    case CSSPrimitiveValue::UnitType::kQuarterMillimeters:
      result = value * kCssPixelsPerQuarterMillimeter;
      return true;
    case CSSPrimitiveValue::UnitType::kInches:
      result = value * kCssPixelsPerInch;
      return true;
    case CSSPrimitiveValue::UnitType::kPoints:
      result = value * kCssPixelsPerPoint;
      return true;
    case CSSPrimitiveValue::UnitType::kPicas:
      result = value * kCssPixelsPerPica;
      return true;
    default:
      return false;
  }
}

}  // namespace blink
