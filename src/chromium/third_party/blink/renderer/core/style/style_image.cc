// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: style_image.cc
// Description: StyleImage Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/style/style_image.h"

#if 0 // BKTODO:
#include "third_party/blink/renderer/core/svg/graphics/svg_image.h"
#include "third_party/blink/renderer/core/svg/graphics/svg_image_for_container.h"
#endif
#include "third_party/blink/renderer/platform/geometry/layout_size.h"

namespace blink {

FloatSize StyleImage::ApplyZoom(const FloatSize& size, float multiplier) const {
  if (multiplier == 1.0f || ImageHasRelativeSize())
    return size;

  float width = size.Width() * multiplier;
  float height = size.Height() * multiplier;

  // Don't let images that have a width/height >= 1 shrink below 1 when zoomed.
  if (size.Width() > 0)
    width = std::max(1.0f, width);

  if (size.Height() > 0)
    height = std::max(1.0f, height);

  return FloatSize(width, height);
}

FloatSize StyleImage::ImageSizeForSVGImage(
    SVGImage* svg_image,
    float multiplier,
    const LayoutSize& default_object_size) const {
  FloatSize unzoomed_default_object_size(default_object_size);
  unzoomed_default_object_size.Scale(1 / multiplier);
  // FIXME(schenney): Remove this rounding hack once background image
  // geometry is converted to handle rounding downstream.
  ASSERT(false); // BKTODO:
  return FloatSize();
#if 0
  return FloatSize(RoundedIntSize(
      ApplyZoom(svg_image->ConcreteObjectSize(unzoomed_default_object_size),
                multiplier)));
#endif
}

}  // namespace blink
