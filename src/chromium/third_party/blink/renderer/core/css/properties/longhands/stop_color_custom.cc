// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: stop_color_custom.cc
// Description: StopColor Class
//      Author: Ziming Li
//     Created: 2020-10-13
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/properties/longhands/stop_color.h"

#include "third_party/blink/renderer/core/css/parser/css_parser_context.h"
#include "third_party/blink/renderer/core/css/parser/css_property_parser_helpers.h"
#include "third_party/blink/renderer/core/css/properties/computed_style_utils.h"

namespace blink {
namespace CSSLonghand {

const CSSValue* StopColor::ParseSingleValue(
    CSSParserTokenRange& range,
    const CSSParserContext& context,
    const CSSParserLocalContext&) const {
  return CSSPropertyParserHelpers::ConsumeColor(range, context.Mode());
}

const blink::Color StopColor::ColorIncludingFallback(
    bool visited_link,
    const ComputedStyle& style) const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  StyleColor result = style.StopColor();
  if (!result.IsCurrentColor())
    return result.GetColor();
  return visited_link ? style.VisitedLinkColor() : style.GetColor();
#endif
}

const CSSValue* StopColor::CSSValueFromComputedStyleInternal(
    const ComputedStyle& style,
    const SVGComputedStyle&,
    const LayoutObject*,
    Node*,
    bool allow_visited_style) const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return ComputedStyleUtils::CurrentColorOrValidColor(style, style.StopColor());
#endif
}

}  // namespace CSSLonghand
}  // namespace blink
