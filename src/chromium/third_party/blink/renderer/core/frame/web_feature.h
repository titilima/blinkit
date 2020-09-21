// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: web_feature.h
// Description: WebFeature Placeholder
//      Author: Ziming Li
//     Created: 2019-10-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_WEB_FEATURE_H
#define BLINKIT_BLINK_WEB_FEATURE_H

#pragma once

namespace blink {

enum class WebFeature {
    kAnimationSetPlaybackRateCompensatorySeek,
    kBasicShapeEllipseNoRadius,
    kBasicShapeEllipseOneRadius,
    kBasicShapeEllipseTwoRadius,
    kCaseInsensitiveAttrSelectorMatch,
    kCSSAnimationsStackedNeutralKeyframe,
    kCSSAtRuleCharset,
    kCSSAtRuleFontFace,
    kCSSAtRuleImport,
    kCSSAtRuleKeyframes,
    kCSSAtRuleMedia,
    kCSSAtRuleNamespace,
    kCSSAtRulePage,
    kCSSAtRuleSupports,
    kCSSAtRuleViewport,
    kCSSAtRuleWebkitKeyframes,
    kCSSBasicShape,
    kCSSDeepCombinatorAndShadow,
    kCSSFilterBlur,
    kCSSFilterBrightness,
    kCSSFilterContrast,
    kCSSFilterDropShadow,
    kCSSFilterFunctionNoArguments,
    kCSSFilterGrayscale,
    kCSSFilterHueRotate,
    kCSSFilterInvert,
    kCSSFilterOpacity,
    kCSSFilterReference,
    kCSSFilterSaturate,
    kCSSFilterSepia,
    kCSSGradient,
    kCSSOffsetInEffect,
    kCSSPaintFunction,
    kCSSValueAppearanceButton,
    kCSSValueAppearanceCaret,
    kCSSValueAppearanceCheckbox,
    kCSSValueAppearanceListbox,
    kCSSValueAppearanceMenulist,
    kCSSValueAppearanceMenulistButton,
    kCSSValueAppearanceNone,
    kCSSValueAppearanceNotNone,
    kCSSValueAppearanceOthers,
    kCSSValueAppearanceRadio,
    kCSSValueAppearanceSearchField,
    kCSSValueAppearanceTextField,
    kCSSValueDisplayContents,
    kCSSValuePrefixedFillAvailable,
    kCSSValuePrefixedFitContent,
    kCSSValuePrefixedMaxContent,
    kCSSValuePrefixedMinContent,
    kCSSValueUserModifyReadOnly,
    kCSSValueUserModifyReadWrite,
    kCSSValueUserModifyReadWritePlaintextOnly,
    kDeepCombinatorInStaticProfile,
    kDeprecatedWebKitGradient,
    kDeprecatedWebKitLinearGradient,
    kDeprecatedWebKitRadialGradient,
    kDeprecatedWebKitRepeatingLinearGradient,
    kDeprecatedWebKitRepeatingRadialGradient,
    kDuplicatedAttribute,
    kFontShapingNotDefGlyphObserved,
    kGetComputedStyleWebkitAppearance,
    kHasBeforeOrAfterPseudoElement,
    kHasIDClassTagAttribute,
    kHTMLParseErrorNestedForm,
    kNegativeBackgroundSize,
    kNegativeMaskSize,
    kPrefixedDevicePixelRatioMediaFeature,
    kPrefixedMaxDevicePixelRatioMediaFeature,
    kPrefixedMinDevicePixelRatioMediaFeature,
    kPrefixedTransform3dMediaFeature,
    kPseudoShadowInStaticProfile,
    kQuotedAnimationName,
    kQuotedKeyframesRule,
    kSyntheticKeyframesInCompositedCSSAnimation,
    kThreeValuedPositionBackground,
    kUnitlessPerspectiveInTransformProperty,
    kUnitlessZeroAngleFilter,
    kUnitlessZeroAngleGradient,
    kUnitlessZeroAngleTransform,
    kWebkitCrossFade,

    kNumberOfFeatures
};

} // namespace blink

#endif // BLINKIT_BLINK_WEB_FEATURE_H
