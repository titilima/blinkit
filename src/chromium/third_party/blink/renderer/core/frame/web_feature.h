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
    kCSSLegacyAlignment,
    kCSSOffsetInEffect,
    kCSSOverflowPaged,
    kCSSPaintFunction,
    kCSSResizeAuto,
    kCSSValueAppearanceButton,
    kCSSValueAppearanceButtonForAnchor,
    kCSSValueAppearanceButtonForButton,
    kCSSValueAppearanceButtonForOtherButtons,
    kCSSValueAppearanceButtonRendered,
    kCSSValueAppearanceCaret,
    kCSSValueAppearanceCheckbox,
    kCSSValueAppearanceListbox,
    kCSSValueAppearanceMenulist,
    kCSSValueAppearanceMenulistButton,
    kCSSValueAppearanceMenuListButtonRendered,
    kCSSValueAppearanceNone,
    kCSSValueAppearanceNotNone,
    kCSSValueAppearanceOthers,
    kCSSValueAppearanceRadio,
    kCSSValueAppearanceSearchField,
    kCSSValueAppearanceTextAreaRendered,
    kCSSValueAppearanceTextField,
    kCSSValueAppearanceTextFieldRendered,
    kCSSValueDisplayContents,
    kCSSValuePrefixedFillAvailable,
    kCSSValuePrefixedFitContent,
    kCSSValuePrefixedMaxContent,
    kCSSValuePrefixedMinContent,
    kCSSValueUserModifyReadOnly,
    kCSSValueUserModifyReadWrite,
    kCSSValueUserModifyReadWritePlaintextOnly,
    kCSSZoomNotEqualToOne,
    kCaseInsensitiveAttrSelectorMatch,
    kDeepCombinatorInStaticProfile,
    kDeprecatedWebKitGradient,
    kDeprecatedWebKitLinearGradient,
    kDeprecatedWebKitRadialGradient,
    kDeprecatedWebKitRepeatingLinearGradient,
    kDeprecatedWebKitRepeatingRadialGradient,
    kDuplicatedAttribute,
    kFixedWidthTableDistributionChanged,
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
    kLocalCSSFile,
    kLocalCSSFileExtensionRejected,
    kNegativeBackgroundSize,
    kNegativeMaskSize,
    kPendingStylesheetAddedAfterBodyStarted,
    kPrefixedCursorGrab,
    kPrefixedCursorGrabbing,
    kPrefixedCursorZoomIn,
    kPrefixedCursorZoomOut,
    kPrefixedDevicePixelRatioMediaFeature,
    kPrefixedMaxDevicePixelRatioMediaFeature,
    kPrefixedMinDevicePixelRatioMediaFeature,
    kPrefixedTransform3dMediaFeature,
    kPseudoBeforeAfterForInputElement,
    kPseudoShadowInStaticProfile,
    kQuotedAnimationName,
    kQuotedKeyframesRule,
    kScrollAnchored,
    kScrollbarUseHorizontalScrollbarButton,
    kScrollbarUseHorizontalScrollbarThumb,
    kScrollbarUseHorizontalScrollbarTrack,
    kScrollbarUseVerticalScrollbarButton,
    kScrollbarUseVerticalScrollbarThumb,
    kScrollbarUseVerticalScrollbarTrack,
    kSelectionToStringWithShadowTree,
    kShapeOutsideBorderBox,
    kShapeOutsideContentBox,
    kShapeOutsideContentBoxDifferentFromMarginBox,
    kShapeOutsideMarginBox,
    kShapeOutsidePaddingBox,
    kShapeOutsidePaddingBoxDifferentFromMarginBox,
    kStyleSheetListAnonymousNamedGetter,
    kStyleSheetListNonNullAnonymousNamedGetter,
    kSyntheticKeyframesInCompositedCSSAnimation,
    kTableRowDirectionDifferentFromTable,
    kTableSectionDirectionDifferentFromTable,
    kTextAutosizing,
    kThreeValuedPositionBackground,
    kUnitlessPerspectiveInPerspectiveProperty,
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
