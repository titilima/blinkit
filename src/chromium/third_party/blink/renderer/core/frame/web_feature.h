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
    kAspectRatioFlexItem,
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
    kCSSContainLayoutPositionedDescendants,
    kCSSDeepCombinatorAndShadow,
    kCSSEnvironmentVariable,
    kCSSEnvironmentVariable_SafeAreaInsetBottom,
    kCSSEnvironmentVariable_SafeAreaInsetLeft,
    kCSSEnvironmentVariable_SafeAreaInsetRight,
    kCSSEnvironmentVariable_SafeAreaInsetTop,
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
    kCSSFlexibleBox,
    kCSSGradient,
    kCSSGridLayout,
    kCSSOffsetInEffect,
    kCSSOverflowPaged,
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
    kFlexboxIntrinsicSizeAlgorithmIsDifferent,
    kFlexboxPercentageMarginVertical,
    kFlexboxPercentagePaddingVertical,
    kFontShapingNotDefGlyphObserved,
    kGetComputedStyleWebkitAppearance,
    kHasBeforeOrAfterPseudoElement,
    kHasIDClassTagAttribute,
    kHTMLParseErrorNestedForm,
    kInnerTextWithShadowTree,
    kLineClamp,
    kNegativeBackgroundSize,
    kNegativeMaskSize,
    kPendingStylesheetAddedAfterBodyStarted,
    kPrefixedDevicePixelRatioMediaFeature,
    kPrefixedMaxDevicePixelRatioMediaFeature,
    kPrefixedMinDevicePixelRatioMediaFeature,
    kPrefixedTransform3dMediaFeature,
    kPseudoBeforeAfterForInputElement,
    kPseudoShadowInStaticProfile,
    kQuotedAnimationName,
    kQuotedKeyframesRule,
    kSelectionToStringWithShadowTree,
    kStyleSheetListAnonymousNamedGetter,
    kStyleSheetListNonNullAnonymousNamedGetter,
    kSyntheticKeyframesInCompositedCSSAnimation,
    kTableRowDirectionDifferentFromTable,
    kTableSectionDirectionDifferentFromTable,
    kThreeValuedPositionBackground,
    kUnitlessPerspectiveInTransformProperty,
    kUnitlessZeroAngleFilter,
    kUnitlessZeroAngleGradient,
    kUnitlessZeroAngleTransform,
    kWebkitBoxAlignNotInitial,
    kWebkitBoxChildFlexNotInitial,
    kWebkitBoxChildOrdinalGroupNotInitial,
    kWebkitBoxDirectionNotInitial,
    kWebkitBoxLayout,
    kWebkitBoxLayoutHorizontal,
    kWebkitBoxLayoutVertical,
    kWebkitBoxLineClamp,
    kWebkitBoxLineClampDoesSomething,
    kWebkitBoxLineClampManyChildren,
    kWebkitBoxLineClampOneChild,
    kWebkitBoxLineClampOneChildIsLayoutBlockFlowInline,
    kWebkitBoxLineClampNoChildren,
    kWebkitBoxManyChildren,
    kWebkitBoxNoChildren,
    kWebkitBoxNotDefaultOrder,
    kWebkitBoxOneChild,
    kWebkitBoxOneChildIsLayoutBlockFlowInline,
    kWebkitBoxPackNotInitial,
    kWebkitCrossFade,
    kWindowFindWithShadowTree,

    kNumberOfFeatures
};

} // namespace blink

#endif // BLINKIT_BLINK_WEB_FEATURE_H
