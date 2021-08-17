// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/CSSPropertyMetadata.h"

#include "platform/RuntimeEnabledFeatures.h"
#include "wtf/BitArray.h"

namespace blink {

bool CSSPropertyMetadata::isInterpolableProperty(CSSPropertyID property)
{
    switch(property) {
    case CSSPropertyInvalid:
        ASSERT_NOT_REACHED();
        return false;
    case CSSPropertyWebkitMaskBoxImageWidth:
    case CSSPropertyShapeOutside:
    case CSSPropertyTextDecorationColor:
    case CSSPropertyOutlineWidth:
    case CSSPropertyWebkitColumnWidth:
    case CSSPropertyHeight:
    case CSSPropertyBorderImageSource:
    case CSSPropertyPaddingRight:
    case CSSPropertyBorderBottomLeftRadius:
    case CSSPropertyWebkitMaskPositionY:
    case CSSPropertyWebkitMaskPositionX:
    case CSSPropertyShapeMargin:
    case CSSPropertyOutlineColor:
    case CSSPropertyPaddingBottom:
    case CSSPropertyBorderTopRightRadius:
    case CSSPropertyBackgroundImage:
    case CSSPropertyFontWeight:
    case CSSPropertyShapeImageThreshold:
    case CSSPropertyMarginRight:
    case CSSPropertyWebkitColumnRuleColor:
    case CSSPropertyWebkitTextStrokeColor:
    case CSSPropertyOpacity:
    case CSSPropertyWebkitMaskBoxImageOutset:
    case CSSPropertyMotionRotation:
    case CSSPropertyWebkitBorderHorizontalSpacing:
    case CSSPropertyFlexBasis:
    case CSSPropertyWidows:
    case CSSPropertyLetterSpacing:
    case CSSPropertyFlexShrink:
    case CSSPropertyWebkitTransformOriginY:
    case CSSPropertyWebkitTransformOriginX:
    case CSSPropertyTransform:
    case CSSPropertyWordSpacing:
    case CSSPropertyPerspectiveOrigin:
    case CSSPropertyWebkitColumnCount:
    case CSSPropertyBorderBottomRightRadius:
    case CSSPropertyBorderRightWidth:
    case CSSPropertyBorderTopLeftRadius:
    case CSSPropertyBorderLeftWidth:
    case CSSPropertyBorderImageSlice:
    case CSSPropertyBorderTopWidth:
    case CSSPropertyBottom:
    case CSSPropertyTop:
    case CSSPropertyWebkitColumnGap:
    case CSSPropertyTransformOrigin:
    case CSSPropertyMarginTop:
    case CSSPropertyMaxWidth:
    case CSSPropertyTextIndent:
    case CSSPropertyTextShadow:
    case CSSPropertyBorderImageOutset:
    case CSSPropertyWebkitMaskBoxImageSource:
    case CSSPropertyPerspective:
    case CSSPropertyLineHeight:
    case CSSPropertyScale:
    case CSSPropertyPaddingLeft:
    case CSSPropertyVerticalAlign:
    case CSSPropertyBackdropFilter:
    case CSSPropertyBorderRightColor:
    case CSSPropertyLeft:
    case CSSPropertyWidth:
    case CSSPropertyWebkitClipPath:
    case CSSPropertyMarginBottom:
    case CSSPropertyWebkitFilter:
    case CSSPropertyOutlineOffset:
    case CSSPropertyWebkitMaskSize:
    case CSSPropertyRotate:
    case CSSPropertyBorderImageWidth:
    case CSSPropertyFlexGrow:
    case CSSPropertyWebkitMaskImage:
    case CSSPropertyWebkitColumnRuleWidth:
    case CSSPropertyWebkitTransformOriginZ:
    case CSSPropertyRight:
    case CSSPropertyBackgroundSize:
    case CSSPropertyClip:
    case CSSPropertyColor:
    case CSSPropertyWebkitBorderVerticalSpacing:
    case CSSPropertyMarginLeft:
    case CSSPropertyMinHeight:
    case CSSPropertyBorderLeftColor:
    case CSSPropertyMaxHeight:
    case CSSPropertyWebkitMaskBoxImageSlice:
    case CSSPropertyBackgroundPositionY:
    case CSSPropertyBackgroundPositionX:
    case CSSPropertyMinWidth:
    case CSSPropertyBorderBottomWidth:
    case CSSPropertyMotionOffset:
    case CSSPropertyTranslate:
    case CSSPropertyBorderTopColor:
    case CSSPropertyBackgroundColor:
    case CSSPropertyPaddingTop:
    case CSSPropertyBoxShadow:
    case CSSPropertyZIndex:
    case CSSPropertyListStyleImage:
    case CSSPropertyObjectPosition:
    case CSSPropertyOrphans:
    case CSSPropertyWebkitPerspectiveOriginY:
    case CSSPropertyWebkitPerspectiveOriginX:
    case CSSPropertyFontSizeAdjust:
    case CSSPropertyBorderBottomColor:
    case CSSPropertyFontSize:
    case CSSPropertyVisibility:
        return true;
    default:
        return false;
    }
}

bool CSSPropertyMetadata::isInheritedProperty(CSSPropertyID property)
{
    switch(property) {
    case CSSPropertyInvalid:
        ASSERT_NOT_REACHED();
        return false;
    case CSSPropertyWebkitHyphenateCharacter:
    case CSSPropertyFontFamily:
    case CSSPropertyTextRendering:
    case CSSPropertyImageRendering:
    case CSSPropertyWebkitUserSelect:
    case CSSPropertyWebkitTextEmphasisStyle:
    case CSSPropertyQuotes:
    case CSSPropertyWebkitUserModify:
    case CSSPropertyFontKerning:
    case CSSPropertyFontWeight:
    case CSSPropertyWebkitFontSmoothing:
    case CSSPropertyWebkitTextStrokeColor:
    case CSSPropertyTextCombineUpright:
    case CSSPropertyWebkitBorderHorizontalSpacing:
    case CSSPropertyWebkitTapHighlightColor:
    case CSSPropertyCaptionSide:
    case CSSPropertyWebkitPrintColorAdjust:
    case CSSPropertyWidows:
    case CSSPropertyLetterSpacing:
    case CSSPropertyWebkitRubyPosition:
    case CSSPropertyTextJustify:
    case CSSPropertyWordSpacing:
    case CSSPropertyFontStyle:
    case CSSPropertyFontVariant:
    case CSSPropertyWritingMode:
    case CSSPropertyWebkitTextSecurity:
    case CSSPropertyWebkitLineBreak:
    case CSSPropertyWebkitBoxDirection:
    case CSSPropertyBorderCollapse:
    case CSSPropertyWebkitRtlOrdering:
    case CSSPropertyWordWrap:
    case CSSPropertyTextIndent:
    case CSSPropertyTextShadow:
    case CSSPropertyLineHeight:
    case CSSPropertyWebkitTextStrokeWidth:
    case CSSPropertyWebkitTextDecorationsInEffect:
    case CSSPropertyWebkitTextFillColor:
    case CSSPropertyFontStretch:
    case CSSPropertyWhiteSpace:
    case CSSPropertyOverflowWrap:
    case CSSPropertyWebkitLocale:
    case CSSPropertyWebkitTextEmphasisPosition:
    case CSSPropertyTextUnderlinePosition:
    case CSSPropertyFontFeatureSettings:
    case CSSPropertyListStylePosition:
    case CSSPropertyPointerEvents:
    case CSSPropertyFontVariantLigatures:
    case CSSPropertyWebkitTextCombine:
    case CSSPropertyWebkitTextOrientation:
    case CSSPropertyTextTransform:
    case CSSPropertyDirection:
    case CSSPropertyTextOrientation:
    case CSSPropertyColor:
    case CSSPropertyWebkitBorderVerticalSpacing:
    case CSSPropertyWebkitWritingMode:
    case CSSPropertyTextAlignLast:
    case CSSPropertyCursor:
    case CSSPropertyTabSize:
    case CSSPropertyWebkitHighlight:
    case CSSPropertyEmptyCells:
    case CSSPropertyWebkitTextEmphasisColor:
    case CSSPropertyWordBreak:
    case CSSPropertySpeak:
    case CSSPropertyListStyleImage:
    case CSSPropertyListStyleType:
    case CSSPropertyImageOrientation:
    case CSSPropertyTextAlign:
    case CSSPropertyOrphans:
    case CSSPropertyFontSizeAdjust:
    case CSSPropertyFontSize:
    case CSSPropertyVisibility:
    case CSSPropertyVariable:
        return true;
    default:
        return false;
    }
}

// There is one more valid property ID than the total count of CSS properties
// because of custom properties.
static const int numValidPropertyIDs = numCSSProperties + 1;

bool CSSPropertyMetadata::isEnabledProperty(CSSPropertyID unresolvedProperty)
{
    CSSPropertyID property = resolveCSSPropertyID(unresolvedProperty);
    static BitArray<numValidPropertyIDs>* enabledProperties = 0;
    if (!enabledProperties) {
        enabledProperties = new BitArray<numValidPropertyIDs>(true); // All bits sets to 1.
        static_assert(CSSPropertyVariable == 1, "CSSPropertyVariable should directly precede first_enum_value.");
        if (!RuntimeEnabledFeatures::cssVariablesEnabled())
            enabledProperties->clear(0);
        if (!RuntimeEnabledFeatures::cssFontDisplayEnabled())
            enabledProperties->clear(CSSPropertyFontDisplay - 1);
        if (!RuntimeEnabledFeatures::css3TextDecorationsEnabled())
            enabledProperties->clear(CSSPropertyTextDecorationColor - 1);
        if (!RuntimeEnabledFeatures::cssContainmentEnabled())
            enabledProperties->clear(CSSPropertyContain - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridTemplate - 1);
        if (!RuntimeEnabledFeatures::css3TextDecorationsEnabled())
            enabledProperties->clear(CSSPropertyTextDecorationLine - 1);
        if (!RuntimeEnabledFeatures::cssScrollSnapPointsEnabled())
            enabledProperties->clear(CSSPropertyScrollSnapType - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridGap - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridAutoRows - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridColumn - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridTemplateRows - 1);
        if (!RuntimeEnabledFeatures::cssomSmoothScrollEnabled())
            enabledProperties->clear(CSSPropertyScrollBehavior - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridAutoFlow - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyJustifySelf - 1);
        if (!RuntimeEnabledFeatures::css3TextEnabled())
            enabledProperties->clear(CSSPropertyTextJustify - 1);
        if (!RuntimeEnabledFeatures::cssCompositingEnabled())
            enabledProperties->clear(CSSPropertyIsolation - 1);
        if (!RuntimeEnabledFeatures::cssScrollSnapPointsEnabled())
            enabledProperties->clear(CSSPropertyScrollSnapPointsX - 1);
        if (!RuntimeEnabledFeatures::cssScrollSnapPointsEnabled())
            enabledProperties->clear(CSSPropertyScrollSnapPointsY - 1);
        if (!RuntimeEnabledFeatures::columnFillEnabled())
            enabledProperties->clear(CSSPropertyColumnFill - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridTemplateColumns - 1);
        if (!RuntimeEnabledFeatures::cssMaskSourceTypeEnabled())
            enabledProperties->clear(CSSPropertyMaskSourceType - 1);
        if (!RuntimeEnabledFeatures::cssIndependentTransformPropertiesEnabled())
            enabledProperties->clear(CSSPropertyScale - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridAutoColumns - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridRowStart - 1);
        if (!RuntimeEnabledFeatures::cssCompositingEnabled())
            enabledProperties->clear(CSSPropertyMixBlendMode - 1);
        if (!RuntimeEnabledFeatures::cssBackdropFilterEnabled())
            enabledProperties->clear(CSSPropertyBackdropFilter - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridRowGap - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridRowEnd - 1);
        if (!RuntimeEnabledFeatures::css3TextDecorationsEnabled())
            enabledProperties->clear(CSSPropertyTextUnderlinePosition - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridRow - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridColumnStart - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyJustifyItems - 1);
        if (!RuntimeEnabledFeatures::cssIndependentTransformPropertiesEnabled())
            enabledProperties->clear(CSSPropertyRotate - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridTemplateAreas - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGrid - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridColumnEnd - 1);
        if (!RuntimeEnabledFeatures::cssScrollSnapPointsEnabled())
            enabledProperties->clear(CSSPropertyScrollSnapCoordinate - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridArea - 1);
        if (!RuntimeEnabledFeatures::css3TextDecorationsEnabled())
            enabledProperties->clear(CSSPropertyTextDecorationStyle - 1);
        if (!RuntimeEnabledFeatures::cssIndependentTransformPropertiesEnabled())
            enabledProperties->clear(CSSPropertyTranslate - 1);
        if (!RuntimeEnabledFeatures::imageOrientationEnabled())
            enabledProperties->clear(CSSPropertyImageOrientation - 1);
        if (!RuntimeEnabledFeatures::cssGridLayoutEnabled())
            enabledProperties->clear(CSSPropertyGridColumnGap - 1);
        if (!RuntimeEnabledFeatures::cssScrollSnapPointsEnabled())
            enabledProperties->clear(CSSPropertyScrollSnapDestination - 1);
        if (!RuntimeEnabledFeatures::cssFontSizeAdjustEnabled())
            enabledProperties->clear(CSSPropertyFontSizeAdjust - 1);
    }
    return enabledProperties->get(property - 1);
}

void CSSPropertyMetadata::filterEnabledCSSPropertiesIntoVector(const CSSPropertyID* properties, size_t propertyCount, Vector<CSSPropertyID>& outVector)
{
    for (unsigned i = 0; i < propertyCount; i++) {
        CSSPropertyID property = properties[i];
        if (isEnabledProperty(property))
            outVector.append(property);
    }
}

} // namespace blink
