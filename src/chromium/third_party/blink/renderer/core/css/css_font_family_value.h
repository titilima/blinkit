// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_font_family_value.h
// Description: CSSFontFamilyValue Class
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_FONT_FAMILY_VALUE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_FONT_FAMILY_VALUE_H_

#include "third_party/blink/renderer/core/css/css_value.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

class CORE_EXPORT CSSFontFamilyValue : public CSSValue {
 public:
  static std::shared_ptr<CSSFontFamilyValue> Create(const String& family_name);

  String Value() const { return string_; }

  String CustomCSSText() const;

  bool Equals(const CSSFontFamilyValue& other) const {
    return string_ == other.string_;
  }

 private:
  friend class CSSValuePool;

  CSSFontFamilyValue(const String&);

  // TODO(sashab): Change this to an AtomicString.
  String string_;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSFontFamilyValue, IsFontFamilyValue());

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_FONT_FAMILY_VALUE_H_
