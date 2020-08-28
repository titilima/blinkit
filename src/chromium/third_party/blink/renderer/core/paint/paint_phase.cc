// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/paint/paint_phase.h"

#include "third_party/blink/renderer/platform/graphics/paint/display_item.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"

namespace blink {

static_assert(static_cast<PaintPhase>(DisplayItem::kPaintPhaseMax) ==
                  PaintPhase::kMax,
              "DisplayItem Type and PaintPhase should stay in sync");

}  // namespace blink
