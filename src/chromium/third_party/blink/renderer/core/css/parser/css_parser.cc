// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_parser.cc
// Description: CSSParser Class
//      Author: Ziming Li
//     Created: 2020-03-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/parser/css_parser.h"

#include <memory>
#include "third_party/blink/renderer/core/css/css_selector_list.h"
#include "third_party/blink/renderer/core/css/parser/css_selector_parser.h"
#include "third_party/blink/renderer/core/css/parser/css_tokenizer.h"

namespace blink {

CSSSelectorList CSSParser::ParseSelector(
    std::unique_ptr<CSSParserContext> &context,
    const String& selector) {
  CSSTokenizer tokenizer(selector);
  const auto tokens = tokenizer.TokenizeToEOF();
  return CSSSelectorParser::ParseSelector(CSSParserTokenRange(tokens), context);
}

}  // namespace blink
