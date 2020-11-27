// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_color_value.cc
// Description: CSSColorValue Class
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/css_color_value.h"

#include "third_party/blink/renderer/core/css/css_value_pool.h"

namespace blink {
namespace cssvalue {

CSSColorValue* CSSColorValue::Create(RGBA32 color) {
  // These are the empty and deleted values of the hash table.
  if (color == Color::kTransparent)
    return CssValuePool().TransparentColor();
  if (color == Color::kWhite)
    return CssValuePool().WhiteColor();
  // Just because it is common.
  if (color == Color::kBlack)
    return CssValuePool().BlackColor();

  Member<CSSColorValue> &entry =
      CssValuePool().GetColorCacheEntry(color);
  if (!entry)
    entry = new CSSColorValue(color);
  return entry;
}

}  // namespace cssvalue
}  // namespace blink
