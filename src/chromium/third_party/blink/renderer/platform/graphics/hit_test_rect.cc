// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: hit_test_rect.cc
// Description: HitTestRect Class
//      Author: Ziming Li
//     Created: 2020-10-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/graphics/hit_test_rect.h"

#if 0 // BKTODO:
#include "base/containers/flat_map.h"
#include "cc/base/region.h"
#include "cc/layers/touch_action_region.h"
#endif

namespace blink {

#if 0 // BKTODO:
// static
cc::TouchActionRegion HitTestRect::BuildRegion(
    const Vector<HitTestRect>& hit_test_rects) {
  base::flat_map<TouchAction, cc::Region> region_map;
  region_map.reserve(hit_test_rects.size());
  for (const HitTestRect& hit_test_rect : hit_test_rects) {
    const TouchAction& action = hit_test_rect.whitelisted_touch_action;
    const LayoutRect& rect = hit_test_rect.rect;
    region_map[action].Union(EnclosingIntRect(rect));
  }
  return cc::TouchActionRegion(std::move(region_map));
}
#endif

// static
LayoutRect HitTestRect::GetBounds(const Vector<HitTestRect>& hit_test_rects) {
  ASSERT(false); // BKTODO:
  return LayoutRect();
#if 0
  cc::Region region;
  for (const HitTestRect& hit_test_rect : hit_test_rects) {
    const LayoutRect& rect = hit_test_rect.rect;
    region.Union(EnclosingIntRect(rect));
  }
  const auto& rect = region.bounds();
  return LayoutRect(IntRect(rect));
#endif
}

}  // namespace blink
