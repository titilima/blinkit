// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: inline_painter.cc
// Description: InlinePainter Class
//      Author: Ziming Li
//     Created: 2020-10-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/paint/inline_painter.h"

#include "third_party/blink/renderer/core/layout/layout_block_flow.h"
#include "third_party/blink/renderer/core/layout/layout_inline.h"
#include "third_party/blink/renderer/core/paint/line_box_list_painter.h"
#include "third_party/blink/renderer/core/paint/ng/ng_paint_fragment.h"
// BKTODO: #include "third_party/blink/renderer/core/paint/object_painter.h"
#include "third_party/blink/renderer/core/paint/paint_info.h"
#include "third_party/blink/renderer/core/paint/scoped_paint_state.h"
#include "third_party/blink/renderer/platform/geometry/layout_point.h"

namespace blink {

void InlinePainter::Paint(const PaintInfo& paint_info) {
  ScopedPaintState paint_state(layout_inline_, paint_info);
  auto paint_offset = paint_state.PaintOffset();
  const auto& local_paint_info = paint_state.GetPaintInfo();

  if (RuntimeEnabledFeatures::LayoutNGEnabled()) {
    // Inline box with self painting layer is painted in this code path.
    if (auto* block_flow = layout_inline_.EnclosingNGBlockFlow()) {
      if (auto* block_flow_fragment = block_flow->PaintFragment()) {
        block_flow_fragment->PaintInlineBoxForDescendants(
            local_paint_info, paint_offset, &layout_inline_);
        return;
      }
    }
  }

  if (local_paint_info.phase == PaintPhase::kForeground &&
      local_paint_info.IsPrinting()) {
    ASSERT(false); // BKTODO:
#if 0
    ObjectPainter(layout_inline_)
        .AddPDFURLRectIfNeeded(local_paint_info, paint_offset);
#endif
  }

  if (ShouldPaintSelfOutline(local_paint_info.phase) ||
      ShouldPaintDescendantOutlines(local_paint_info.phase)) {
    ASSERT(false); // BKTODO:
#if 0
    ObjectPainter painter(layout_inline_);
    if (ShouldPaintDescendantOutlines(local_paint_info.phase))
      painter.PaintInlineChildrenOutlines(local_paint_info);
    if (ShouldPaintSelfOutline(local_paint_info.phase) &&
        !layout_inline_.IsElementContinuation())
      painter.PaintOutline(local_paint_info, paint_offset);
#endif
    return;
  }

  LineBoxListPainter(*layout_inline_.LineBoxes())
      .Paint(layout_inline_, local_paint_info, paint_offset);
}

}  // namespace blink
