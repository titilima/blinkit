// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: skia_text_metrics.h
// Description: SkiaTextMetrics Class
//      Author: Ziming Li
//     Created: 2020-12-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_FONTS_SKIA_SKIA_TEXT_METRICS_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_FONTS_SKIA_SKIA_TEXT_METRICS_H_

#include "third_party/blink/renderer/platform/fonts/glyph.h"

#include <SkPaint.h>
// BKTODO: #include <hb.h>
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class SkiaTextMetrics final {
 public:
  SkiaTextMetrics(const SkPaint*);

#if 0 // BKTODO:
  void GetGlyphWidthForHarfBuzz(hb_codepoint_t, hb_position_t* width);
  void GetGlyphWidthForHarfBuzz(unsigned count,
                                hb_codepoint_t* first_glyph,
                                unsigned glyph_stride,
                                hb_position_t* first_advance,
                                unsigned advance_stride);
  void GetGlyphExtentsForHarfBuzz(hb_codepoint_t, hb_glyph_extents_t*);
#endif

  void GetSkiaBoundsForGlyph(Glyph, SkRect* bounds);
  void GetSkiaBoundsForGlyphs(const Vector<Glyph, 256>&, SkRect*);
  float GetSkiaWidthForGlyph(Glyph);

  // BKTODO: static hb_position_t SkiaScalarToHarfBuzzPosition(SkScalar value);

 private:
  const SkPaint* paint_;
};

}  // namespace blink

#endif
