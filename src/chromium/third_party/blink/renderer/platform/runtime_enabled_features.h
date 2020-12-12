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
    BLINK_DEFINE_STABLE_FEATURE(CSSEnvironmentVariables)
    BLINK_DEFINE_STABLE_FEATURE(CSSHexAlphaColor)
    BLINK_DEFINE_STABLE_FEATURE(CSSInBodyDoesNotBlockPaint)
    BLINK_DEFINE_STABLE_FEATURE(CSSOMSmoothScroll)
    BLINK_DEFINE_STABLE_FEATURE(CSSScrollSnapPoints)
    BLINK_DEFINE_STABLE_FEATURE(HTMLImportsStyleApplication)
    BLINK_DEFINE_STABLE_FEATURE(InputMultipleFieldsUI)
    BLINK_DEFINE_STABLE_FEATURE(PassiveDocumentEventListeners)
    BLINK_DEFINE_STABLE_FEATURE(RenderingPipelineThrottling)
    BLINK_DEFINE_STABLE_FEATURE(RequireCSSExtensionForFile)
    BLINK_DEFINE_STABLE_FEATURE(SmoothScrollJSIntervention)
    BLINK_DEFINE_STABLE_FEATURE(TextUnderlinePositionLeftRight)
    BLINK_DEFINE_STABLE_FEATURE(TrustedEventsDefaultAction)

    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSAdditiveAnimations)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSBackdropFilter)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSFocusVisible)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSFontSizeAdjust)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSIndependentTransformProperties)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSLayoutAPI)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSLogical)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSMaskSourceType)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSMatches)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSOffsetPathRay)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSOffsetPathRayContain)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSOffsetPositionAnchor)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSPaintAPIArguments)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSPartPseudoElement)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSPseudoIS)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSSnapSize)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSVariables2)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSSViewport)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CSS3Text)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CacheStyleSheetWithMediaQueries)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CompositeOpaqueFixedPosition)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(ConstructableStylesheets)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(CustomElementDefaultStyle)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(ExperimentalProductivityFeatures)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(FramesTimingFunction)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(ImplicitRootScroller)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(InvisibleDOM)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(IsolatedCodeCache)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(JankTracking)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(MediaQueryShape)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(StackedCSSPropertyAnimations)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(WebAnimationsAPI)
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(WorkStealingInScriptRunner)

    BLINKIT_DISABLE_FEATURE(BlinkGenPropertyTrees)
    BLINKIT_DISABLE_FEATURE(ClientPlaceholdersForServerLoFi)
    BLINKIT_DISABLE_FEATURE(CompositedSelectionUpdate)
    BLINKIT_DISABLE_FEATURE(DisplayCutoutAPI)
    BLINKIT_DISABLE_FEATURE(ElementTiming)
    BLINKIT_DISABLE_FEATURE(FastMobileScrolling)
    BLINKIT_DISABLE_FEATURE(FontCacheScaling)
    BLINKIT_DISABLE_FEATURE(FractionalScrollOffsets)
    BLINKIT_DISABLE_FEATURE(FullscreenUnprefixed)
    BLINKIT_DISABLE_FEATURE(ImageOrientation)
    BLINKIT_DISABLE_FEATURE(LayoutNG)
    BLINKIT_DISABLE_FEATURE(LayoutNGBlockFragmentation)
    BLINKIT_DISABLE_FEATURE(LayoutNGFieldset)
    BLINKIT_DISABLE_FEATURE(LayoutNGFlexBox)
    BLINKIT_DISABLE_FEATURE(LayoutNGFragmentCaching)
    BLINKIT_DISABLE_FEATURE(LazyImageLoading)
    BLINKIT_DISABLE_FEATURE(NoHoverAfterLayoutChange)
    BLINKIT_DISABLE_FEATURE(NoHoverDuringScroll)
    BLINKIT_DISABLE_FEATURE(OffMainThreadCSSPaint)
    BLINKIT_DISABLE_FEATURE(PaintTouchActionRects)
    BLINKIT_DISABLE_FEATURE(PaintTracking)
    BLINKIT_DISABLE_FEATURE(PaintUnderInvalidationChecking)
    BLINKIT_DISABLE_FEATURE(PassiveDocumentWheelEventListeners)
    BLINKIT_DISABLE_FEATURE(PrintBrowser) // BKTODO: Check if necessary.
    BLINKIT_DISABLE_FEATURE(ScrollCustomization)
    BLINKIT_DISABLE_FEATURE(SlimmingPaintV2)
    BLINKIT_DISABLE_FEATURE(TrackLayoutPassesPerBlock)
    BLINKIT_DISABLE_FEATURE(VisibilityCollapseColumn)
};

} // namespace blink

#endif // BLINKIT_BLINK_RUNTIME_ENABLED_FEATURES_H
