// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: runtime_enabled_features.h
// Description: RuntimeEnabledFeatures Class
//      Author: Ziming Li
//     Created: 2019-10-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_RUNTIME_ENABLED_FEATURES_H
#define BLINKIT_BLINK_RUNTIME_ENABLED_FEATURES_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/allocator.h"

#define BLINKIT_ENABLE_FEATURE(Name)    static constexpr bool Name ## Enabled(void) { return true;  }
#define BLINKIT_DISABLE_FEATURE(Name)   static constexpr bool Name ## Enabled(void) { return false; }

/**
 * Experimental & test features are disabled by default.
 * To enable these features, change the macros below to BLINKIT_ENABLE_FEATURE.
 */
#define BLINK_DEFINE_STABLE_FEATURE         BLINKIT_ENABLE_FEATURE
#define BLINK_DEFINE_EXPERIMENTAL_FEATURE   BLINKIT_DISABLE_FEATURE
#define BLINK_DEFINE_TEST_FEATURE           BLINKIT_DISABLE_FEATURE

namespace blink {

class RuntimeEnabledFeatures final
{
    STATIC_ONLY(RuntimeEnabledFeatures);
public:
    BLINK_DEFINE_STABLE_FEATURE(CallCaptureListenersAtCapturePhaseAtShadowHosts)
    BLINK_DEFINE_STABLE_FEATURE(CSSInBodyDoesNotBlockPaint)
    BLINK_DEFINE_STABLE_FEATURE(TrustedEventsDefaultAction)

    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSFocusVisible)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSMatches)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSPartPseudoElement)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSPseudoIS)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(InvisibleDOM)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(IsolatedCodeCache)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(WorkStealingInScriptRunner)

    BLINKIT_DISABLE_FEATURE(FullscreenUnprefixed)
    BLINKIT_DISABLE_FEATURE(LayoutNG)
    BLINKIT_DISABLE_FEATURE(SlimmingPaintV2)
};

} // namespace blink

#endif // BLINKIT_BLINK_RUNTIME_ENABLED_FEATURES_H
