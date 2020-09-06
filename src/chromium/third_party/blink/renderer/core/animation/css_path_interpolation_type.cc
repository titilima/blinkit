// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_path_interpolation_type.cc
// Description: CSSPathInterpolationType Class
//      Author: Ziming Li
//     Created: 2020-09-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/animation/css_path_interpolation_type.h"

#include <memory>
#include <utility>

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/animation/path_interpolation_functions.h"
#include "third_party/blink/renderer/core/css/css_path_value.h"
#include "third_party/blink/renderer/core/css/resolver/style_resolver_state.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {

namespace {

// Returns the property's path() value.
// If the property's value is not a path(), returns nullptr.
StylePath* GetPath(const CSSProperty& property, const ComputedStyle& style) {
  ASSERT(false); // BKTODO: Check if necessary.
  return nullptr;
#if 0
  switch (property.PropertyID()) {
    case CSSPropertyD:
      return style.SvgStyle().D();
    case CSSPropertyOffsetPath: {
      BasicShape* offset_path = style.OffsetPath();
      if (!offset_path || offset_path->GetType() != BasicShape::kStylePathType)
        return nullptr;
      return ToStylePath(style.OffsetPath());
    }
    default:
      NOTREACHED();
      return nullptr;
  }
#endif
}

// Set the property to the given path() value.
void SetPath(const CSSProperty& property,
             ComputedStyle& style,
             scoped_refptr<blink::StylePath> path) {
  switch (property.PropertyID()) {
    case CSSPropertyD:
      ASSERT(false); // BKTODO: style.SetD(std::move(path));
      return;
    case CSSPropertyOffsetPath:
      style.SetOffsetPath(std::move(path));
      return;
    default:
      NOTREACHED();
      return;
  }
}

}  // namespace

void CSSPathInterpolationType::ApplyStandardPropertyValue(
    const InterpolableValue& interpolable_value,
    const NonInterpolableValue* non_interpolable_value,
    StyleResolverState& state) const {
  ASSERT(false); // BKTODO:
#if 0
  std::unique_ptr<SVGPathByteStream> path_byte_stream =
      PathInterpolationFunctions::AppliedValue(interpolable_value,
                                               non_interpolable_value);
  if (path_byte_stream->IsEmpty()) {
    SetPath(CssProperty(), *state.Style(), nullptr);
    return;
  }
  SetPath(CssProperty(), *state.Style(),
          StylePath::Create(std::move(path_byte_stream)));
#endif
}

void CSSPathInterpolationType::Composite(
    UnderlyingValueOwner& underlying_value_owner,
    double underlying_fraction,
    const InterpolationValue& value,
    double interpolation_fraction) const {
  PathInterpolationFunctions::Composite(underlying_value_owner,
                                        underlying_fraction, *this, value);
}

InterpolationValue CSSPathInterpolationType::MaybeConvertNeutral(
    const InterpolationValue& underlying,
    ConversionCheckers& conversion_checkers) const {
  return PathInterpolationFunctions::MaybeConvertNeutral(underlying,
                                                         conversion_checkers);
}

InterpolationValue CSSPathInterpolationType::MaybeConvertInitial(
    const StyleResolverState&,
    ConversionCheckers&) const {
  return PathInterpolationFunctions::ConvertValue(
      nullptr, PathInterpolationFunctions::ForceAbsolute);
}

class InheritedPathChecker : public CSSInterpolationType::CSSConversionChecker {
 public:
  static std::unique_ptr<InheritedPathChecker> Create(
      const CSSProperty& property,
      scoped_refptr<StylePath> style_path) {
    return base::WrapUnique(
        new InheritedPathChecker(property, std::move(style_path)));
  }

 private:
  InheritedPathChecker(const CSSProperty& property,
                       scoped_refptr<StylePath> style_path)
      : property_(property), style_path_(std::move(style_path)) {}

  bool IsValid(const StyleResolverState& state,
               const InterpolationValue& underlying) const final {
    return GetPath(property_, *state.ParentStyle()) == style_path_.get();
  }

  const CSSProperty& property_;
  const scoped_refptr<StylePath> style_path_;
};

InterpolationValue CSSPathInterpolationType::MaybeConvertInherit(
    const StyleResolverState& state,
    ConversionCheckers& conversion_checkers) const {
  if (!state.ParentStyle())
    return nullptr;

  conversion_checkers.push_back(InheritedPathChecker::Create(
      CssProperty(), GetPath(CssProperty(), *state.ParentStyle())));
  return PathInterpolationFunctions::ConvertValue(
      GetPath(CssProperty(), *state.ParentStyle()),
      PathInterpolationFunctions::ForceAbsolute);
}

InterpolationValue CSSPathInterpolationType::MaybeConvertValue(
    const CSSValue& value,
    const StyleResolverState*,
    ConversionCheckers& conversion_checkers) const {
  if (!value.IsPathValue()) {
    return nullptr;
  }
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return PathInterpolationFunctions::ConvertValue(
      cssvalue::ToCSSPathValue(value).ByteStream(),
      PathInterpolationFunctions::ForceAbsolute);
#endif
}

InterpolationValue
CSSPathInterpolationType::MaybeConvertStandardPropertyUnderlyingValue(
    const ComputedStyle& style) const {
  return PathInterpolationFunctions::ConvertValue(
      GetPath(CssProperty(), style), PathInterpolationFunctions::ForceAbsolute);
}

PairwiseInterpolationValue CSSPathInterpolationType::MaybeMergeSingles(
    InterpolationValue&& start,
    InterpolationValue&& end) const {
  return PathInterpolationFunctions::MaybeMergeSingles(std::move(start),
                                                       std::move(end));
}

}  // namespace blink
