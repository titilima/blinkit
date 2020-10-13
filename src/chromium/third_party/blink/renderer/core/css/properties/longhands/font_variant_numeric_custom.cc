// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: font_variant_numeric_custom.cc
// Description: FontVariantNumeric Class
//      Author: Ziming Li
//     Created: 2020-10-13
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/properties/longhands/font_variant_numeric.h"

#include "third_party/blink/renderer/core/css/parser/css_property_parser_helpers.h"
// BKTODO: #include "third_party/blink/renderer/core/css/parser/font_variant_numeric_parser.h"
#include "third_party/blink/renderer/core/css/properties/computed_style_utils.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {
namespace CSSLonghand {

const CSSValue* FontVariantNumeric::ParseSingleValue(
    CSSParserTokenRange& range,
    const CSSParserContext& context,
    const CSSParserLocalContext&) const {
  if (range.Peek().Id() == CSSValueNormal)
    return CSSPropertyParserHelpers::ConsumeIdent(range);

  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  FontVariantNumericParser numeric_parser;
  do {
    if (numeric_parser.ConsumeNumeric(range) !=
        FontVariantNumericParser::ParseResult::kConsumedValue)
      return nullptr;
  } while (!range.AtEnd());

  return numeric_parser.FinalizeValue();
#endif
}

const CSSValue* FontVariantNumeric::CSSValueFromComputedStyleInternal(
    const ComputedStyle& style,
    const SVGComputedStyle&,
    const LayoutObject*,
    Node* styled_node,
    bool allow_visited_style) const {
  return ComputedStyleUtils::ValueForFontVariantNumeric(style);
}

}  // namespace CSSLonghand
}  // namespace blink
