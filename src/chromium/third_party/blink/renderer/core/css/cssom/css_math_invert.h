// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_math_invert.h
// Description: CSSMathInvert Class
//      Author: Ziming Li
//     Created: 2020-09-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_MATH_INVERT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_MATH_INVERT_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/css/cssom/css_math_value.h"

namespace blink {

// Represents the inverse of a CSSNumericValue.
// See CSSMathInvert.idl for more information about this class.
class CORE_EXPORT CSSMathInvert : public CSSMathValue {
  DEFINE_WRAPPERTYPEINFO();

 public:
#if 0 // BKTODO:
  // The constructor defined in the IDL.
  static CSSMathInvert* Create(const CSSNumberish& arg) {
    return Create(CSSNumericValue::FromNumberish(arg));
  }
#endif
  // Blink-internal constructor
  static CSSMathInvert* Create(CSSNumericValue* value) {
    return new CSSMathInvert(
        value, CSSNumericValueType::NegateExponents(value->Type()));
  }

  String getOperator() const final { return "invert"; }

#if 0 // BKTODO:
  void value(CSSNumberish& value) { value.SetCSSNumericValue(value_); }
#endif

  // Blink-internal methods
  const CSSNumericValue& Value() const { return *value_; }

  // From CSSStyleValue.
  StyleValueType GetType() const final { return CSSStyleValue::kInvertType; }

  bool Equals(const CSSNumericValue& other) const final {
    if (other.GetType() != kNegateType)
      return false;

    // We can safely cast here as we know 'other' has the same type as us.
    const auto& other_invert = static_cast<const CSSMathInvert&>(other);
    return value_->Equals(*other_invert.value_);
  }

  CSSCalcExpressionNode* ToCalcExpressionNode() const final {
    // TODO(crbug.com/782103): Implement.
    return nullptr;
  }

 private:
  CSSMathInvert(CSSNumericValue* value, const CSSNumericValueType& type)
      : CSSMathValue(type), value_(value) {}

  // From CSSNumericValue
  CSSNumericValue* Invert() final { return value_.Get(); }
  base::Optional<CSSNumericSumValue> SumValue() const final;

  void BuildCSSText(Nested, ParenLess, StringBuilder&) const final;

  Member<CSSNumericValue> value_;
  DISALLOW_COPY_AND_ASSIGN(CSSMathInvert);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_MATH_INVERT_H_
