// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: color_behavior.cc
// Description: ColorBehavior Class
//      Author: Ziming Li
//     Created: 2021-01-11
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/graphics/color_behavior.h"

#include "base/metrics/histogram_macros.h"
#include "third_party/blink/renderer/platform/graphics/bitmap_image_metrics.h"
#include "third_party/skia/include/core/SkICC.h"
#if 0 // BKTODO:
#include "ui/gfx/icc_profile.h"
#include "ui/gfx/skia_color_space_util.h"
#endif

namespace blink {

bool ColorBehavior::operator==(const ColorBehavior& other) const {
  if (type_ != other.type_)
    return false;
  return true;
}

bool ColorBehavior::operator!=(const ColorBehavior& other) const {
  return !(*this == other);
}

}  // namespace blink
