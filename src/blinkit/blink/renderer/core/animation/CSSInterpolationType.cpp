// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSInterpolationType.cpp
// Description: CSSInterpolationType Class
//      Author: Ziming Li
//     Created: 2021-10-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/CSSInterpolationType.h"

#include "core/StylePropertyShorthand.h"
#include "core/animation/StringKeyframe.h"
#include "core/css/CSSVariableReferenceValue.h"
#include "core/css/resolver/CSSVariableResolver.h"
#include "core/css/resolver/StyleResolverState.h"
#include "platform/RuntimeEnabledFeatures.h"

namespace blink {

class ResolvedVariableChecker : public InterpolationType::ConversionChecker {
public:
    static PassOwnPtr<ResolvedVariableChecker> create(const InterpolationType& type, CSSPropertyID property, CSSVariableReferenceValue *variableReference, const GCRefPtr<CSSValue> &resolvedValue)
    {
        return adoptPtr(new ResolvedVariableChecker(type, property, variableReference, resolvedValue));
    }

private:
    ResolvedVariableChecker(const InterpolationType& type, CSSPropertyID property, CSSVariableReferenceValue *variableReference, const GCRefPtr<CSSValue> &resolvedValue)
        : ConversionChecker(type)
        , m_property(property)
        , m_variableReference(variableReference)
        , m_resolvedValue(resolvedValue)
    { }

    bool isValid(const InterpolationEnvironment& environment, const UnderlyingValue&) const final
    {
        // TODO(alancutter): Just check the variables referenced instead of doing a full CSSValue resolve.
        GCRefPtr<CSSValue> resolvedValue = CSSVariableResolver::resolveVariableReferences(environment.state().style()->variables(), m_property, *m_variableReference);
        return m_resolvedValue->equals(*resolvedValue);
    }

    CSSPropertyID m_property;
    GCRefPtr<CSSVariableReferenceValue> m_variableReference;
    GCRefPtr<CSSValue> m_resolvedValue;
};

PassOwnPtr<InterpolationValue> CSSInterpolationType::maybeConvertSingle(const PropertySpecificKeyframe& keyframe, const InterpolationEnvironment& environment, const UnderlyingValue& underlyingValue, ConversionCheckers& conversionCheckers) const
{
    GCRefPtr<CSSValue> resolvedCSSValueOwner;
    const CSSValue* value = toCSSPropertySpecificKeyframe(keyframe).value();

    if (!value)
        return maybeConvertNeutral(underlyingValue, conversionCheckers);

    if (value->isVariableReferenceValue() && !isShorthandProperty(cssProperty())) {
        resolvedCSSValueOwner = CSSVariableResolver::resolveVariableReferences(environment.state().style()->variables(), cssProperty(), toCSSVariableReferenceValue(*value));
        conversionCheckers.append(ResolvedVariableChecker::create(*this, cssProperty(), toCSSVariableReferenceValue(const_cast<CSSValue*>(value)), resolvedCSSValueOwner));
        value = resolvedCSSValueOwner.get();
    }

    if (value->isInitialValue() || (value->isUnsetValue() && !CSSPropertyMetadata::isInheritedProperty(cssProperty())))
        return maybeConvertInitial();

    if (value->isInheritedValue() || (value->isUnsetValue() && CSSPropertyMetadata::isInheritedProperty(cssProperty())))
        return maybeConvertInherit(environment.state(), conversionCheckers);

    return maybeConvertValue(*value, environment.state(), conversionCheckers);
}

} // namespace blink
