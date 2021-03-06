// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_paint_interpolation_type.cc
// Description: CSSPaintInterpolationType Class
//      Author: Ziming Li
//     Created: 2020-09-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/animation/css_paint_interpolation_type.h"

#include <memory>
#include <utility>

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/animation/css_color_interpolation_type.h"
#include "third_party/blink/renderer/core/css/resolver/style_resolver_state.h"
#include "third_party/blink/renderer/core/css/style_color.h"
#include "third_party/blink/renderer/core/css_property_names.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {

InterpolationValue CSSPaintInterpolationType::MaybeConvertNeutral(
    const InterpolationValue&,
    ConversionCheckers&) const {
  return InterpolationValue(
      CSSColorInterpolationType::CreateInterpolableColor(Color::kTransparent));
}

InterpolationValue CSSPaintInterpolationType::MaybeConvertInitial(
    const StyleResolverState&,
    ConversionCheckers& conversion_checkers) const {
  ASSERT(false); // BKTODO: Check if necessary.
  return nullptr;
#if 0
  StyleColor initial_color;
  if (!GetColor(CssProperty(), ComputedStyle::InitialStyle(), initial_color))
    return nullptr;
  return InterpolationValue(
      CSSColorInterpolationType::CreateInterpolableColor(initial_color));
#endif
}

class InheritedPaintChecker
    : public CSSInterpolationType::CSSConversionChecker {
 public:
  static std::unique_ptr<InheritedPaintChecker> Create(
      const CSSProperty& property,
      const StyleColor& color) {
    return base::WrapUnique(new InheritedPaintChecker(property, color));
  }
  static std::unique_ptr<InheritedPaintChecker> Create(
      const CSSProperty& property) {
    return base::WrapUnique(new InheritedPaintChecker(property));
  }

 private:
  InheritedPaintChecker(const CSSProperty& property)
      : property_(property), valid_color_(false) {}
  InheritedPaintChecker(const CSSProperty& property, const StyleColor& color)
      : property_(property), valid_color_(true), color_(color) {}

  bool IsValid(const StyleResolverState& state,
               const InterpolationValue& underlying) const final {
    ASSERT(false); // BKTODO: Check if necessary.
    return !valid_color_;
#if 0
    StyleColor parent_color;
    if (!GetColor(property_, *state.ParentStyle(), parent_color))
      return !valid_color_;
    return valid_color_ && parent_color == color_;
#endif
  }

  const CSSProperty& property_;
  const bool valid_color_;
  const StyleColor color_;
};

InterpolationValue CSSPaintInterpolationType::MaybeConvertInherit(
    const StyleResolverState& state,
    ConversionCheckers& conversion_checkers) const {
  if (!state.ParentStyle())
    return nullptr;
  ASSERT(false); // BKTODO: Check if necessary.
  conversion_checkers.push_back(InheritedPaintChecker::Create(CssProperty()));
  return nullptr;
#if 0
  StyleColor parent_color;
  if (!GetColor(CssProperty(), *state.ParentStyle(), parent_color)) {
    conversion_checkers.push_back(InheritedPaintChecker::Create(CssProperty()));
    return nullptr;
  }
  conversion_checkers.push_back(
      InheritedPaintChecker::Create(CssProperty(), parent_color));
  return InterpolationValue(
      CSSColorInterpolationType::CreateInterpolableColor(parent_color));
#endif
}

InterpolationValue CSSPaintInterpolationType::MaybeConvertValue(
    const CSSValue& value,
    const StyleResolverState*,
    ConversionCheckers&) const {
  std::unique_ptr<InterpolableValue> interpolable_color =
      CSSColorInterpolationType::MaybeCreateInterpolableColor(value);
  if (!interpolable_color)
    return nullptr;
  return InterpolationValue(std::move(interpolable_color));
}

InterpolationValue
CSSPaintInterpolationType::MaybeConvertStandardPropertyUnderlyingValue(
    const ComputedStyle& style) const {
  ASSERT(false); // BKTODO: Check if necessary.
  return nullptr;
#if 0
  // TODO(alancutter): Support capturing and animating with the visited paint
  // color.
  StyleColor underlying_color;
  if (!GetColor(CssProperty(), style, underlying_color))
    return nullptr;
  return InterpolationValue(
      CSSColorInterpolationType::CreateInterpolableColor(underlying_color));
#endif
}

void CSSPaintInterpolationType::ApplyStandardPropertyValue(
    const InterpolableValue& interpolable_color,
    const NonInterpolableValue*,
    StyleResolverState& state) const {
  ASSERT(false); // BKTODO: Check if necessary.
#if 0
  Color color = CSSColorInterpolationType::ResolveInterpolableColor(
      interpolable_color, state);
  SVGComputedStyle& mutable_svg_style = state.Style()->AccessSVGStyle();
  switch (CssProperty().PropertyID()) {
    case CSSPropertyFill:
      mutable_svg_style.SetFillPaint(SVGPaint(color));
      mutable_svg_style.SetVisitedLinkFillPaint(SVGPaint(color));
      break;
    case CSSPropertyStroke:
      mutable_svg_style.SetStrokePaint(SVGPaint(color));
      mutable_svg_style.SetVisitedLinkStrokePaint(SVGPaint(color));
      break;
    default:
      NOTREACHED();
  }
#endif
}

}  // namespace blink
