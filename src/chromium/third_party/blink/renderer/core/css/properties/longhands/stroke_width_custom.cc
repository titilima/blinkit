// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: stroke_width_custom.cc
// Description: StrokeWidth Class
//      Author: Ziming Li
//     Created: 2020-10-13
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/properties/longhands/stroke_width.h"

#include "third_party/blink/renderer/core/css/parser/css_property_parser_helpers.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {
namespace CSSLonghand {

const CSSValue* StrokeWidth::ParseSingleValue(
    CSSParserTokenRange& range,
    const CSSParserContext&,
    const CSSParserLocalContext&) const {
  return CSSPropertyParserHelpers::ConsumeLengthOrPercent(
      range, kSVGAttributeMode, kValueRangeAll,
      CSSPropertyParserHelpers::UnitlessQuirk::kForbid);
}

const CSSValue* StrokeWidth::CSSValueFromComputedStyleInternal(
    const ComputedStyle& style,
    const SVGComputedStyle& svg_style,
    const LayoutObject*,
    Node* styled_node,
    bool allow_visited_style) const {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  const Length& length = svg_style.StrokeWidth().length();
  if (length.IsFixed()) {
    return CSSPrimitiveValue::Create(length.Value(),
                                     CSSPrimitiveValue::UnitType::kPixels);
  }
  return CSSValue::Create(length, style.EffectiveZoom());
#endif
}

}  // namespace CSSLonghand
}  // namespace blink
