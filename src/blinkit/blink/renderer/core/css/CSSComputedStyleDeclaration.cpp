// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSComputedStyleDeclaration.cpp
// Description: CSSComputedStyleDeclaration Class
//      Author: Ziming Li
//     Created: 2021-07-28
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004 Zack Rusin <zack@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Alexey Proskuryakov <ap@webkit.org>
 * Copyright (C) 2007 Nicholas Shanks <webkit@nickshanks.com>
 * Copyright (C) 2011 Sencha, Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include "core/css/CSSComputedStyleDeclaration.h"

#include "bindings/core/v8/ExceptionState.h"
#include "core/CSSPropertyNames.h"
#include "core/css/CSSPrimitiveValueMappings.h"
#include "core/css/CSSPropertyMetadata.h"
#include "core/css/CSSSelector.h"
#include "core/css/CSSValuePool.h"
#include "core/css/ComputedStyleCSSValueMapping.h"
#include "core/css/parser/CSSParser.h"
#include "core/css/parser/CSSVariableParser.h"
#include "core/css/resolver/StyleResolver.h"
#include "core/dom/Document.h"
#include "core/dom/ExceptionCode.h"
#include "core/dom/PseudoElement.h"
#include "core/layout/LayoutObject.h"
#include "core/style/ComputedStyle.h"
#include "wtf/text/StringBuilder.h"

namespace blink {

// List of all properties we know how to compute, omitting shorthands.
// NOTE: Do not use this list, use computableProperties() instead
// to respect runtime enabling of CSS properties.
static const CSSPropertyID staticComputableProperties[] = {
    CSSPropertyAnimationDelay,
    CSSPropertyAnimationDirection,
    CSSPropertyAnimationDuration,
    CSSPropertyAnimationFillMode,
    CSSPropertyAnimationIterationCount,
    CSSPropertyAnimationName,
    CSSPropertyAnimationPlayState,
    CSSPropertyAnimationTimingFunction,
    CSSPropertyBackgroundAttachment,
    CSSPropertyBackgroundBlendMode,
    CSSPropertyBackgroundClip,
    CSSPropertyBackgroundColor,
    CSSPropertyBackgroundImage,
    CSSPropertyBackgroundOrigin,
    CSSPropertyBackgroundPosition, // more-specific background-position-x/y are non-standard
    CSSPropertyBackgroundRepeat,
    CSSPropertyBackgroundSize,
    CSSPropertyBorderBottomColor,
    CSSPropertyBorderBottomLeftRadius,
    CSSPropertyBorderBottomRightRadius,
    CSSPropertyBorderBottomStyle,
    CSSPropertyBorderBottomWidth,
    CSSPropertyBorderCollapse,
    CSSPropertyBorderImageOutset,
    CSSPropertyBorderImageRepeat,
    CSSPropertyBorderImageSlice,
    CSSPropertyBorderImageSource,
    CSSPropertyBorderImageWidth,
    CSSPropertyBorderLeftColor,
    CSSPropertyBorderLeftStyle,
    CSSPropertyBorderLeftWidth,
    CSSPropertyBorderRightColor,
    CSSPropertyBorderRightStyle,
    CSSPropertyBorderRightWidth,
    CSSPropertyBorderTopColor,
    CSSPropertyBorderTopLeftRadius,
    CSSPropertyBorderTopRightRadius,
    CSSPropertyBorderTopStyle,
    CSSPropertyBorderTopWidth,
    CSSPropertyBottom,
    CSSPropertyBoxShadow,
    CSSPropertyBoxSizing,
    CSSPropertyCaptionSide,
    CSSPropertyClear,
    CSSPropertyClip,
    CSSPropertyColor,
    CSSPropertyContent,
    CSSPropertyCursor,
    CSSPropertyDirection,
    CSSPropertyDisplay,
    CSSPropertyEmptyCells,
    CSSPropertyFloat,
    CSSPropertyFontFamily,
    CSSPropertyFontKerning,
    CSSPropertyFontSize,
    CSSPropertyFontSizeAdjust,
    CSSPropertyFontStretch,
    CSSPropertyFontStyle,
    CSSPropertyFontVariant,
    CSSPropertyFontVariantLigatures,
    CSSPropertyFontWeight,
    CSSPropertyHeight,
    CSSPropertyImageOrientation,
    CSSPropertyImageRendering,
    CSSPropertyIsolation,
    CSSPropertyJustifyItems,
    CSSPropertyJustifySelf,
    CSSPropertyLeft,
    CSSPropertyLetterSpacing,
    CSSPropertyLineHeight,
    CSSPropertyListStyleImage,
    CSSPropertyListStylePosition,
    CSSPropertyListStyleType,
    CSSPropertyMarginBottom,
    CSSPropertyMarginLeft,
    CSSPropertyMarginRight,
    CSSPropertyMarginTop,
    CSSPropertyMaxHeight,
    CSSPropertyMaxWidth,
    CSSPropertyMinHeight,
    CSSPropertyMinWidth,
    CSSPropertyMixBlendMode,
    CSSPropertyMotionOffset,
    CSSPropertyMotionPath,
    CSSPropertyMotionRotation,
    CSSPropertyObjectFit,
    CSSPropertyObjectPosition,
    CSSPropertyOpacity,
    CSSPropertyOrphans,
    CSSPropertyOutlineColor,
    CSSPropertyOutlineOffset,
    CSSPropertyOutlineStyle,
    CSSPropertyOutlineWidth,
    CSSPropertyOverflowWrap,
    CSSPropertyOverflowX,
    CSSPropertyOverflowY,
    CSSPropertyPaddingBottom,
    CSSPropertyPaddingLeft,
    CSSPropertyPaddingRight,
    CSSPropertyPaddingTop,
    CSSPropertyPageBreakAfter,
    CSSPropertyPageBreakBefore,
    CSSPropertyPageBreakInside,
    CSSPropertyPointerEvents,
    CSSPropertyPosition,
    CSSPropertyResize,
    CSSPropertyRight,
    CSSPropertyScrollBehavior,
    CSSPropertySpeak,
    CSSPropertyTableLayout,
    CSSPropertyTabSize,
    CSSPropertyTextAlign,
    CSSPropertyTextAlignLast,
    CSSPropertyTextDecoration,
    CSSPropertyTextDecorationLine,
    CSSPropertyTextDecorationStyle,
    CSSPropertyTextDecorationColor,
    CSSPropertyTextJustify,
    CSSPropertyTextUnderlinePosition,
    CSSPropertyTextIndent,
    CSSPropertyTextRendering,
    CSSPropertyTextShadow,
    CSSPropertyTextOverflow,
    CSSPropertyTextTransform,
    CSSPropertyTop,
    CSSPropertyTouchAction,
    CSSPropertyTransitionDelay,
    CSSPropertyTransitionDuration,
    CSSPropertyTransitionProperty,
    CSSPropertyTransitionTimingFunction,
    CSSPropertyUnicodeBidi,
    CSSPropertyVerticalAlign,
    CSSPropertyVisibility,
    CSSPropertyWhiteSpace,
    CSSPropertyWidows,
    CSSPropertyWidth,
    CSSPropertyWillChange,
    CSSPropertyWordBreak,
    CSSPropertyWordSpacing,
    CSSPropertyWordWrap,
    CSSPropertyZIndex,
    CSSPropertyZoom,

    CSSPropertyWebkitAppearance,
    CSSPropertyBackfaceVisibility,
    CSSPropertyWebkitBackgroundClip,
    CSSPropertyWebkitBackgroundComposite,
    CSSPropertyWebkitBackgroundOrigin,
    CSSPropertyWebkitBorderHorizontalSpacing,
    CSSPropertyWebkitBorderImage,
    CSSPropertyWebkitBorderVerticalSpacing,
    CSSPropertyWebkitBoxAlign,
    CSSPropertyWebkitBoxDecorationBreak,
    CSSPropertyWebkitBoxDirection,
    CSSPropertyWebkitBoxFlex,
    CSSPropertyWebkitBoxFlexGroup,
    CSSPropertyWebkitBoxLines,
    CSSPropertyWebkitBoxOrdinalGroup,
    CSSPropertyWebkitBoxOrient,
    CSSPropertyWebkitBoxPack,
    CSSPropertyWebkitBoxReflect,
    CSSPropertyWebkitClipPath,
    CSSPropertyWebkitColumnBreakAfter,
    CSSPropertyWebkitColumnBreakBefore,
    CSSPropertyWebkitColumnBreakInside,
    CSSPropertyWebkitColumnCount,
    CSSPropertyWebkitColumnGap,
    CSSPropertyWebkitColumnRuleColor,
    CSSPropertyWebkitColumnRuleStyle,
    CSSPropertyWebkitColumnRuleWidth,
    CSSPropertyWebkitColumnSpan,
    CSSPropertyWebkitColumnWidth,
    CSSPropertyWebkitFilter,
    CSSPropertyBackdropFilter,
    CSSPropertyAlignContent,
    CSSPropertyAlignItems,
    CSSPropertyAlignSelf,
    CSSPropertyFlexBasis,
    CSSPropertyFlexGrow,
    CSSPropertyFlexShrink,
    CSSPropertyFlexDirection,
    CSSPropertyFlexWrap,
    CSSPropertyJustifyContent,
    CSSPropertyWebkitFontSmoothing,
    CSSPropertyGridAutoColumns,
    CSSPropertyGridAutoFlow,
    CSSPropertyGridAutoRows,
    CSSPropertyGridColumnEnd,
    CSSPropertyGridColumnStart,
    CSSPropertyGridTemplateAreas,
    CSSPropertyGridTemplateColumns,
    CSSPropertyGridTemplateRows,
    CSSPropertyGridRowEnd,
    CSSPropertyGridRowStart,
    CSSPropertyGridColumnGap,
    CSSPropertyGridRowGap,
    CSSPropertyWebkitHighlight,
    CSSPropertyWebkitHyphenateCharacter,
    CSSPropertyWebkitLineBreak,
    CSSPropertyWebkitLineClamp,
    CSSPropertyWebkitLocale,
    CSSPropertyWebkitMarginBeforeCollapse,
    CSSPropertyWebkitMarginAfterCollapse,
    CSSPropertyWebkitMaskBoxImage,
    CSSPropertyWebkitMaskBoxImageOutset,
    CSSPropertyWebkitMaskBoxImageRepeat,
    CSSPropertyWebkitMaskBoxImageSlice,
    CSSPropertyWebkitMaskBoxImageSource,
    CSSPropertyWebkitMaskBoxImageWidth,
    CSSPropertyWebkitMaskClip,
    CSSPropertyWebkitMaskComposite,
    CSSPropertyWebkitMaskImage,
    CSSPropertyWebkitMaskOrigin,
    CSSPropertyWebkitMaskPosition,
    CSSPropertyWebkitMaskRepeat,
    CSSPropertyWebkitMaskSize,
    CSSPropertyOrder,
    CSSPropertyPerspective,
    CSSPropertyPerspectiveOrigin,
    CSSPropertyWebkitPrintColorAdjust,
    CSSPropertyWebkitRtlOrdering,
    CSSPropertyShapeOutside,
    CSSPropertyShapeImageThreshold,
    CSSPropertyShapeMargin,
    CSSPropertyWebkitTapHighlightColor,
    CSSPropertyWebkitTextCombine,
    CSSPropertyWebkitTextDecorationsInEffect,
    CSSPropertyWebkitTextEmphasisColor,
    CSSPropertyWebkitTextEmphasisPosition,
    CSSPropertyWebkitTextEmphasisStyle,
    CSSPropertyWebkitTextFillColor,
    CSSPropertyWebkitTextOrientation,
    CSSPropertyWebkitTextSecurity,
    CSSPropertyWebkitTextStrokeColor,
    CSSPropertyWebkitTextStrokeWidth,
    CSSPropertyTransform,
    CSSPropertyTransformOrigin,
    CSSPropertyTransformStyle,
    CSSPropertyWebkitUserDrag,
    CSSPropertyWebkitUserModify,
    CSSPropertyWebkitUserSelect,
    CSSPropertyWebkitWritingMode,
    CSSPropertyWebkitAppRegion,
#if 0 // BKTODO:
    CSSPropertyBufferedRendering,
    CSSPropertyClipPath,
    CSSPropertyClipRule,
    CSSPropertyMask,
    CSSPropertyFilter,
    CSSPropertyFloodColor,
    CSSPropertyFloodOpacity,
    CSSPropertyLightingColor,
    CSSPropertyStopColor,
    CSSPropertyStopOpacity,
    CSSPropertyColorInterpolation,
    CSSPropertyColorInterpolationFilters,
    CSSPropertyColorRendering,
    CSSPropertyFill,
    CSSPropertyFillOpacity,
    CSSPropertyFillRule,
    CSSPropertyMarkerEnd,
    CSSPropertyMarkerMid,
    CSSPropertyMarkerStart,
    CSSPropertyMaskType,
#endif
    CSSPropertyMaskSourceType,
#if 0 // BKTODO:
    CSSPropertyShapeRendering,
    CSSPropertyStroke,
    CSSPropertyStrokeDasharray,
    CSSPropertyStrokeDashoffset,
    CSSPropertyStrokeLinecap,
    CSSPropertyStrokeLinejoin,
    CSSPropertyStrokeMiterlimit,
    CSSPropertyStrokeOpacity,
    CSSPropertyStrokeWidth,
    CSSPropertyAlignmentBaseline,
    CSSPropertyBaselineShift,
    CSSPropertyDominantBaseline,
    CSSPropertyTextAnchor,
#endif
    CSSPropertyWritingMode,
#if 0 // BKTODO:
    CSSPropertyVectorEffect,
    CSSPropertyPaintOrder,
    CSSPropertyD,
    CSSPropertyCx,
    CSSPropertyCy,
    CSSPropertyX,
    CSSPropertyY,
    CSSPropertyR,
    CSSPropertyRx,
    CSSPropertyRy,
#endif
    CSSPropertyScrollSnapType,
    CSSPropertyScrollSnapPointsX,
    CSSPropertyScrollSnapPointsY,
    CSSPropertyScrollSnapCoordinate,
    CSSPropertyScrollSnapDestination,
    CSSPropertyTranslate,
    CSSPropertyRotate,
    CSSPropertyScale,
};

static const Vector<CSSPropertyID>& computableProperties()
{
    DEFINE_STATIC_LOCAL(Vector<CSSPropertyID>, properties, ());
    if (properties.isEmpty())
        CSSPropertyMetadata::filterEnabledCSSPropertiesIntoVector(staticComputableProperties, WTF_ARRAY_LENGTH(staticComputableProperties), properties);
    return properties;
}

CSSComputedStyleDeclaration::CSSComputedStyleDeclaration(PassRefPtrWillBeRawPtr<Node> n, bool allowVisitedStyle, const String& pseudoElementName)
    : m_node(n)
    , m_allowVisitedStyle(allowVisitedStyle)
{
    unsigned nameWithoutColonsStart = pseudoElementName[0] == ':' ? (pseudoElementName[1] == ':' ? 2 : 1) : 0;
    m_pseudoElementSpecifier = CSSSelector::pseudoId(CSSSelector::parsePseudoType(
        AtomicString(pseudoElementName.substring(nameWithoutColonsStart)), false));
}

CSSComputedStyleDeclaration::~CSSComputedStyleDeclaration()
{
}

String CSSComputedStyleDeclaration::cssText() const
{
    StringBuilder result;
    const Vector<CSSPropertyID>& properties = computableProperties();

    for (unsigned i = 0; i < properties.size(); i++) {
        if (i)
            result.append(' ');
        result.append(getPropertyName(properties[i]));
        result.appendLiteral(": ");
        result.append(getPropertyValue(properties[i]));
        result.append(';');
    }

    return result.toString();
}

void CSSComputedStyleDeclaration::setCSSText(const String&, ExceptionState& exceptionState)
{
    exceptionState.throwDOMException(NoModificationAllowedError, "These styles are computed, and therefore read-only.");
}

static CSSValueID cssIdentifierForFontSizeKeyword(int keywordSize)
{
    ASSERT_ARG(keywordSize, keywordSize);
    ASSERT_ARG(keywordSize, keywordSize <= 8);
    return static_cast<CSSValueID>(CSSValueXxSmall + keywordSize - 1);
}

inline static GCRefPtr<CSSPrimitiveValue> zoomAdjustedPixelValue(double value, const ComputedStyle& style)
{
    return cssValuePool().createValue(adjustFloatForAbsoluteZoom(value, style), CSSPrimitiveValue::UnitType::Pixels);
}

GCRefPtr<CSSValue> CSSComputedStyleDeclaration::getFontSizeCSSValuePreferringKeyword() const
{
    if (!m_node)
        return nullptr;

    m_node->document().updateLayoutIgnorePendingStylesheets();

    const ComputedStyle* style = m_node->ensureComputedStyle(m_pseudoElementSpecifier);
    if (!style)
        return nullptr;

    if (int keywordSize = style->fontDescription().keywordSize())
        return cssValuePool().createIdentifierValue(cssIdentifierForFontSizeKeyword(keywordSize));


    return zoomAdjustedPixelValue(style->fontDescription().computedPixelSize(), *style);
}

bool CSSComputedStyleDeclaration::isMonospaceFont() const
{
    if (!m_node)
        return false;

    const ComputedStyle* style = m_node->ensureComputedStyle(m_pseudoElementSpecifier);
    if (!style)
        return false;

    return style->fontDescription().isMonospace();
}

static void logUnimplementedPropertyID(CSSPropertyID propertyID)
{
    DEFINE_STATIC_LOCAL(HashSet<CSSPropertyID>, propertyIDSet, ());
    if (!propertyIDSet.add(propertyID).isNewEntry)
        return;

    WTF_LOG_ERROR("WebKit does not yet implement getComputedStyle for '%s'.", getPropertyName(propertyID));
}

static bool isLayoutDependent(CSSPropertyID propertyID, const ComputedStyle* style, LayoutObject* layoutObject)
{
    if (!layoutObject)
        return false;

    // Some properties only depend on layout in certain conditions which
    // are specified in the main switch statement below. So we can avoid
    // forcing layout in those conditions. The conditions in this switch
    // statement must remain in sync with the conditions in the main switch.
    // FIXME: Some of these cases could be narrowed down or optimized better.
    switch (propertyID) {
    case CSSPropertyBottom:
    case CSSPropertyHeight:
    case CSSPropertyLeft:
    case CSSPropertyRight:
    case CSSPropertyTop:
    case CSSPropertyPerspectiveOrigin:
    case CSSPropertyTransform:
    case CSSPropertyTranslate:
    case CSSPropertyTransformOrigin:
    case CSSPropertyWidth:
        return layoutObject->isBox();
    case CSSPropertyMargin:
        return layoutObject->isBox()
            && (!style || !style->marginBottom().isFixed() || !style->marginTop().isFixed()
                || !style->marginLeft().isFixed() || !style->marginRight().isFixed());
    case CSSPropertyMarginLeft:
        return layoutObject->isBox() && (!style || !style->marginLeft().isFixed());
    case CSSPropertyMarginRight:
        return layoutObject->isBox() && (!style || !style->marginRight().isFixed());
    case CSSPropertyMarginTop:
        return layoutObject->isBox() && (!style || !style->marginTop().isFixed());
    case CSSPropertyMarginBottom:
        return layoutObject->isBox() && (!style || !style->marginBottom().isFixed());
    case CSSPropertyPadding:
        return layoutObject->isBox()
            && (!style || !style->paddingBottom().isFixed() || !style->paddingTop().isFixed()
                || !style->paddingLeft().isFixed() || !style->paddingRight().isFixed());
    case CSSPropertyPaddingBottom:
        return layoutObject->isBox() && (!style || !style->paddingBottom().isFixed());
    case CSSPropertyPaddingLeft:
        return layoutObject->isBox() && (!style || !style->paddingLeft().isFixed());
    case CSSPropertyPaddingRight:
        return layoutObject->isBox() && (!style || !style->paddingRight().isFixed());
    case CSSPropertyPaddingTop:
        return layoutObject->isBox() && (!style || !style->paddingTop().isFixed());
    case CSSPropertyGridTemplateColumns:
    case CSSPropertyGridTemplateRows:
    case CSSPropertyGridTemplate:
    case CSSPropertyGrid:
        return layoutObject->isLayoutGrid();
    default:
        return false;
    }
}

const ComputedStyle* CSSComputedStyleDeclaration::computeComputedStyle() const
{
    Node* styledNode = this->styledNode();
    ASSERT(styledNode);
    return styledNode->ensureComputedStyle(styledNode->isPseudoElement() ? NOPSEUDO : m_pseudoElementSpecifier);
}

Node* CSSComputedStyleDeclaration::styledNode() const
{
    if (!m_node)
        return nullptr;
    if (m_node->isElementNode()) {
        if (PseudoElement* element = toElement(m_node.get())->pseudoElement(m_pseudoElementSpecifier))
            return element;
    }
    return m_node.get();
}

GCRefPtr<CSSValue> CSSComputedStyleDeclaration::getPropertyCSSValue(AtomicString customPropertyName) const
{
    const ComputedStyle* style = computeComputedStyle();
    if (!style)
        return nullptr;
    return ComputedStyleCSSValueMapping::get(customPropertyName, *style);
}

const HashMap<AtomicString, RefPtr<CSSVariableData>>* CSSComputedStyleDeclaration::getVariables() const
{
    const ComputedStyle* style = computeComputedStyle();
    if (!style)
        return nullptr;
    return ComputedStyleCSSValueMapping::getVariables(*style);
}

GCRefPtr<CSSValue> CSSComputedStyleDeclaration::getPropertyCSSValue(CSSPropertyID propertyID) const
{
    Node* styledNode = this->styledNode();
    if (!styledNode)
        return nullptr;
    LayoutObject* layoutObject = styledNode->layoutObject();
    const ComputedStyle* style;

    Document& document = styledNode->document();

    document.updateLayoutTreeForNodeIfNeeded(styledNode);

    // The style recalc could have caused the styled node to be discarded or replaced
    // if it was a PseudoElement so we need to update it.
    styledNode = this->styledNode();
    layoutObject = styledNode->layoutObject();

    style = computeComputedStyle();

    bool forceFullLayout = isLayoutDependent(propertyID, style, layoutObject)
        || styledNode->isInShadowTree()
        || (document.ownerElement() && document.ensureStyleResolver().hasViewportDependentMediaQueries());

    if (forceFullLayout) {
        document.updateLayoutIgnorePendingStylesheets();
        styledNode = this->styledNode();
        style = computeComputedStyle();
        layoutObject = styledNode->layoutObject();
    }

    if (!style)
        return nullptr;

    GCRefPtr<CSSValue> value = ComputedStyleCSSValueMapping::get(propertyID, *style, layoutObject, styledNode, m_allowVisitedStyle);
    if (value)
        return value;

    logUnimplementedPropertyID(propertyID);
    return nullptr;
}

String CSSComputedStyleDeclaration::getPropertyValue(CSSPropertyID propertyID) const
{
    GCRefPtr<CSSValue> value = getPropertyCSSValue(propertyID);
    if (value)
        return value->cssText();
    return "";
}


unsigned CSSComputedStyleDeclaration::length() const
{
    if (!m_node || !m_node->inActiveDocument())
        return 0;
    return computableProperties().size();
}

String CSSComputedStyleDeclaration::item(unsigned i) const
{
    if (i >= length())
        return "";

    return getPropertyNameString(computableProperties()[i]);
}

bool CSSComputedStyleDeclaration::cssPropertyMatches(CSSPropertyID propertyID, const CSSValue* propertyValue) const
{
    if (propertyID == CSSPropertyFontSize && propertyValue->isPrimitiveValue() && m_node) {
        m_node->document().updateLayoutIgnorePendingStylesheets();
        const ComputedStyle* style = m_node->ensureComputedStyle(m_pseudoElementSpecifier);
        if (style && style->fontDescription().keywordSize()) {
            CSSValueID sizeValue = cssIdentifierForFontSizeKeyword(style->fontDescription().keywordSize());
            const CSSPrimitiveValue* primitiveValue = toCSSPrimitiveValue(propertyValue);
            if (primitiveValue->isValueID() && primitiveValue->getValueID() == sizeValue)
                return true;
        }
    }
    GCRefPtr<CSSValue> value = getPropertyCSSValue(propertyID);
    return value && propertyValue && value->equals(*propertyValue);
}

PassRefPtrWillBeRawPtr<MutableStylePropertySet> CSSComputedStyleDeclaration::copyProperties() const
{
    return copyPropertiesInSet(computableProperties());
}

PassRefPtrWillBeRawPtr<MutableStylePropertySet> CSSComputedStyleDeclaration::copyPropertiesInSet(const Vector<CSSPropertyID>& properties) const
{
    std::vector<CSSProperty> list;
    list.reserve(properties.size());
    for (unsigned i = 0; i < properties.size(); ++i) {
        GCRefPtr<CSSValue> value = getPropertyCSSValue(properties[i]);
        if (value)
            list.emplace_back(CSSProperty(properties[i], value, false));
    }
    return MutableStylePropertySet::create(list.data(), list.size());
}

CSSRule* CSSComputedStyleDeclaration::parentRule() const
{
    return nullptr;
}

String CSSComputedStyleDeclaration::getPropertyValue(const String& propertyName)
{
    CSSPropertyID propertyID = cssPropertyID(propertyName);
    if (!propertyID) {
        if (RuntimeEnabledFeatures::cssVariablesEnabled() && CSSVariableParser::isValidVariableName(propertyName)) {
            GCRefPtr<CSSValue> value = getPropertyCSSValue(AtomicString(propertyName));
            if (value)
                return value->cssText();
        }
        return String();
    }
    ASSERT(CSSPropertyMetadata::isEnabledProperty(propertyID));
    return getPropertyValue(propertyID);
}

String CSSComputedStyleDeclaration::getPropertyPriority(const String&)
{
    // All computed styles have a priority of not "important".
    return "";
}

String CSSComputedStyleDeclaration::getPropertyShorthand(const String&)
{
    return "";
}

bool CSSComputedStyleDeclaration::isPropertyImplicit(const String&)
{
    return false;
}

void CSSComputedStyleDeclaration::setProperty(const String& name, const String&, const String&, ExceptionState& exceptionState)
{
    exceptionState.throwDOMException(NoModificationAllowedError, "These styles are computed, and therefore the '" + name + "' property is read-only.");
}

String CSSComputedStyleDeclaration::removeProperty(const String& name, ExceptionState& exceptionState)
{
    exceptionState.throwDOMException(NoModificationAllowedError, "These styles are computed, and therefore the '" + name + "' property is read-only.");
    return String();
}

GCRefPtr<CSSValue> CSSComputedStyleDeclaration::getPropertyCSSValueInternal(CSSPropertyID propertyID)
{
    return getPropertyCSSValue(propertyID);
}

String CSSComputedStyleDeclaration::getPropertyValueInternal(CSSPropertyID propertyID)
{
    return getPropertyValue(propertyID);
}

void CSSComputedStyleDeclaration::setPropertyInternal(CSSPropertyID id, const String&, const String&, bool, ExceptionState& exceptionState)
{
    // TODO(leviw): This code is currently unreachable, but shouldn't be.
    exceptionState.throwDOMException(NoModificationAllowedError, "These styles are computed, and therefore the '" + getPropertyNameString(id) + "' property is read-only.");
}

DEFINE_TRACE(CSSComputedStyleDeclaration)
{
    visitor->trace(m_node);
    CSSStyleDeclaration::trace(visitor);
}

} // namespace blink
