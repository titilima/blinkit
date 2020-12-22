// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: settings.h
// Description: Settings
//      Author: Ziming Li
//     Created: 2020-10-24
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SETTINGS_H
#define BLINKIT_BLINK_SETTINGS_H

#pragma once

#include "third_party/blink/public/platform/web_viewport_style.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {

class Settings
{
public:
    static constexpr int DefaultFontSize = 16;
    static constexpr int DefaultFixedFontSize = 13;
    static constexpr int MinimumFontSize = 0;
    static constexpr int MinimumLogicalFontSize = 6;

    static const AtomicString& StandardFontFamilyName(void);

    static constexpr bool AcceleratedCompositingEnabled = false;
    static constexpr bool AllowCustomScrollbarInMainFrame = true;
    static constexpr bool ForceZeroLayoutHeight = false;
    static constexpr bool IgnoreMainFrameOverflowHiddenQuirk = false;
    static constexpr bool MainFrameResizesAreOrientationChanges = false;
    static constexpr bool PreferCompositingToLCDTextEnabled = false;
    static constexpr bool ScrollAnimatorEnabled = true;
    static constexpr bool ShouldClearDocumentBackground = true;
    static constexpr bool ShrinksViewportContentToFit = true;
    static constexpr bool TextAutosizingEnabled = false;
    static constexpr bool UseLegacyBackgroundSizeShorthandBehavior = false;
    static constexpr bool ViewportEnabled = false;
    static constexpr bool ViewportMetaEnabled = false;
    static constexpr bool ViewportMetaMergeContentQuirk = false;

    static constexpr WebViewportStyle ViewportStyle = WebViewportStyle::kDefault;
};

} // namespace blink

#endif // BLINKIT_BLINK_SETTINGS_H
