// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/CSSVisibilityInterpolationType.h"

#include "core/css/CSSPrimitiveValueMappings.h"
#include "core/css/resolver/StyleResolverState.h"

namespace blink {

class CSSVisibilityNonInterpolableValue : public NonInterpolableValue {
public:
    ~CSSVisibilityNonInterpolableValue() final { }

    static PassRefPtr<CSSVisibilityNonInterpolableValue> create(EVisibility start, EVisibility end)
    {
        return adoptRef(new CSSVisibilityNonInterpolableValue(start, end));
    }

    EVisibility visibility() const
    {
        ASSERT(m_isSingle);
        return m_start;
    }

    EVisibility visibility(double fraction) const
    {
        if (m_isSingle || fraction <= 0)
            return m_start;
        if (fraction >= 1)
            return m_end;
        if (m_start == VISIBLE || m_end == VISIBLE)
            return VISIBLE;
        return fraction < 0.5 ? m_start : m_end;
    }

    DECLARE_NON_INTERPOLABLE_VALUE_TYPE();

private:
    CSSVisibilityNonInterpolableValue(EVisibility start, EVisibility end)
        : m_start(start)
        , m_end(end)
        , m_isSingle(m_start == m_end)
    { }

    const EVisibility m_start;
    const EVisibility m_end;
    const bool m_isSingle;
};

DEFINE_NON_INTERPOLABLE_VALUE_TYPE(CSSVisibilityNonInterpolableValue);
DEFINE_NON_INTERPOLABLE_VALUE_TYPE_CASTS(CSSVisibilityNonInterpolableValue);

class UnderlyingVisibilityChecker : public InterpolationType::ConversionChecker {
public:
    ~UnderlyingVisibilityChecker() final {}

    static PassOwnPtr<UnderlyingVisibilityChecker> create(const InterpolationType& type, EVisibility visibility)
    {
        return adoptPtr(new UnderlyingVisibilityChecker(type, visibility));
    }

private:
    UnderlyingVisibilityChecker(const InterpolationType& type, EVisibility visibility)
        : ConversionChecker(type)
        , m_visibility(visibility)
    { }

    bool isValid(const InterpolationEnvironment&, const UnderlyingValue& underlyingValue) const final
    {
        double underlyingFraction = toInterpolableNumber(underlyingValue->interpolableValue()).value();
        EVisibility underlyingVisibility = toCSSVisibilityNonInterpolableValue(underlyingValue->nonInterpolableValue())->visibility(underlyingFraction);
        return m_visibility == underlyingVisibility;
    }

    const EVisibility m_visibility;
};

class ParentVisibilityChecker : public InterpolationType::ConversionChecker {
public:
    static PassOwnPtr<ParentVisibilityChecker> create(const InterpolationType& type, EVisibility visibility)
    {
        return adoptPtr(new ParentVisibilityChecker(type, visibility));
    }

private:
    ParentVisibilityChecker(const InterpolationType& type, EVisibility visibility)
        : ConversionChecker(type)
        , m_visibility(visibility)
    { }

    bool isValid(const InterpolationEnvironment& environment, const UnderlyingValue&) const final
    {
        return m_visibility == environment.state().parentStyle()->visibility();
    }

    DEFINE_INLINE_VIRTUAL_TRACE() { ConversionChecker::trace(visitor); }

    const double m_visibility;
};

PassOwnPtr<InterpolationValue> CSSVisibilityInterpolationType::createVisibilityValue(EVisibility visibility) const
{
    return InterpolationValue::create(*this, InterpolableNumber::create(0), CSSVisibilityNonInterpolableValue::create(visibility, visibility));
}

PassOwnPtr<InterpolationValue> CSSVisibilityInterpolationType::maybeConvertNeutral(const UnderlyingValue& underlyingValue, ConversionCheckers& conversionCheckers) const
{
    double underlyingFraction = toInterpolableNumber(underlyingValue->interpolableValue()).value();
    EVisibility underlyingVisibility = toCSSVisibilityNonInterpolableValue(underlyingValue->nonInterpolableValue())->visibility(underlyingFraction);
    conversionCheckers.append(UnderlyingVisibilityChecker::create(*this, underlyingVisibility));
    return createVisibilityValue(underlyingVisibility);
}

PassOwnPtr<InterpolationValue> CSSVisibilityInterpolationType::maybeConvertInitial() const
{
    return createVisibilityValue(VISIBLE);
}

PassOwnPtr<InterpolationValue> CSSVisibilityInterpolationType::maybeConvertInherit(const StyleResolverState& state, ConversionCheckers& conversionCheckers) const
{
    if (!state.parentStyle())
        return nullptr;
    EVisibility inheritedVisibility = state.parentStyle()->visibility();
    conversionCheckers.append(ParentVisibilityChecker::create(*this, inheritedVisibility));
    return createVisibilityValue(inheritedVisibility);
}

PassOwnPtr<InterpolationValue> CSSVisibilityInterpolationType::maybeConvertValue(const CSSValue& value, const StyleResolverState& state, ConversionCheckers& conversionCheckers) const
{
    if (!value.isPrimitiveValue())
        return nullptr;

    const CSSPrimitiveValue& primitiveValue = toCSSPrimitiveValue(value);
    CSSValueID keyword = primitiveValue.getValueID();

    switch (keyword) {
    case CSSValueHidden:
    case CSSValueVisible:
    case CSSValueCollapse:
        return createVisibilityValue(primitiveValue.convertTo<EVisibility>());
    default:
        return nullptr;
    }
}

PassOwnPtr<InterpolationValue> CSSVisibilityInterpolationType::maybeConvertUnderlyingValue(const InterpolationEnvironment& environment) const
{
    return createVisibilityValue(environment.state().style()->visibility());
}

PassOwnPtr<PairwisePrimitiveInterpolation> CSSVisibilityInterpolationType::mergeSingleConversions(InterpolationValue& startValue, InterpolationValue& endValue) const
{
    return PairwisePrimitiveInterpolation::create(
        *this,
        InterpolableNumber::create(0),
        InterpolableNumber::create(1),
        CSSVisibilityNonInterpolableValue::create(
            toCSSVisibilityNonInterpolableValue(startValue.nonInterpolableValue())->visibility(),
            toCSSVisibilityNonInterpolableValue(endValue.nonInterpolableValue())->visibility()));
}

void CSSVisibilityInterpolationType::composite(UnderlyingValue& underlyingValue, double underlyingFraction, const InterpolationValue& value) const
{
    underlyingValue.set(&value);
}

void CSSVisibilityInterpolationType::apply(const InterpolableValue& interpolableValue, const NonInterpolableValue* nonInterpolableValue, InterpolationEnvironment& environment) const
{
    // Visibility interpolation has been deferred to application time here due to its non-linear behaviour.
    double fraction = toInterpolableNumber(interpolableValue).value();
    EVisibility visibility = toCSSVisibilityNonInterpolableValue(nonInterpolableValue)->visibility(fraction);
    environment.state().style()->setVisibility(visibility);
}

} // namespace blink
