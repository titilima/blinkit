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
    kCSSDeepCombinatorAndShadow,
    kDeepCombinatorInStaticProfile,
    kDuplicatedAttribute,
    kHasBeforeOrAfterPseudoElement,
    kHasIDClassTagAttribute,
    kHTMLParseErrorNestedForm,
    kPseudoShadowInStaticProfile,
};

} // namespace blink

#endif // BLINKIT_BLINK_WEB_FEATURE_H
