// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_color_value.h
// Description: CSSColorValue Class
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_COLOR_VALUE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_COLOR_VALUE_H_

#include "third_party/blink/renderer/core/css/css_value.h"
#include "third_party/blink/renderer/platform/graphics/color.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

class CSSValuePool;

namespace cssvalue {

// Represents the non-keyword subset of <color>.
class CSSColorValue : public CSSValue {
 public:
  // TODO(sashab): Make this create() method take a Color instead.
  static std::shared_ptr<CSSColorValue> Create(RGBA32 color);

  String CustomCSSText() const {
    return color_.SerializedAsCSSComponentValue();
  }

  Color Value() const { return color_; }

  bool Equals(const CSSColorValue& other) const {
    return color_ == other.color_;
  }

 private:
  friend class ::blink::CSSValuePool;

  CSSColorValue(Color color) : CSSValue(kColorClass), color_(color) {}

  Color color_;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSColorValue, IsColorValue());

}  // namespace cssvalue
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_COLOR_VALUE_H_
