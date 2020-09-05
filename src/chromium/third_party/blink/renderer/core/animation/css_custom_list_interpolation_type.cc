// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_custom_list_interpolation_type.cc
// Description: CSSCustomListInterpolationType Class
//      Author: Ziming Li
//     Created: 2020-09-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/animation/css_custom_list_interpolation_type.h"

#include "third_party/blink/renderer/core/animation/length_interpolation_functions.h"
#include "third_party/blink/renderer/core/animation/underlying_length_checker.h"
#include "third_party/blink/renderer/core/css/css_primitive_value.h"
#include "third_party/blink/renderer/core/css/css_value_list.h"

namespace blink {

InterpolationValue CSSCustomListInterpolationType::MaybeConvertNeutral(
    const InterpolationValue& underlying,
    ConversionCheckers& conversion_checkers) const {
  size_t underlying_length =
      UnderlyingLengthChecker::GetUnderlyingLength(underlying);
  conversion_checkers.push_back(
      UnderlyingLengthChecker::Create(underlying_length));

  if (underlying_length == 0)
    return nullptr;

  InterpolationValue null_underlying(nullptr);
  ConversionCheckers null_checkers;

  auto convert_inner = [this, &null_underlying, &null_checkers](size_t) {
    return this->inner_interpolation_type_->MaybeConvertNeutral(null_underlying,
                                                                null_checkers);
  };

  return ListInterpolationFunctions::CreateList(underlying_length,
                                                convert_inner);
}

InterpolationValue CSSCustomListInterpolationType::MaybeConvertValue(
    const CSSValue& value,
    const StyleResolverState* state,
    ConversionCheckers&) const {
  if (!value.IsValueList())
    return nullptr;

  ConversionCheckers null_checkers;

  const CSSValueList& list = ToCSSValueList(value);
  return ListInterpolationFunctions::CreateList(
      list.length(), [this, &list, state, &null_checkers](size_t index) {
        return this->inner_interpolation_type_->MaybeConvertValue(
            list.Item(index), state, null_checkers);
      });
}

const CSSValue* CSSCustomListInterpolationType::CreateCSSValue(
    const InterpolableValue& interpolable_value,
    const NonInterpolableValue* non_interpolable_value,
    const StyleResolverState& state) const {
  const InterpolableList& interpolable_list =
      ToInterpolableList(interpolable_value);
  const NonInterpolableList* non_interpolable_list =
      non_interpolable_value ? &ToNonInterpolableList(*non_interpolable_value)
                             : nullptr;

  CSSValueList* list = nullptr;

  switch (syntax_repeat_) {
    default:
      NOTREACHED();
      FALLTHROUGH;
    case CSSSyntaxRepeat::kSpaceSeparated:
      list = CSSValueList::CreateSpaceSeparated();
      break;
    case CSSSyntaxRepeat::kCommaSeparated:
      list = CSSValueList::CreateCommaSeparated();
      break;
  }

  DCHECK(!non_interpolable_list ||
         interpolable_list.length() == non_interpolable_list->length());

  for (size_t i = 0; i < interpolable_list.length(); ++i) {
    const NonInterpolableValue* non_interpolable_single_value =
        non_interpolable_list ? non_interpolable_list->Get(i) : nullptr;
    list->Append(*inner_interpolation_type_->CreateCSSValue(
        *interpolable_list.Get(i), non_interpolable_single_value, state));
  }

  return list;
}

void CSSCustomListInterpolationType::Composite(
    UnderlyingValueOwner& underlying_value_owner,
    double underlying_fraction,
    const InterpolationValue& value,
    double interpolation_fraction) const {
  // TODO(andruud): Properly support composition once behavior is defined.
  // https://github.com/w3c/css-houdini-drafts/issues/799
  underlying_value_owner.Set(*this, value);
}

PairwiseInterpolationValue CSSCustomListInterpolationType::MaybeMergeSingles(
    InterpolationValue&& start,
    InterpolationValue&& end) const {
  ASSERT(false); // BKTODO:
  return PairwiseInterpolationValue(nullptr);
#if 0
  return ListInterpolationFunctions::MaybeMergeSingles(
      std::move(start), std::move(end),
      ListInterpolationFunctions::LengthMatchingStrategy::kEqual,
      WTF::BindRepeating(&CSSInterpolationType::MaybeMergeSingles,
                         WTF::Unretained(inner_interpolation_type_.get())));
#endif
}

}  // namespace blink
