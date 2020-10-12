// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/properties/longhands/opacity.h"

#include "third_party/blink/renderer/core/css/parser/css_property_parser_helpers.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {
namespace CSSLonghand {

const CSSValue* Opacity::ParseSingleValue(CSSParserTokenRange& range,
                                          const CSSParserContext& context,
                                          const CSSParserLocalContext&) const {
  return CSSPropertyParserHelpers::ConsumeNumber(range, kValueRangeAll);
}

const CSSValue* Opacity::CSSValueFromComputedStyleInternal(
    const ComputedStyle& style,
    const SVGComputedStyle&,
    const LayoutObject*,
    Node*,
    bool allow_visited_style) const {
  return CSSPrimitiveValue::Create(style.Opacity(),
                                   CSSPrimitiveValue::UnitType::kNumber);
}

}  // namespace CSSLonghand
}  // namespace blink
