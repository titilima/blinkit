// -------------------------------------------------
// BlinKit - cc Library
// -------------------------------------------------
//   File Name: scoped_raster_flags.CC
// Description: ScopedRasterFlags Class
//      Author: Ziming Li
//     Created: 2020-12-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/paint/scoped_raster_flags.h"

#include "cc/paint/image_provider.h"
// BKTODO: #include "cc/paint/image_transfer_cache_entry.h"
#include "cc/paint/paint_filter.h"
// BKTODO: #include "cc/paint/paint_image_builder.h"

namespace cc {
ScopedRasterFlags::ScopedRasterFlags(const PaintFlags* flags,
                                     ImageProvider* image_provider,
                                     const SkMatrix& ctm,
                                     uint8_t alpha)
    : original_flags_(flags) {
  if (image_provider) {
    ASSERT(false); // BKTODO: decode_stashing_image_provider_.emplace(image_provider);

    // We skip the op if any images fail to decode.
    DecodeImageShader(ctm);
    if (decode_failed_)
      return;
    DecodeRecordShader(ctm);
    if (decode_failed_)
      return;
    DecodeFilter();
    if (decode_failed_)
      return;
  }

  if (alpha != 255) {
    DCHECK(flags->SupportsFoldingAlpha());
    MutableFlags()->setAlpha(SkMulDiv255Round(flags->getAlpha(), alpha));
  }

  AdjustStrokeIfNeeded(ctm);
}

ScopedRasterFlags::~ScopedRasterFlags() = default;

void ScopedRasterFlags::DecodeImageShader(const SkMatrix& ctm) {
  if (!flags()->HasShader() ||
      flags()->getShader()->shader_type() != PaintShader::Type::kImage)
    return;

  ASSERT(false); // BKTODO:
#if 0
  uint32_t transfer_cache_entry_id = kInvalidImageTransferCacheEntryId;
  SkFilterQuality raster_quality = flags()->getFilterQuality();
  bool transfer_cache_entry_needs_mips = false;
  auto decoded_shader = flags()->getShader()->CreateDecodedImage(
      ctm, flags()->getFilterQuality(), &*decode_stashing_image_provider_,
      &transfer_cache_entry_id, &raster_quality,
      &transfer_cache_entry_needs_mips);
  DCHECK_EQ(transfer_cache_entry_id, kInvalidImageTransferCacheEntryId);
  DCHECK_EQ(transfer_cache_entry_needs_mips, false);

  if (!decoded_shader) {
    decode_failed_ = true;
    return;
  }

  MutableFlags()->setFilterQuality(raster_quality);
  MutableFlags()->setShader(decoded_shader);
#endif
}

void ScopedRasterFlags::DecodeRecordShader(const SkMatrix& ctm) {
  if (!flags()->HasShader() ||
      flags()->getShader()->shader_type() != PaintShader::Type::kPaintRecord)
    return;

  ASSERT(false); // BKTODO:
#if 0
  // Only replace shaders with animated images. Creating transient shaders for
  // replacing decodes during raster results in cache misses in skia's picture
  // shader cache, which results in re-rasterizing the picture for every draw.
  if (flags()->getShader()->image_analysis_state() !=
      ImageAnalysisState::kAnimatedImages) {
    return;
  }

  gfx::SizeF raster_scale(1.f, 1.f);
  auto decoded_shader =
      flags()->getShader()->CreateScaledPaintRecord(ctm, &raster_scale);
  decoded_shader->CreateSkShader(&raster_scale,
                                 &*decode_stashing_image_provider_);
  MutableFlags()->setShader(std::move(decoded_shader));
#endif
}

void ScopedRasterFlags::DecodeFilter() {
  ASSERT(false); // BKTODO:
#if 0
  if (!flags()->getImageFilter() ||
      !flags()->getImageFilter()->has_discardable_images() ||
      flags()->getImageFilter()->image_analysis_state() !=
          ImageAnalysisState::kAnimatedImages) {
    return;
  }

  MutableFlags()->setImageFilter(flags()->getImageFilter()->SnapshotWithImages(
      &*decode_stashing_image_provider_));
#endif
}

void ScopedRasterFlags::AdjustStrokeIfNeeded(const SkMatrix& ctm) {
  // With anti-aliasing turned off, strokes with a device space width in (0, 1)
  // may not raster at all.  To avoid this, we have two options:
  //
  // 1) force a hairline stroke (stroke-width == 0)
  // 2) force anti-aliasing on

  SkSize scale;
  if (flags()->isAntiAlias() ||                          // safe to raster
      flags()->getStyle() == PaintFlags::kFill_Style ||  // not a stroke
      !flags()->getStrokeWidth() ||                      // explicit hairline
      !ctm.decomposeScale(&scale)) {                     // cannot decompose
    return;
  }

  const auto stroke_vec =
      SkVector::Make(flags()->getStrokeWidth() * scale.width(),
                     flags()->getStrokeWidth() * scale.height());
  if (stroke_vec.x() >= 1.f && stroke_vec.y() >= 1.f)
    return;  // safe to raster

  const auto can_substitute_hairline =
      flags()->getStrokeCap() == PaintFlags::kDefault_Cap &&
      flags()->getStrokeJoin() == PaintFlags::kDefault_Join;
  if (can_substitute_hairline && stroke_vec.x() < 1.f && stroke_vec.y() < 1.f) {
    // Use modulated hairline when possible, as it is faster and produces
    // results closer to the original intent.
    MutableFlags()->setStrokeWidth(0);
    MutableFlags()->setAlpha(std::round(
        flags()->getAlpha() * std::sqrt(stroke_vec.x() * stroke_vec.y())));
    return;
  }

  // Fall back to anti-aliasing.
  MutableFlags()->setAntiAlias(true);
}

}  // namespace cc
