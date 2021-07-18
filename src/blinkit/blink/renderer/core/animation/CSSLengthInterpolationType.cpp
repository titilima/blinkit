// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/CSSLengthInterpolationType.h"

#include "core/animation/LengthPropertyFunctions.h"
#include "core/animation/css/CSSAnimatableValueFactory.h"
#include "core/css/CSSCalculationValue.h"
#include "core/css/resolver/StyleBuilder.h"
#include "core/css/resolver/StyleResolverState.h"

namespace blink {

// This class is implemented as a singleton whose instance represents the presence of percentages being used in a Length value
// while nullptr represents the absence of any percentages.
class CSSLengthNonInterpolableValue : public NonInterpolableValue {
public:
    ~CSSLengthNonInterpolableValue() final { ASSERT_NOT_REACHED(); }
    static PassRefPtr<CSSLengthNonInterpolableValue> create(bool hasPercentage)
    {
        DEFINE_STATIC_REF(CSSLengthNonInterpolableValue, singleton, adoptRef(new CSSLengthNonInterpolableValue()));
        ASSERT(singleton);
        return hasPercentage ? singleton : nullptr;
    }
    static PassRefPtr<CSSLengthNonInterpolableValue> merge(const NonInterpolableValue* a, const NonInterpolableValue* b)
    {
        return create(hasPercentage(a) || hasPercentage(b));
    }
    static bool hasPercentage(const NonInterpolableValue* nonInterpolableValue)
    {
        ASSERT(!nonInterpolableValue || nonInterpolableValue->type() == CSSLengthNonInterpolableValue::staticType);
        return static_cast<bool>(nonInterpolableValue);
    }

    DECLARE_NON_INTERPOLABLE_VALUE_TYPE();

private:
    CSSLengthNonInterpolableValue() { }
};

DEFINE_NON_INTERPOLABLE_VALUE_TYPE(CSSLengthNonInterpolableValue);
DEFINE_NON_INTERPOLABLE_VALUE_TYPE_CASTS(CSSLengthNonInterpolableValue);

CSSLengthInterpolationType::CSSLengthInterpolationType(CSSPropertyID property)
    : CSSInterpolationType(property)
    , m_valueRange(LengthPropertyFunctions::valueRange(property))
{ }

float CSSLengthInterpolationType::effectiveZoom(const ComputedStyle& style) const
{
    return LengthPropertyFunctions::isZoomedLength(cssProperty()) ? style.effectiveZoom() : 1;
}

PassOwnPtr<InterpolableValue> CSSLengthInterpolationType::createInterpolablePixels(double pixels)
{
    OwnPtr<InterpolableList> interpolableList = createNeutralInterpolableValue();
    interpolableList->set(CSSPrimitiveValue::UnitTypePixels, InterpolableNumber::create(pixels));
    return interpolableList.release();
}

InterpolationComponent CSSLengthInterpolationType::maybeConvertLength(const Length& length, float zoom)
{
    if (!length.isSpecified())
        return nullptr;

    PixelsAndPercent pixelsAndPercent = length.pixelsAndPercent();
    OwnPtr<InterpolableList> values = createNeutralInterpolableValue();
    values->set(CSSPrimitiveValue::UnitTypePixels, InterpolableNumber::create(pixelsAndPercent.pixels / zoom));
    values->set(CSSPrimitiveValue::UnitTypePercentage, InterpolableNumber::create(pixelsAndPercent.percent));

    return InterpolationComponent(values.release(), CSSLengthNonInterpolableValue::create(length.hasPercent()));
}

PassOwnPtr<InterpolableList> CSSLengthInterpolationType::createNeutralInterpolableValue()
{
    const size_t length = CSSPrimitiveValue::LengthUnitTypeCount;
    OwnPtr<InterpolableList> values = InterpolableList::create(length);
    for (size_t i = 0; i < length; i++)
        values->set(i, InterpolableNumber::create(0));
    return values.release();
}

PairwiseInterpolationComponent CSSLengthInterpolationType::mergeSingleConversionComponents(InterpolationComponent& startValue, InterpolationComponent& endValue)
{
    return PairwiseInterpolationComponent(
        startValue.interpolableValue.release(),
        endValue.interpolableValue.release(),
        CSSLengthNonInterpolableValue::merge(startValue.nonInterpolableValue.get(), endValue.nonInterpolableValue.get()));
}

bool CSSLengthInterpolationType::nonInterpolableValuesAreCompatible(const NonInterpolableValue*, const NonInterpolableValue*)
{
    return true;
}

void CSSLengthInterpolationType::composite(
    OwnPtr<InterpolableValue>& underlyingInterpolableValue,
    RefPtr<NonInterpolableValue>& underlyingNonInterpolableValue,
    double underlyingFraction,
    const InterpolableValue& interpolableValue,
    const NonInterpolableValue* nonInterpolableValue)
{
    underlyingInterpolableValue->scaleAndAdd(underlyingFraction, interpolableValue);
    underlyingNonInterpolableValue = CSSLengthNonInterpolableValue::merge(underlyingNonInterpolableValue.get(), nonInterpolableValue);
}

InterpolationComponent CSSLengthInterpolationType::maybeConvertCSSValue(const CSSValue& value)
{
    if (!value.isPrimitiveValue())
        return nullptr;

    const CSSPrimitiveValue& primitiveValue = toCSSPrimitiveValue(value);

    CSSLengthArray valueArray;
    for (size_t i = 0; i < CSSPrimitiveValue::LengthUnitTypeCount; i++)
        valueArray.append(0);
    bool hasPercentage = false;

    if (!primitiveValue.isLength() && !primitiveValue.isPercentage() && !primitiveValue.isCalculatedPercentageWithLength())
        return nullptr;
    CSSLengthTypeArray hasType;
    hasType.ensureSize(CSSPrimitiveValue::LengthUnitTypeCount);
    primitiveValue.accumulateLengthArray(valueArray, hasType);
    hasPercentage = hasType.get(CSSPrimitiveValue::UnitTypePercentage);

    OwnPtr<InterpolableList> values = InterpolableList::create(CSSPrimitiveValue::LengthUnitTypeCount);
    for (size_t i = 0; i < CSSPrimitiveValue::LengthUnitTypeCount; i++)
        values->set(i, InterpolableNumber::create(valueArray.at(i)));

    return InterpolationComponent(values.release(), CSSLengthNonInterpolableValue::create(hasPercentage));
}

class ParentLengthChecker : public InterpolationType::ConversionChecker {
public:
    static PassOwnPtr<ParentLengthChecker> create(const InterpolationType& type, CSSPropertyID property, const Length& length)
    {
        return adoptPtr(new ParentLengthChecker(type, property, length));
    }

private:
    ParentLengthChecker(const InterpolationType& type, CSSPropertyID property, const Length& length)
        : ConversionChecker(type)
        , m_property(property)
        , m_length(length)
    { }

    bool isValid(const InterpolationEnvironment& environment, const UnderlyingValue&) const final
    {
        Length parentLength;
        if (!LengthPropertyFunctions::getLength(m_property, *environment.state().parentStyle(), parentLength))
            return false;
        return parentLength == m_length;
    }

    const CSSPropertyID m_property;
    const Length m_length;
};

PassOwnPtr<InterpolationValue> CSSLengthInterpolationType::maybeConvertNeutral(const UnderlyingValue&, ConversionCheckers&) const
{
    return InterpolationValue::create(*this, createNeutralInterpolableValue());
}

PassOwnPtr<InterpolationValue> CSSLengthInterpolationType::maybeConvertInitial() const
{
    Length initialLength;
    if (!LengthPropertyFunctions::getInitialLength(cssProperty(), initialLength))
        return nullptr;
    InterpolationComponent component = maybeConvertLength(initialLength, 1);
    return component ? InterpolationValue::create(*this, component) : nullptr;
}

PassOwnPtr<InterpolationValue> CSSLengthInterpolationType::maybeConvertInherit(const StyleResolverState& state, ConversionCheckers& conversionCheckers) const
{
    if (!state.parentStyle())
        return nullptr;
    Length inheritedLength;
    if (!LengthPropertyFunctions::getLength(cssProperty(), *state.parentStyle(), inheritedLength))
        return nullptr;
    conversionCheckers.append(ParentLengthChecker::create(*this, cssProperty(), inheritedLength));
    InterpolationComponent component = maybeConvertLength(inheritedLength, effectiveZoom(*state.parentStyle()));
    return component ? InterpolationValue::create(*this, component) : nullptr;
}

PassOwnPtr<InterpolationValue> CSSLengthInterpolationType::maybeConvertValue(const CSSValue& value, const StyleResolverState&, ConversionCheckers& conversionCheckers) const
{
    if (value.isPrimitiveValue() && toCSSPrimitiveValue(value).isValueID()) {
        CSSValueID valueID = toCSSPrimitiveValue(value).getValueID();
        double pixels;
        if (!LengthPropertyFunctions::getPixelsForKeyword(cssProperty(), valueID, pixels))
            return nullptr;
        return InterpolationValue::create(*this, createInterpolablePixels(pixels));
    }

    InterpolationComponent component = maybeConvertCSSValue(value);
    if (!component)
        return nullptr;
    return InterpolationValue::create(*this, component);
}

PassOwnPtr<InterpolationValue> CSSLengthInterpolationType::maybeConvertUnderlyingValue(const InterpolationEnvironment& environment) const
{
    Length underlyingLength;
    if (!LengthPropertyFunctions::getLength(cssProperty(), *environment.state().style(), underlyingLength))
        return nullptr;
    InterpolationComponent component = maybeConvertLength(underlyingLength, effectiveZoom(*environment.state().style()));
    return component ? InterpolationValue::create(*this, component) : nullptr;
}

PassOwnPtr<PairwisePrimitiveInterpolation> CSSLengthInterpolationType::mergeSingleConversions(InterpolationValue& startValue, InterpolationValue& endValue) const
{
    PairwiseInterpolationComponent component = mergeSingleConversionComponents(startValue.mutableComponent(), endValue.mutableComponent());
    return PairwisePrimitiveInterpolation::create(*this, component);
}

void CSSLengthInterpolationType::composite(UnderlyingValue& underlyingValue, double underlyingFraction, const InterpolationValue& value) const
{
    InterpolationComponent& underlyingComponent = underlyingValue.mutableComponent();
    composite(underlyingComponent.interpolableValue, underlyingComponent.nonInterpolableValue,
        underlyingFraction, value.interpolableValue(), value.nonInterpolableValue());
}

static bool isPixelsOrPercentOnly(const InterpolableList& values)
{
    for (size_t i = 0; i < CSSPrimitiveValue::LengthUnitTypeCount; i++) {
        if (i == CSSPrimitiveValue::UnitTypePixels || i == CSSPrimitiveValue::UnitTypePercentage)
            continue;
        if (toInterpolableNumber(values.get(i))->value())
            return false;
    }
    return true;
}

// TODO(alancutter): Move this to Length.h.
static double clampToRange(double x, ValueRange range)
{
    return (range == ValueRangeNonNegative && x < 0) ? 0 : x;
}

static Length createLength(double pixels, double percentage, bool hasPercentage, ValueRange range)
{
    ASSERT(hasPercentage || percentage == 0);
    if (pixels && hasPercentage)
        return Length(CalculationValue::create(PixelsAndPercent(pixels, percentage), range));
    if (hasPercentage)
        return Length(clampToRange(percentage, range), Percent);
    return Length(CSSPrimitiveValue::clampToCSSLengthRange(clampToRange(pixels, range)), Fixed);
}

static Length resolveInterpolablePixelsOrPercentageLength(const InterpolableList& values, bool hasPercentage, ValueRange range, double zoom)
{
    ASSERT(isPixelsOrPercentOnly(values));
    double pixels = toInterpolableNumber(values.get(CSSPrimitiveValue::UnitTypePixels))->value() * zoom;
    double percentage = toInterpolableNumber(values.get(CSSPrimitiveValue::UnitTypePercentage))->value();
    return createLength(pixels, percentage, hasPercentage, range);
}

Length CSSLengthInterpolationType::resolveInterpolableLength(const InterpolableValue& interpolableValue, const NonInterpolableValue* nonInterpolableValue, const CSSToLengthConversionData& conversionData, ValueRange range)
{
    const InterpolableList& interpolableList = toInterpolableList(interpolableValue);
    bool hasPercentage = CSSLengthNonInterpolableValue::hasPercentage(nonInterpolableValue);
    double pixels = 0;
    double percentage = 0;
    for (size_t i = 0; i < CSSPrimitiveValue::LengthUnitTypeCount; i++) {
        double value = toInterpolableNumber(*interpolableList.get(i)).value();
        if (i == CSSPrimitiveValue::UnitTypePercentage) {
            percentage = value;
        } else {
            CSSPrimitiveValue::UnitType type = CSSPrimitiveValue::lengthUnitTypeToUnitType(static_cast<CSSPrimitiveValue::LengthUnitType>(i));
            pixels += conversionData.zoomedComputedPixels(value, type);
        }
    }
    return createLength(pixels, percentage, hasPercentage, range);
}

static CSSPrimitiveValue::UnitType toUnitType(int lengthUnitType)
{
    return static_cast<CSSPrimitiveValue::UnitType>(CSSPrimitiveValue::lengthUnitTypeToUnitType(static_cast<CSSPrimitiveValue::LengthUnitType>(lengthUnitType)));
}

static PassRefPtrWillBeRawPtr<CSSCalcExpressionNode> createCalcExpression(const InterpolableList& values, bool hasPercentage)
{
    RefPtrWillBeRawPtr<CSSCalcExpressionNode> result = nullptr;
    for (size_t i = 0; i < CSSPrimitiveValue::LengthUnitTypeCount; i++) {
        double value = toInterpolableNumber(values.get(i))->value();
        if (value || (i == CSSPrimitiveValue::UnitTypePercentage && hasPercentage)) {
            RefPtrWillBeRawPtr<CSSCalcExpressionNode> node = CSSCalcValue::createExpressionNode(CSSPrimitiveValue::create(value, toUnitType(i)));
            result = result ? CSSCalcValue::createExpressionNode(result.release(), node.release(), CalcAdd) : node.release();
        }
    }
    ASSERT(result);
    return result.release();
}

static PassRefPtrWillBeRawPtr<CSSValue> createCSSValue(const InterpolableList& values, bool hasPercentage, ValueRange range)
{
    RefPtrWillBeRawPtr<CSSPrimitiveValue> result;
    size_t firstUnitIndex = CSSPrimitiveValue::LengthUnitTypeCount;
    size_t unitTypeCount = 0;
    for (size_t i = 0; i < CSSPrimitiveValue::LengthUnitTypeCount; i++) {
        if ((hasPercentage && i == CSSPrimitiveValue::UnitTypePercentage) || toInterpolableNumber(values.get(i))->value()) {
            unitTypeCount++;
            if (unitTypeCount == 1)
                firstUnitIndex = i;
        }
    }
    switch (unitTypeCount) {
    case 0:
        return CSSPrimitiveValue::create(0, CSSPrimitiveValue::UnitType::Pixels);
    case 1: {
        double value = clampToRange(toInterpolableNumber(values.get(firstUnitIndex))->value(), range);
        return CSSPrimitiveValue::create(value, toUnitType(firstUnitIndex));
    }
    default:
        return CSSPrimitiveValue::create(CSSCalcValue::create(createCalcExpression(values, hasPercentage), range));
    }
}

void CSSLengthInterpolationType::apply(const InterpolableValue& interpolableValue, const NonInterpolableValue* nonInterpolableValue, InterpolationEnvironment& environment) const
{
    StyleResolverState& state = environment.state();
    const InterpolableList& values = toInterpolableList(interpolableValue);
    bool hasPercentage = CSSLengthNonInterpolableValue::hasPercentage(nonInterpolableValue);
    if (isPixelsOrPercentOnly(values)) {
        Length length = resolveInterpolablePixelsOrPercentageLength(values, hasPercentage, m_valueRange, effectiveZoom(*state.style()));
        if (LengthPropertyFunctions::setLength(cssProperty(), *state.style(), length)) {
#if ENABLE(ASSERT)
            // Assert that setting the length on ComputedStyle directly is identical to the AnimatableValue code path.
            RefPtr<AnimatableValue> before = CSSAnimatableValueFactory::create(cssProperty(), *state.style());
            StyleBuilder::applyProperty(cssProperty(), state, createCSSValue(values, hasPercentage, m_valueRange).get());
            RefPtr<AnimatableValue> after = CSSAnimatableValueFactory::create(cssProperty(), *state.style());
            ASSERT(before->equals(*after));
#endif
            return;
        }
    }
    StyleBuilder::applyProperty(cssProperty(), state, createCSSValue(values, hasPercentage, m_valueRange).get());
}

} // namespace blink
