// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: border_left_color_custom.cc
// Description: BorderLeftColor Class
//      Author: Ziming Li
//     Created: 2020-10-12
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/properties/longhands/border_left_color.h"

#include "third_party/blink/renderer/core/css/css_color_value.h"
#include "third_party/blink/renderer/core/css/properties/computed_style_utils.h"
#include "third_party/blink/renderer/core/css/properties/css_parsing_utils.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {

class CSSParserContext;
class CSSParserLocalContext;
class CSSParserTokenRange;

namespace CSSLonghand {

const CSSValue* BorderLeftColor::ParseSingleValue(
    CSSParserTokenRange& range,
    const CSSParserContext& context,
    const CSSParserLocalContext& local_context) const {
  return CSSParsingUtils::ConsumeBorderColorSide(range, context, local_context);
}

const blink::Color BorderLeftColor::ColorIncludingFallback(
    bool visited_link,
    const ComputedStyle& style) const {
  StyleColor result = visited_link ? style.VisitedLinkBorderLeftColor()
                                   : style.BorderLeftColor();
  EBorderStyle border_style = style.BorderLeftStyle();
  return ComputedStyleUtils::BorderSideColor(style, result, border_style,
                                             visited_link);
}

const CSSValue* BorderLeftColor::CSSValueFromComputedStyleInternal(
    const ComputedStyle& style,
    const SVGComputedStyle&,
    const LayoutObject*,
    Node*,
    bool allow_visited_style) const {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return allow_visited_style ? cssvalue::CSSColorValue::Create(
                                   style.VisitedDependentColor(*this).Rgb())
                             : ComputedStyleUtils::CurrentColorOrValidColor(
                                   style, style.BorderLeftColor());
#endif
}

}  // namespace CSSLonghand
}  // namespace blink