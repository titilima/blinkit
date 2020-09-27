// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: drawing_display_item.h
// Description: DrawingDisplayItem Class
//      Author: Ziming Li
//     Created: 2020-09-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_PAINT_DRAWING_DISPLAY_ITEM_H_
#define THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_PAINT_DRAWING_DISPLAY_ITEM_H_

#include "base/compiler_specific.h"
#include "third_party/blink/renderer/platform/graphics/paint/display_item.h"
#include "third_party/blink/renderer/platform/graphics/paint/paint_record.h"
#include "third_party/blink/renderer/platform/platform_export.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/skia/include/core/SkRefCnt.h"

namespace blink {

// DrawingDisplayItem contains recorded painting operations which can be
// replayed to produce a rastered output.
//
// This class has two notions of the bounds around the content that was recorded
// and will be produced by the item. The first is the |record_bounds| which
// describes the bounds of all content in the |record| in the space of the
// record. The second is the |visual_rect| which should describe the same thing,
// but takes into account transforms and clips that would apply to the
// PaintRecord, and is in the space of the DisplayItemList. This allows the
// visual_rect to be compared between DrawingDisplayItems, and to give bounds
// around what the user can actually see from the PaintRecord.
class PLATFORM_EXPORT DrawingDisplayItem final : public DisplayItem {
 public:
#if 0 // BKTODO:
  DISABLE_CFI_PERF
  DrawingDisplayItem(const DisplayItemClient& client,
                     Type type,
                     sk_sp<const PaintRecord> record,
                     bool known_to_be_opaque);
#endif

  void Replay(GraphicsContext&) const override;
  void AppendToDisplayItemList(const FloatSize& visual_rect_offset,
                               cc::DisplayItemList&) const override;
  bool DrawsContent() const override;

#if 0 // BKTODO:
  const sk_sp<const PaintRecord>& GetPaintRecord() const { return record_; }
#endif

  bool KnownToBeOpaque() const {
    DCHECK(RuntimeEnabledFeatures::SlimmingPaintV2Enabled());
    return known_to_be_opaque_;
  }

  bool Equals(const DisplayItem& other) const final;

 private:
#if DCHECK_IS_ON()
  void PropertiesAsJSON(JSONObject&) const override;
#endif

#if 0 // BKTODO:
  sk_sp<const PaintRecord> record_;
#endif

  // True if there are no transparent areas. Only used for SlimmingPaintV2.
  const bool known_to_be_opaque_;
};

#if 0 // BKTODO:
// TODO(dcheng): Move this ctor back inline once the clang plugin is fixed.
DISABLE_CFI_PERF
inline DrawingDisplayItem::DrawingDisplayItem(const DisplayItemClient& client,
                                              Type type,
                                              sk_sp<const PaintRecord> record,
                                              bool known_to_be_opaque = false)
    : DisplayItem(client, type, sizeof(*this)),
      record_(record && record->size() ? std::move(record) : nullptr),
      known_to_be_opaque_(known_to_be_opaque) {
  DCHECK(IsDrawingType(type));
}
#endif

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_GRAPHICS_PAINT_DRAWING_DISPLAY_ITEM_H_
