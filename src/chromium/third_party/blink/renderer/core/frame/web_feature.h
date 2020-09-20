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
    kCSSDeepCombinatorAndShadow,
    kDeepCombinatorInStaticProfile,
    kDuplicatedAttribute,
    kFontShapingNotDefGlyphObserved,
    kGetComputedStyleWebkitAppearance,
    kHasBeforeOrAfterPseudoElement,
    kHasIDClassTagAttribute,
    kHTMLParseErrorNestedForm,
    kPrefixedDevicePixelRatioMediaFeature,
    kPrefixedMaxDevicePixelRatioMediaFeature,
    kPrefixedMinDevicePixelRatioMediaFeature,
    kPrefixedTransform3dMediaFeature,
    kPseudoShadowInStaticProfile,
    kQuotedKeyframesRule,
    kSyntheticKeyframesInCompositedCSSAnimation,
};

} // namespace blink

#endif // BLINKIT_BLINK_WEB_FEATURE_H
