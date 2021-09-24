#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Settings.h
// Description: Settings
//      Author: Ziming Li
//     Created: 2020-10-24
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_SETTINGS_H
#define BLINKIT_BLINKIT_SETTINGS_H

#include "blinkit/blink/renderer/core/editing/EditingBehaviorTypes.h"
#include "blinkit/blink/renderer/core/editing/SelectionStrategy.h"
#include "blinkit/blink/public/platform/PointerProperties.h"
#include "blinkit/blink/public/platform/WebDisplayMode.h"
#include "blinkit/blink/renderer/platform/FontFamilyNames.h"
#include "blinkit/blink/renderer/platform/graphics/ImageAnimationPolicy.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

#define BLINKIT_BOOLEAN_SETTING_ITEM(name, value)   static constexpr bool name##(void) { return value;  }

namespace blink {

class Settings
{
    STATIC_ONLY(Settings);
public:
    BLINKIT_BOOLEAN_SETTING_ITEM(acceleratedCompositingEnabled, false); // BKTODO: Support
    BLINKIT_BOOLEAN_SETTING_ITEM(allowCustomScrollbarInMainFrame, true);
    BLINKIT_BOOLEAN_SETTING_ITEM(caretBrowsingEnabled, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(deviceSupportsMouse, true);
    BLINKIT_BOOLEAN_SETTING_ITEM(downloadableBinaryFontsEnabled, true);
    BLINKIT_BOOLEAN_SETTING_ITEM(forceZeroLayoutHeight, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(ignoreMainFrameOverflowHiddenQuirk, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(imagesEnabled, true);
    BLINKIT_BOOLEAN_SETTING_ITEM(inertVisualViewport, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(loadsImagesAutomatically, true);
    BLINKIT_BOOLEAN_SETTING_ITEM(mainFrameClipsContent, true);
    BLINKIT_BOOLEAN_SETTING_ITEM(preferCompositingToLCDTextEnabled, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(reportScreenSizeInPhysicalPixelsQuirk, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(reportWheelOverscroll, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(resizeIsDeviceSizeChange, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(rootLayerScrolls, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(scrollAnimatorEnabled, true);
    BLINKIT_BOOLEAN_SETTING_ITEM(selectionIncludesAltImageText, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(shrinksViewportContentToFit, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(shouldClearDocumentBackground, true);
    BLINKIT_BOOLEAN_SETTING_ITEM(shouldPrintBackgrounds, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(shouldRespectImageOrientation, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(showContextMenuOnMouseUp, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(smartInsertDeleteEnabled, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(textAreasAreResizable, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(textAutosizingEnabled, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(useLegacyBackgroundSizeShorthandBehavior, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(useSolidColorScrollbars, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(usesEncodingDetector, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(viewportEnabled, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(viewportMetaEnabled, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(viewportMetaZeroValuesQuirk, false);
    BLINKIT_BOOLEAN_SETTING_ITEM(viewportMetaMergeContentQuirk, false);

    static constexpr int availableHoverTypes(void) { return HoverTypeNone; }
    static constexpr int availablePointerTypes(void) { return PointerTypeNone; }
    static constexpr int defaultFixedFontSize(void) { return 16; }
    static constexpr int defaultFontSize(void) { return 16; }
    static constexpr int minimumFontSize(void) { return 10; }
    static constexpr int minimumLogicalFontSize(void) { return 10; }
    static constexpr double passwordEchoDurationInSeconds(void) { return 1.0; }

    static constexpr WebDisplayMode displayModeOverride(void) { return WebDisplayModeUndefined; }
    static constexpr ImageAnimationPolicy imageAnimationPolicy(void) { return ImageAnimationPolicyAllowed; }
    static constexpr HoverType primaryHoverType(void) { return HoverTypeNone; }
    static constexpr PointerType primaryPointerType(void) { return PointerTypeNone; }
    static constexpr SelectionStrategy selectionStrategy(void) { return SelectionStrategy::Character; }

    static String defaultTextEncodingName(void) { return String("UTF-8"); }
    static const AtomicString& StandardFontFamilyName(void) { return FontFamilyNames::webkit_standard; }

#if OS_WIN
    static constexpr EditingBehaviorType editingBehaviorType(void) { return EditingWindowsBehavior; }
    BLINKIT_BOOLEAN_SETTING_ITEM(selectTrailingWhitespaceEnabled, true);
#endif
};

} // namespace blink

#endif // BLINKIT_BLINKIT_SETTINGS_H
