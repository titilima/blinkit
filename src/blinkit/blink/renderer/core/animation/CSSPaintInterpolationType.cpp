// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ComputedStyle.cpp
// Description: ComputedStyle Class
//      Author: Ziming Li
//     Created: 2021-08-13
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/CSSPaintInterpolationType.h"

#include "core/animation/CSSColorInterpolationType.h"
// BKTODO: #include "core/animation/PaintPropertyFunctions.h"
#include "core/css/resolver/StyleResolverState.h"

namespace blink {

PassOwnPtr<InterpolationValue> CSSPaintInterpolationType::maybeConvertNeutral(const UnderlyingValue&, ConversionCheckers&) const
{
    return InterpolationValue::create(*this, CSSColorInterpolationType::createInterpolableColor(Color::transparent));
}

PassOwnPtr<InterpolationValue> CSSPaintInterpolationType::maybeConvertInitial() const
{
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    StyleColor initialColor;
    if (!PaintPropertyFunctions::getInitialColor(cssProperty(), initialColor))
        return nullptr;
    return InterpolationValue::create(*this, CSSColorInterpolationType::createInterpolableColor(initialColor));
#endif
}

class ParentPaintChecker : public InterpolationType::ConversionChecker {
public:
    static PassOwnPtr<ParentPaintChecker> create(const InterpolationType& type, CSSPropertyID property, const StyleColor& color)
    {
        return adoptPtr(new ParentPaintChecker(type, property, color));
    }
    static PassOwnPtr<ParentPaintChecker> create(const InterpolationType& type, CSSPropertyID property)
    {
        return adoptPtr(new ParentPaintChecker(type, property));
    }

private:
    ParentPaintChecker(const InterpolationType& type, CSSPropertyID property)
        : ConversionChecker(type)
        , m_property(property)
        , m_validColor(false)
    { }
    ParentPaintChecker(const InterpolationType& type, CSSPropertyID property, const StyleColor& color)
        : ConversionChecker(type)
        , m_property(property)
        , m_validColor(true)
        , m_color(color)
    { }

    bool isValid(const InterpolationEnvironment& environment, const UnderlyingValue&) const final
    {
        ASSERT(false); // BKTODO:
        return false;
#if 0
        StyleColor parentColor;
        if (!PaintPropertyFunctions::getColor(m_property, *environment.state().parentStyle(), parentColor))
            return !m_validColor;
        return m_validColor && parentColor == m_color;
#endif
    }

    DEFINE_INLINE_VIRTUAL_TRACE()
    {
        ConversionChecker::trace(visitor);
    }

    const CSSPropertyID m_property;
    const bool m_validColor;
    const StyleColor m_color;
};

PassOwnPtr<InterpolationValue> CSSPaintInterpolationType::maybeConvertInherit(const StyleResolverState& state, ConversionCheckers& conversionCheckers) const
{
    if (!state.parentStyle())
        return nullptr;
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    StyleColor parentColor;
    if (!PaintPropertyFunctions::getColor(cssProperty(), *state.parentStyle(), parentColor)) {
        conversionCheckers.append(ParentPaintChecker::create(*this, cssProperty()));
        return nullptr;
    }
    conversionCheckers.append(ParentPaintChecker::create(*this, cssProperty(), parentColor));
    return InterpolationValue::create(*this, CSSColorInterpolationType::createInterpolableColor(parentColor));
#endif
}

PassOwnPtr<InterpolationValue> CSSPaintInterpolationType::maybeConvertValue(const CSSValue& value, const StyleResolverState&, ConversionCheckers&) const
{
    OwnPtr<InterpolableValue> interpolableColor = CSSColorInterpolationType::maybeCreateInterpolableColor(value);
    if (!interpolableColor)
        return nullptr;
    return InterpolationValue::create(*this, interpolableColor.release());
}

PassOwnPtr<InterpolationValue> CSSPaintInterpolationType::maybeConvertUnderlyingValue(const InterpolationEnvironment& environment) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    // TODO(alancutter): Support capturing and animating with the visited paint color.
    StyleColor underlyingColor;
    if (!PaintPropertyFunctions::getColor(cssProperty(), *environment.state().style(), underlyingColor))
        return nullptr;
    return InterpolationValue::create(*this, CSSColorInterpolationType::createInterpolableColor(underlyingColor));
#endif
}

void CSSPaintInterpolationType::apply(const InterpolableValue& interpolableColor, const NonInterpolableValue*, InterpolationEnvironment& environment) const
{
    ASSERT(false); // BKTODO: PaintPropertyFunctions::setColor(cssProperty(), *environment.state().style(), CSSColorInterpolationType::resolveInterpolableColor(interpolableColor, environment.state()));
}

} // namespace blink
