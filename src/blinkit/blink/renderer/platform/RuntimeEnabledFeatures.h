#pragma once
// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: RuntimeEnabledFeatures.h
// Description: RuntimeEnabledFeatures Class
//      Author: Ziming Li
//     Created: 2019-01-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_RUNTIME_ENABLED_FEATURES_H
#define BLINKIT_BLINKIT_RUNTIME_ENABLED_FEATURES_H

#include "blinkit/blink/renderer/wtf/Allocator.h"

#define BLINKIT_ENABLE_FEATURE(name)    static constexpr bool name ## Enabled(void) { return true;  }
#define BLINKIT_DISABLE_FEATURE(name)   static constexpr bool name ## Enabled(void) { return false; }

#define BLINK_DEFINE_STABLE_FEATURE BLINKIT_ENABLE_FEATURE

/**
 * Experimental & test features are disabled by default.
 * To enable these features, change the macros below to BLINKIT_ENABLE_FEATURE.
 */
#define BLINK_DEFINE_EXPERIMENTAL_FEATURE   BLINKIT_DISABLE_FEATURE
#define BLINK_DEFINE_TEST_FEATURE           BLINKIT_DISABLE_FEATURE

class RuntimeEnabledFeatures final
{
    STATIC_ONLY(RuntimeEnabledFeatures);
public:
    BLINK_DEFINE_STABLE_FEATURE(alwaysUseComplexText);
    BLINK_DEFINE_STABLE_FEATURE(cssCompositing);
    BLINK_DEFINE_STABLE_FEATURE(cssVariables);
    BLINK_DEFINE_STABLE_FEATURE(eventListenerOptions);
    BLINK_DEFINE_STABLE_FEATURE(imageRenderingPixelated);
    BLINK_DEFINE_STABLE_FEATURE(linkPreconnect);
    BLINK_DEFINE_STABLE_FEATURE(selectionForComposedTree);
    BLINK_DEFINE_STABLE_FEATURE(selectionPaintingWithoutSelectionGaps);
    BLINK_DEFINE_STABLE_FEATURE(styleMatchedPropertiesCache);
    BLINK_DEFINE_STABLE_FEATURE(styleSharing);

    BLINK_DEFINE_EXPERIMENTAL_FEATURE(columnFill);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(compositorWorker);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(contextMenu);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(css3Text);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(css3TextDecorations);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssBackdropFilter);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssContainment);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssFontDisplay);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssFontSizeAdjust);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssGridLayout);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssIndependentTransformProperties);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssMaskSourceType);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssPropertyD);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssTouchActionPanDirections);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssViewport);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(cssomSmoothScroll);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(decodeToYUV);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(fractionalScrollOffsets);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(frameTimingSupport);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(linkPreload);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(renderingPipelineThrottling);    
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(scrollTopLeftInterop);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(stackedCSSPropertyAnimations);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(userSelectAll);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(webAnimationsAPI);
    BLINK_DEFINE_EXPERIMENTAL_FEATURE(webFontsIntervention);

    BLINK_DEFINE_TEST_FEATURE(cssScrollSnapPoints);
    BLINK_DEFINE_TEST_FEATURE(imageOrientation);

    BLINKIT_DISABLE_FEATURE(authorShadowDOMForAnyElement);
    BLINKIT_DISABLE_FEATURE(compositedSelectionUpdate);
    BLINKIT_DISABLE_FEATURE(compositorAnimationTimelines);
    BLINKIT_DISABLE_FEATURE(cssStickyPosition);
    BLINKIT_DISABLE_FEATURE(fastMobileScrolling);
    BLINKIT_DISABLE_FEATURE(langAttributeAwareFormControlUI);
    BLINKIT_DISABLE_FEATURE(overlayScrollbars);
    BLINKIT_DISABLE_FEATURE(pointerEvent);    
    BLINKIT_DISABLE_FEATURE(scrollCustomization);
    BLINKIT_DISABLE_FEATURE(slimmingPaintOffsetCaching);
    BLINKIT_DISABLE_FEATURE(slimmingPaintStrictCullRectClipping);
    BLINKIT_DISABLE_FEATURE(slimmingPaintUnderInvalidationChecking);
    BLINKIT_DISABLE_FEATURE(slimmingPaintV2);
    BLINKIT_DISABLE_FEATURE(touch);
    BLINKIT_DISABLE_FEATURE(trustedEventsDefaultAction);
    BLINKIT_DISABLE_FEATURE(webAnimationsSVG);
};

#endif // BLINKIT_BLINKIT_RUNTIME_ENABLED_FEATURES_H
