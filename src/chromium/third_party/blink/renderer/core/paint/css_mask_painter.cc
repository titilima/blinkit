// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_mask_painter.cc
// Description: CSSMaskPainter Class
//      Author: Ziming Li
//     Created: 2020-10-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/paint/css_mask_painter.h"

#include "third_party/blink/renderer/core/layout/layout_block.h"
#include "third_party/blink/renderer/core/layout/layout_inline.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/layout/svg/layout_svg_resource_masker.h"
#include "third_party/blink/renderer/core/layout/svg/svg_resources.h"
#include "third_party/blink/renderer/core/layout/svg/svg_resources_cache.h"
#endif

namespace blink {

base::Optional<IntRect> CSSMaskPainter::MaskBoundingBox(
    const LayoutObject& object,
    const LayoutPoint& paint_offset) {
  if (!object.IsBoxModelObject() && !object.IsSVGChild())
    return base::nullopt;

  if (object.IsSVG()) {
    ASSERT(false); // BKTODO:
#if 0
    SVGResources* resources =
        SVGResourcesCache::CachedResourcesForLayoutObject(object);
    LayoutSVGResourceMasker* masker = resources ? resources->Masker() : nullptr;
    if (masker) {
      return EnclosingIntRect(
          masker->ResourceBoundingBox(object.ObjectBoundingBox()));
    }
#endif
  }

  if (object.IsSVGChild() && !object.IsSVGForeignObject())
    return base::nullopt;

  const ComputedStyle& style = object.StyleRef();
  if (!style.HasMask())
    return base::nullopt;

  LayoutRect maximum_mask_region;
  // For HTML/CSS objects, the extent of the mask is known as "mask
  // painting area", which is determined by CSS mask-clip property.
  // We don't implement mask-clip:margin-box or no-clip currently,
  // so the maximum we can get is border-box.
  if (object.IsBox()) {
    maximum_mask_region = ToLayoutBox(object).BorderBoxRect();
  } else {
    // For inline elements, depends on the value of box-decoration-break
    // there could be one box in multiple fragments or multiple boxes.
    // Either way here we are only interested in the bounding box of them.
    DCHECK(object.IsLayoutInline());
    maximum_mask_region = ToLayoutInline(object).LinesBoundingBox();
    if (object.HasFlippedBlocksWritingMode())
      object.ContainingBlock()->FlipForWritingMode(maximum_mask_region);
  }
  if (style.HasMaskBoxImageOutsets())
    maximum_mask_region.Expand(style.MaskBoxImageOutsets());
  maximum_mask_region.MoveBy(paint_offset);
  return PixelSnappedIntRect(maximum_mask_region);
}

ColorFilter CSSMaskPainter::MaskColorFilter(const LayoutObject& object) {
  if (!object.IsSVGChild())
    return kColorFilterNone;

  ASSERT(false); // BKTODO:
  return kColorFilterNone;
#if 0
  SVGResources* resources =
      SVGResourcesCache::CachedResourcesForLayoutObject(object);
  LayoutSVGResourceMasker* masker = resources ? resources->Masker() : nullptr;
  if (!masker)
    return kColorFilterNone;
  return masker->StyleRef().SvgStyle().MaskType() == MT_LUMINANCE
             ? kColorFilterLuminanceToAlpha
             : kColorFilterNone;
#endif
}

}  // namespace blink
