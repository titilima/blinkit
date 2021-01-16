// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: pattern.cc
// Description: DisplayItem Class
//      Author: Ziming Li
//     Created: 2020-10-10
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2013 Google, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/platform/graphics/pattern.h"

// BKTODO: #include "third_party/blink/renderer/platform/graphics/image_pattern.h"
#include "third_party/blink/renderer/platform/graphics/paint/paint_flags.h"
#include "third_party/blink/renderer/platform/graphics/paint/paint_record.h"
// BKTODO: #include "third_party/blink/renderer/platform/graphics/paint_record_pattern.h"
#include "third_party/blink/renderer/platform/graphics/skia/skia_utils.h"
#include "third_party/skia/include/core/SkImage.h"
#include "third_party/skia/include/core/SkShader.h"

namespace blink {

scoped_refptr<Pattern> Pattern::CreateImagePattern(
    scoped_refptr<Image> tile_image,
    RepeatMode repeat_mode) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return ImagePattern::Create(std::move(tile_image), repeat_mode);
#endif
}

scoped_refptr<Pattern> Pattern::CreatePaintRecordPattern(
    sk_sp<PaintRecord> record,
    const FloatRect& record_bounds,
    RepeatMode repeat_mode) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return PaintRecordPattern::Create(std::move(record), record_bounds,
                                    repeat_mode);
#endif
}

Pattern::Pattern(RepeatMode repeat_mode) : repeat_mode_(repeat_mode) {}

Pattern::~Pattern() = default;

void Pattern::ApplyToFlags(PaintFlags& flags, const SkMatrix& local_matrix) {
  if (!cached_shader_ || IsLocalMatrixChanged(local_matrix))
    cached_shader_ = CreateShader(local_matrix);

  flags.setShader(cached_shader_);
}

bool Pattern::IsLocalMatrixChanged(const SkMatrix& local_matrix) const {
  return local_matrix != cached_shader_->GetLocalMatrix();
}

}  // namespace blink
