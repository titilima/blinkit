// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/properties/shorthands/border_block.h"

#include "third_party/blink/renderer/core/css/parser/css_property_parser_helpers.h"
#include "third_party/blink/renderer/core/style_property_shorthand.h"

namespace blink {
namespace CSSShorthand {

bool BorderBlock::ParseShorthand(
    bool important,
    CSSParserTokenRange& range,
    const CSSParserContext& context,
    const CSSParserLocalContext&,
    HeapVector<CSSPropertyValue, 256>& properties) const {
  const CSSValue* width = nullptr;
  const CSSValue* style = nullptr;
  const CSSValue* color = nullptr;

  if (!CSSPropertyParserHelpers::ConsumeBorderShorthand(range, context, width,
                                                        style, color)) {
    return false;
  };

  CSSPropertyParserHelpers::AddExpandedPropertyForValue(
      CSSPropertyBorderBlockWidth, *width, important, properties);
  CSSPropertyParserHelpers::AddExpandedPropertyForValue(
      CSSPropertyBorderBlockStyle, *style, important, properties);
  CSSPropertyParserHelpers::AddExpandedPropertyForValue(
      CSSPropertyBorderBlockColor, *color, important, properties);

  return range.AtEnd();
}

const CSSValue* BorderBlock::CSSValueFromComputedStyleInternal(
    const ComputedStyle& style,
    const SVGComputedStyle&,
    const LayoutObject* layout_object,
    Node* styled_node,
    bool allow_visited_style) const {
  const CSSValue* value_start =
      GetCSSPropertyBorderBlockStart().CSSValueFromComputedStyle(
          style, layout_object, styled_node, allow_visited_style);
  const CSSValue* value_end =
      GetCSSPropertyBorderBlockEnd().CSSValueFromComputedStyle(
          style, layout_object, styled_node, allow_visited_style);
  if (!DataEquivalent(value_start, value_end)) {
    return nullptr;
  }
  return value_start;
}

}  // namespace CSSShorthand
}  // namespace blink
