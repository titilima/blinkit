// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/properties/longhands/margin_block_start.h"

#include "third_party/blink/renderer/core/css/parser/css_parser_context.h"
#include "third_party/blink/renderer/core/css/parser/css_property_parser_helpers.h"
#include "third_party/blink/renderer/core/css/properties/css_parsing_utils.h"
#include "third_party/blink/renderer/core/style_property_shorthand.h"

namespace blink {
namespace CSSLonghand {

const CSSValue* MarginBlockStart::ParseSingleValue(
    CSSParserTokenRange& range,
    const CSSParserContext& context,
    const CSSParserLocalContext&) const {
  return CSSParsingUtils::ConsumeMarginOrOffset(
      range, context.Mode(), CSSPropertyParserHelpers::UnitlessQuirk::kForbid);
}

}  // namespace CSSLonghand
}  // namespace blink
