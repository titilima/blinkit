// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: StringKeyframe.cpp
// Description: StringKeyframe Class
//      Author: Ziming Li
//     Created: 2019-03-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/StringKeyframe.h"

#include "core/StylePropertyShorthand.h"
#include "core/animation/CSSColorInterpolationType.h"
#include "core/animation/CSSFontWeightInterpolationType.h"
#include "core/animation/CSSImageInterpolationType.h"
#include "core/animation/CSSImageListInterpolationType.h"
#include "core/animation/CSSLengthInterpolationType.h"
#include "core/animation/CSSNumberInterpolationType.h"
#include "core/animation/CSSPaintInterpolationType.h"
#include "core/animation/CSSShadowListInterpolationType.h"
#include "core/animation/CSSValueInterpolationType.h"
#include "core/animation/CSSVisibilityInterpolationType.h"
#include "core/animation/CompositorAnimations.h"
#include "core/animation/ConstantStyleInterpolation.h"
#include "core/animation/DeferredLegacyStyleInterpolation.h"
#include "core/animation/DoubleStyleInterpolation.h"
#include "core/animation/FilterStyleInterpolation.h"
#include "core/animation/ImageSliceStyleInterpolation.h"
#include "core/animation/InterpolationType.h"
#include "core/animation/InvalidatableInterpolation.h"
#include "core/animation/LegacyStyleInterpolation.h"
#include "core/animation/LengthBoxStyleInterpolation.h"
#include "core/animation/LengthPairStyleInterpolation.h"
#include "core/animation/LengthStyleInterpolation.h"
#include "core/animation/ListStyleInterpolation.h"
#include "core/animation/VisibilityStyleInterpolation.h"
#include "core/animation/css/CSSAnimations.h"
#include "core/css/CSSPropertyMetadata.h"
#include "core/css/resolver/StyleResolver.h"
#include "core/style/ComputedStyle.h"
#include "platform/RuntimeEnabledFeatures.h"

namespace blink {

StringKeyframe::StringKeyframe(const StringKeyframe& copyFrom)
    : Keyframe(copyFrom.m_offset, copyFrom.m_composite, copyFrom.m_easing)
    , m_cssPropertyMap(copyFrom.m_cssPropertyMap->mutableCopy())
    , m_presentationAttributeMap(copyFrom.m_presentationAttributeMap->mutableCopy())
    , m_svgAttributeMap(copyFrom.m_svgAttributeMap)
{
}

void StringKeyframe::setCSSPropertyValue(CSSPropertyID property, const String& value, Element* element, StyleSheetContents* styleSheetContents)
{
    ASSERT(property != CSSPropertyInvalid);
    if (CSSAnimations::isAnimatableProperty(property))
        m_cssPropertyMap->setProperty(property, value, false, styleSheetContents);
}

void StringKeyframe::setCSSPropertyValue(CSSPropertyID property, PassRefPtrWillBeRawPtr<CSSValue> value)
{
    ASSERT(property != CSSPropertyInvalid);
    ASSERT(CSSAnimations::isAnimatableProperty(property));
    m_cssPropertyMap->setProperty(property, value, false);
}

void StringKeyframe::setPresentationAttributeValue(CSSPropertyID property, const String& value, Element* element, StyleSheetContents* styleSheetContents)
{
    ASSERT(property != CSSPropertyInvalid);
    if (CSSAnimations::isAnimatableProperty(property))
        m_presentationAttributeMap->setProperty(property, value, false, styleSheetContents);
}

void StringKeyframe::setSVGAttributeValue(const QualifiedName& attributeName, const String& value)
{
    m_svgAttributeMap.set(&attributeName, value);
}

PropertyHandleSet StringKeyframe::properties() const
{
    // This is not used in time-critical code, so we probably don't need to
    // worry about caching this result.
    PropertyHandleSet properties;
    for (unsigned i = 0; i < m_cssPropertyMap->propertyCount(); ++i) {
        StylePropertySet::PropertyReference propertyReference = m_cssPropertyMap->propertyAt(i);
        ASSERT_WITH_MESSAGE(
            !isShorthandProperty(propertyReference.id()) || propertyReference.value()->isVariableReferenceValue(),
            "Web Animations: Encountered unexpanded shorthand CSS property (%d).", propertyReference.id());
        properties.add(PropertyHandle(propertyReference.id(), false));
    }

    for (unsigned i = 0; i < m_presentationAttributeMap->propertyCount(); ++i)
        properties.add(PropertyHandle(m_presentationAttributeMap->propertyAt(i).id(), true));

    for (const auto& key: m_svgAttributeMap.keys())
        properties.add(PropertyHandle(*key));

    return properties;
}

PassRefPtr<Keyframe> StringKeyframe::clone() const
{
    return adoptRef(new StringKeyframe(*this));
}

PassOwnPtr<Keyframe::PropertySpecificKeyframe> StringKeyframe::createPropertySpecificKeyframe(PropertyHandle property) const
{
    if (property.isCSSProperty())
        return adoptPtr(new CSSPropertySpecificKeyframe(offset(), &easing(), cssPropertyValue(property.cssProperty()), composite()));

    if (property.isPresentationAttribute())
        return adoptPtr(new CSSPropertySpecificKeyframe(offset(), &easing(), presentationAttributeValue(property.presentationAttribute()), composite()));

    ASSERT_NOT_REACHED();
    return nullptr;
}

StringKeyframe::CSSPropertySpecificKeyframe::CSSPropertySpecificKeyframe(double offset, PassRefPtr<TimingFunction> easing, CSSValue* value, EffectModel::CompositeOperation op)
    : Keyframe::PropertySpecificKeyframe(offset, easing, op)
    , m_value(value)
{ }

StringKeyframe::CSSPropertySpecificKeyframe::CSSPropertySpecificKeyframe(double offset, PassRefPtr<TimingFunction> easing, CSSValue* value)
    : Keyframe::PropertySpecificKeyframe(offset, easing, EffectModel::CompositeReplace)
    , m_value(value)
{
    ASSERT(!isNull(m_offset));
}

bool StringKeyframe::CSSPropertySpecificKeyframe::populateAnimatableValue(CSSPropertyID property, Element& element, const ComputedStyle* baseStyle, bool force) const
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(false); // BKTODO: Not reached!
    return false;
#else
    if (m_animatableValueCache && !force)
        return false;
    if (!baseStyle && (!m_value || DeferredLegacyStyleInterpolation::interpolationRequiresStyleResolve(*m_value)))
        return false;
    if (!element.document().frame())
        return false;
    m_animatableValueCache = StyleResolver::createAnimatableValueSnapshot(element, baseStyle, property, m_value.get());
    return true;
#endif
}

namespace {

// TODO(alancutter): Move this into its own file.
const InterpolationTypes* applicableTypesForProperty(PropertyHandle property)
{
    // TODO(alancutter): Initialise this entire HashMap once instead of initialising each property individually.
    using ApplicableTypesMap = HashMap<PropertyHandle, OwnPtr<const InterpolationTypes>>;
    DEFINE_STATIC_LOCAL(ApplicableTypesMap, applicableTypesMap, ());
    auto entry = applicableTypesMap.find(property);
    if (entry != applicableTypesMap.end())
        return entry->value.get();

    bool fallbackToLegacy = false;
    OwnPtr<InterpolationTypes> applicableTypes = adoptPtr(new InterpolationTypes());

    if (property.isCSSProperty() || property.isPresentationAttribute()) {
        CSSPropertyID cssProperty = property.isCSSProperty() ? property.cssProperty() : property.presentationAttribute();
        switch (cssProperty) {
        case CSSPropertyBorderBottomWidth:
        case CSSPropertyBorderLeftWidth:
        case CSSPropertyBorderRightWidth:
        case CSSPropertyBorderTopWidth:
        case CSSPropertyBottom:
        case CSSPropertyFlexBasis:
        case CSSPropertyHeight:
        case CSSPropertyLeft:
        case CSSPropertyLetterSpacing:
        case CSSPropertyMarginBottom:
        case CSSPropertyMarginLeft:
        case CSSPropertyMarginRight:
        case CSSPropertyMarginTop:
        case CSSPropertyMaxHeight:
        case CSSPropertyMaxWidth:
        case CSSPropertyMinHeight:
        case CSSPropertyMinWidth:
        case CSSPropertyMotionOffset:
        case CSSPropertyOutlineOffset:
        case CSSPropertyOutlineWidth:
        case CSSPropertyPaddingBottom:
        case CSSPropertyPaddingLeft:
        case CSSPropertyPaddingRight:
        case CSSPropertyPaddingTop:
        case CSSPropertyPerspective:
        case CSSPropertyRight:
        case CSSPropertyShapeMargin:
        case CSSPropertyTop:
        case CSSPropertyVerticalAlign:
        case CSSPropertyWebkitBorderHorizontalSpacing:
        case CSSPropertyWebkitBorderVerticalSpacing:
        case CSSPropertyWebkitColumnGap:
        case CSSPropertyWebkitColumnRuleWidth:
        case CSSPropertyWebkitColumnWidth:
        case CSSPropertyWebkitPerspectiveOriginX:
        case CSSPropertyWebkitPerspectiveOriginY:
        case CSSPropertyWebkitTransformOriginX:
        case CSSPropertyWebkitTransformOriginY:
        case CSSPropertyWebkitTransformOriginZ:
        case CSSPropertyWidth:
        case CSSPropertyWordSpacing:
            applicableTypes->append(adoptPtr(new CSSLengthInterpolationType(cssProperty)));
            break;
        case CSSPropertyFlexGrow:
        case CSSPropertyFlexShrink:
        case CSSPropertyFontSizeAdjust:
        case CSSPropertyOpacity:
        case CSSPropertyOrphans:
        case CSSPropertyShapeImageThreshold:
        case CSSPropertyWebkitColumnCount:
        case CSSPropertyWidows:
        case CSSPropertyZIndex:
            applicableTypes->append(adoptPtr(new CSSNumberInterpolationType(cssProperty)));
            break;
        case CSSPropertyLineHeight:
            applicableTypes->append(adoptPtr(new CSSLengthInterpolationType(cssProperty)));
            applicableTypes->append(adoptPtr(new CSSNumberInterpolationType(cssProperty)));
            break;
        case CSSPropertyBackgroundColor:
        case CSSPropertyBorderBottomColor:
        case CSSPropertyBorderLeftColor:
        case CSSPropertyBorderRightColor:
        case CSSPropertyBorderTopColor:
        case CSSPropertyColor:
        case CSSPropertyOutlineColor:
        case CSSPropertyTextDecorationColor:
        case CSSPropertyWebkitColumnRuleColor:
        case CSSPropertyWebkitTextStrokeColor:
            applicableTypes->append(adoptPtr(new CSSColorInterpolationType(cssProperty)));
            break;
        case CSSPropertyBoxShadow:
        case CSSPropertyTextShadow:
            applicableTypes->append(adoptPtr(new CSSShadowListInterpolationType(cssProperty)));
            break;
        case CSSPropertyBorderImageSource:
        case CSSPropertyListStyleImage:
        case CSSPropertyWebkitMaskBoxImageSource:
            applicableTypes->append(adoptPtr(new CSSImageInterpolationType(cssProperty)));
            break;
        case CSSPropertyBackgroundImage:
        case CSSPropertyWebkitMaskImage:
            applicableTypes->append(adoptPtr(new CSSImageListInterpolationType(cssProperty)));
            break;
        case CSSPropertyFontWeight:
            applicableTypes->append(adoptPtr(new CSSFontWeightInterpolationType(cssProperty)));
            break;
        case CSSPropertyVisibility:
            applicableTypes->append(adoptPtr(new CSSVisibilityInterpolationType(cssProperty)));
            break;
        default:
            // TODO(alancutter): Support all interpolable CSS properties here so we can stop falling back to the old StyleInterpolation implementation.
            if (CSSPropertyMetadata::isInterpolableProperty(cssProperty))
                fallbackToLegacy = true;
            break;
        }

        if (!fallbackToLegacy)
            applicableTypes->append(adoptPtr(new CSSValueInterpolationType(cssProperty)));

    }

    auto addResult = applicableTypesMap.add(property, fallbackToLegacy ? nullptr : applicableTypes.release());
    return addResult.storedValue->value.get();
}

} // namespace

PassRefPtr<Interpolation> StringKeyframe::CSSPropertySpecificKeyframe::createLegacyStyleInterpolation(CSSPropertyID property, Keyframe::PropertySpecificKeyframe& end, Element* element, const ComputedStyle* baseStyle) const
{
    CSSValue& fromCSSValue = *m_value.get();
    CSSValue& toCSSValue = *toCSSPropertySpecificKeyframe(end).value();
    if (DeferredLegacyStyleInterpolation::interpolationRequiresStyleResolve(fromCSSValue) || DeferredLegacyStyleInterpolation::interpolationRequiresStyleResolve(toCSSValue)) {
        // FIXME: Handle these cases outside of DeferredLegacyStyleInterpolation.
        return DeferredLegacyStyleInterpolation::create(&fromCSSValue, &toCSSValue, property);
    }

    // FIXME: Remove the use of AnimatableValues and Elements here.
    ASSERT(element);
    populateAnimatableValue(property, *element, baseStyle, false);
    end.populateAnimatableValue(property, *element, baseStyle, false);
    return LegacyStyleInterpolation::create(getAnimatableValue(), end.getAnimatableValue(), property);
}

PassRefPtr<Interpolation> StringKeyframe::CSSPropertySpecificKeyframe::maybeCreateInterpolation(PropertyHandle propertyHandle, Keyframe::PropertySpecificKeyframe& end, Element* element, const ComputedStyle* baseStyle) const
{
    const InterpolationTypes* applicableTypes = applicableTypesForProperty(propertyHandle);
    if (applicableTypes)
        return InvalidatableInterpolation::create(propertyHandle, *applicableTypes, *this, end);

    // TODO(alancutter): Remove the remainder of this function.

    // FIXME: Refactor this into a generic piece that lives in InterpolationEffect, and a template parameter specific converter.
    CSSPropertyID property = propertyHandle.isCSSProperty() ? propertyHandle.cssProperty() : propertyHandle.presentationAttribute();
    CSSValue* fromCSSValue = m_value.get();
    CSSValue* toCSSValue = toCSSPropertySpecificKeyframe(end).value();
    InterpolationRange range = RangeAll;

    // FIXME: Remove this flag once we can rely on legacy's behaviour being correct.
    bool forceDefaultInterpolation = false;

    // FIXME: Remove this check once neutral keyframes are implemented in StringKeyframes.
    if (!fromCSSValue || !toCSSValue)
        return DeferredLegacyStyleInterpolation::create(fromCSSValue, toCSSValue, property);

    ASSERT(fromCSSValue && toCSSValue);

    if (!CSSPropertyMetadata::isInterpolableProperty(property)) {
        if (fromCSSValue == toCSSValue)
            return ConstantStyleInterpolation::create(fromCSSValue, property);

        return nullptr;
    }

    if (fromCSSValue->isCSSWideKeyword() || toCSSValue->isCSSWideKeyword())
        return createLegacyStyleInterpolation(property, end, element, baseStyle);

    switch (property) {
    case CSSPropertyFontSize:
        if (LengthStyleInterpolation::canCreateFrom(*fromCSSValue) && LengthStyleInterpolation::canCreateFrom(*toCSSValue))
            return LengthStyleInterpolation::create(*fromCSSValue, *toCSSValue, property, RangeNonNegative);

        // FIXME: Handle keywords e.g. 'smaller', 'larger'.
        if (property == CSSPropertyFontSize)
            return createLegacyStyleInterpolation(property, end, element, baseStyle);

        break;

    case CSSPropertyMotionRotation: {
        RefPtr<Interpolation> interpolation = DoubleStyleInterpolation::maybeCreateFromMotionRotation(*fromCSSValue, *toCSSValue, property);
        if (interpolation)
            return interpolation.release();
            break;
        }
    case CSSPropertyVisibility:
        if (VisibilityStyleInterpolation::canCreateFrom(*fromCSSValue) && VisibilityStyleInterpolation::canCreateFrom(*toCSSValue) && (VisibilityStyleInterpolation::isVisible(*fromCSSValue) || VisibilityStyleInterpolation::isVisible(*toCSSValue)))
            return VisibilityStyleInterpolation::create(*fromCSSValue, *toCSSValue, property);

        break;

    case CSSPropertyBorderBottomLeftRadius:
    case CSSPropertyBorderBottomRightRadius:
    case CSSPropertyBorderTopLeftRadius:
    case CSSPropertyBorderTopRightRadius:
        if (LengthPairStyleInterpolation::canCreateFrom(*fromCSSValue) && LengthPairStyleInterpolation::canCreateFrom(*toCSSValue))
            return LengthPairStyleInterpolation::create(*fromCSSValue, *toCSSValue, property, RangeNonNegative);
        break;

    case CSSPropertyPerspectiveOrigin:
    case CSSPropertyTransformOrigin: {
        RefPtr<Interpolation> interpolation = ListStyleInterpolation<LengthStyleInterpolation>::maybeCreateFromList(*fromCSSValue, *toCSSValue, property, range);
        if (interpolation)
            return interpolation.release();

        // FIXME: Handle keywords: top, right, left, center, bottom
        return createLegacyStyleInterpolation(property, end, element, baseStyle);
    }

    case CSSPropertyClip: {
        if (LengthBoxStyleInterpolation::usesDefaultInterpolation(*fromCSSValue, *toCSSValue)) {
            forceDefaultInterpolation = true;
            break;
        }
        RefPtr<Interpolation> interpolation = LengthBoxStyleInterpolation::maybeCreateFrom(*fromCSSValue, *toCSSValue, property);
        if (interpolation)
            return interpolation.release();
        break;
    }

    case CSSPropertyBorderImageSlice:
    case CSSPropertyWebkitMaskBoxImageSlice: {
        RefPtr<Interpolation> interpolation = ImageSliceStyleInterpolation::maybeCreate(*fromCSSValue, *toCSSValue, property);
        if (interpolation)
            return interpolation.release();
        if (ImageSliceStyleInterpolation::usesDefaultInterpolation(*fromCSSValue, *toCSSValue))
            forceDefaultInterpolation = true;

        break;
    }

    case CSSPropertyWebkitFilter:
    case CSSPropertyBackdropFilter: {
        RefPtr<Interpolation> interpolation = FilterStyleInterpolation::maybeCreateList(*fromCSSValue, *toCSSValue, property);
        if (interpolation)
            return interpolation.release();

        // FIXME: Support drop shadow interpolation.
        return createLegacyStyleInterpolation(property, end, element, baseStyle);
        break;
    }

    case CSSPropertyTranslate: {
        RefPtr<Interpolation> interpolation = ListStyleInterpolation<LengthStyleInterpolation>::maybeCreateFromList(*fromCSSValue, *toCSSValue, property, range);
        if (interpolation)
            return interpolation.release();

        // TODO(soonm): Legacy mode is used when from and to cssvaluelist length does not match.
        return createLegacyStyleInterpolation(property, end, element, baseStyle);
        break;
    }

    case CSSPropertyScale: {
        RefPtr<Interpolation> interpolation = ListStyleInterpolation<DoubleStyleInterpolation>::maybeCreateFromList(*fromCSSValue, *toCSSValue, property, range);
        if (interpolation)
            return interpolation.release();

        // TODO(soonm): Legacy mode is used when from and to cssvaluelist length does not match.
        return createLegacyStyleInterpolation(property, end, element, baseStyle);
        break;
    }

    default:
        // Fall back to LegacyStyleInterpolation.
        return createLegacyStyleInterpolation(property, end, element, baseStyle);
        break;
    }

    if (fromCSSValue == toCSSValue)
        return ConstantStyleInterpolation::create(fromCSSValue, property);

    if (!forceDefaultInterpolation) {
        ASSERT(AnimatableValue::usesDefaultInterpolation(
            StyleResolver::createAnimatableValueSnapshot(*element, baseStyle, property, fromCSSValue).get(),
            StyleResolver::createAnimatableValueSnapshot(*element, baseStyle, property, toCSSValue).get()));
    }

    return nullptr;

}

PassOwnPtr<Keyframe::PropertySpecificKeyframe> StringKeyframe::CSSPropertySpecificKeyframe::neutralKeyframe(double offset, PassRefPtr<TimingFunction> easing) const
{
    return adoptPtr(new CSSPropertySpecificKeyframe(offset, easing, static_cast<CSSValue*>(0), EffectModel::CompositeAdd));
}

PassOwnPtr<Keyframe::PropertySpecificKeyframe> StringKeyframe::CSSPropertySpecificKeyframe::cloneWithOffset(double offset) const
{
    Keyframe::PropertySpecificKeyframe* theClone = new CSSPropertySpecificKeyframe(offset, m_easing, m_value.get());
    toCSSPropertySpecificKeyframe(theClone)->m_animatableValueCache = m_animatableValueCache;
    return adoptPtr(theClone);
}

} // namespace blink
