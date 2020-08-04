// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_parser.h
// Description: CSSParser Class
//      Author: Ziming Li
//     Created: 2020-03-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PARSER_CSS_PARSER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PARSER_CSS_PARSER_H_

#include <memory>
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/css/parser/css_parser_context.h"

namespace blink {

class CSSSelectorList;

// This class serves as the public API for the css/parser subsystem
class CORE_EXPORT CSSParser {
  STATIC_ONLY(CSSParser);

 public:
  static CSSSelectorList ParseSelector(std::unique_ptr<CSSParserContext> &, const String&);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PARSER_CSS_PARSER_H_
