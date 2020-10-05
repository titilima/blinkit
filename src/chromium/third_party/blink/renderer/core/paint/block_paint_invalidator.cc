// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: block_paint_invalidator.cc
// Description: BlockPaintInvalidator Class
//      Author: Ziming Li
//     Created: 2020-10-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/paint/block_paint_invalidator.h"

#include "third_party/blink/renderer/core/editing/drag_caret.h"
#include "third_party/blink/renderer/core/editing/frame_selection.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/layout/layout_block.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/paint/box_paint_invalidator.h"
#include "third_party/blink/renderer/core/paint/object_paint_invalidator.h"
#include "third_party/blink/renderer/core/paint/paint_invalidator.h"

namespace blink {

void BlockPaintInvalidator::ClearPreviousVisualRects() {
  ASSERT(false); // BKTODO:
#if 0
  block_.GetFrame()->Selection().ClearPreviousCaretVisualRect(block_);
  block_.GetFrame()->GetPage()->GetDragCaret().ClearPreviousVisualRect(block_);
#endif
}

void BlockPaintInvalidator::InvalidatePaint(
    const PaintInvalidatorContext& context) {
  BoxPaintInvalidator(block_, context).InvalidatePaint();

  ASSERT(false); // BKTODO:
#if 0
  block_.GetFrame()->Selection().InvalidatePaint(block_, context);
  block_.GetFrame()->GetPage()->GetDragCaret().InvalidatePaint(block_, context);
#endif
}

}  // namespace blink
