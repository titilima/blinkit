// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: display_item_list.cc
// Description: DisplayItemList Class
//      Author: Ziming Li
//     Created: 2020-10-10
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/graphics/paint/display_item_list.h"

// BKTODO: #include "third_party/blink/renderer/platform/graphics/logging_canvas.h"
#include "third_party/blink/renderer/platform/graphics/paint/drawing_display_item.h"
#include "third_party/blink/renderer/platform/graphics/paint/paint_chunk.h"

namespace blink {

#if 0 // BKTODO:
DisplayItemList::Range<DisplayItemList::iterator>
DisplayItemList::ItemsInPaintChunk(const PaintChunk& paint_chunk) {
  return Range<iterator>(begin() + paint_chunk.begin_index,
                         begin() + paint_chunk.end_index);
}

DisplayItemList::Range<DisplayItemList::const_iterator>
DisplayItemList::ItemsInPaintChunk(const PaintChunk& paint_chunk) const {
  return Range<const_iterator>(begin() + paint_chunk.begin_index,
                               begin() + paint_chunk.end_index);
}
#endif

#if DCHECK_IS_ON()

std::unique_ptr<JSONArray> DisplayItemList::SubsequenceAsJSON(
    size_t begin_index,
    size_t end_index,
    JsonFlags flags) const {
  auto json_array = JSONArray::Create();
  AppendSubsequenceAsJSON(begin_index, end_index, flags, *json_array);
  return json_array;
}

void DisplayItemList::AppendSubsequenceAsJSON(size_t begin_index,
                                              size_t end_index,
                                              JsonFlags flags,
                                              JSONArray& json_array) const {
  ASSERT(false); // BKTODO:
#if 0
  for (size_t i = begin_index; i < end_index; ++i) {
    std::unique_ptr<JSONObject> json = JSONObject::Create();

    const auto& item = (*this)[i];
    if ((flags & kSkipNonDrawings) && !item.IsDrawing())
      continue;

    json->SetInteger("index", i);

    if (flags & kShownOnlyDisplayItemTypes) {
      json->SetString("type", DisplayItem::TypeAsDebugString(item.GetType()));
    } else {
      json->SetString("clientDebugName",
                      DisplayItemClient::SafeDebugName(
                          item.Client(), flags & kClientKnownToBeAlive));
      item.PropertiesAsJSON(*json);
    }

#if DCHECK_IS_ON()
    if ((flags & kShowPaintRecords) && item.IsDrawing()) {
      const auto& drawing_item = static_cast<const DrawingDisplayItem&>(item);
      if (const auto* record = drawing_item.GetPaintRecord().get())
        json->SetArray("record", RecordAsJSON(*record));
    }
#endif

    json_array.PushObject(std::move(json));
  }
#endif
}

#endif  // DCHECK_IS_ON()

}  // namespace blink
