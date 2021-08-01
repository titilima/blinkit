// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_theme_engine.h
// Description: WinThemeEngine Class
//      Author: Ziming Li
//     Created: 2019-03-15
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_THEME_ENGINE_H
#define BLINKIT_BLINKIT_WIN_THEME_ENGINE_H

#pragma once

#include <Uxtheme.h>
#include "blinkit/blink/public/platform/WebThemeEngine.h"

namespace BlinKit {

class WinThemeEngine final : public blink::WebThemeEngine
{
public:
    WinThemeEngine(void);
    ~WinThemeEngine(void);
private:
    void PaintByUser32(HDC hdc, Part part, State state, const blink::IntSize &size, const ExtraParams *extra);
    void PaintByUxTheme(HDC hdc, Part part, State state, const blink::IntSize &size, const ExtraParams *extra);
    static void PaintScrollbarCorner(HDC hdc, const blink::IntSize &size);
    void PaintButtonByUxTheme(HDC hdc, State state, const blink::IntSize &size, const ButtonExtraParams *extra);
    void PaintScrollArrowByUxTheme(HDC hdc, Part part, State state, const blink::IntSize &size, const ScrollbarTrackExtraParams *extra);
    void PaintScrollPartByUxTheme(HDC hdc, Part part, State state, const blink::IntSize &size, const ScrollbarTrackExtraParams *extra);
    void PaintTextFieldByUxTheme(HDC hdc, State state, const blink::IntSize &size, const TextFieldExtraParams *extra);
    void Draw(HDC hdc, PCWSTR classList, int partId, int stateId, const blink::IntSize &size);

    // blink::WebThemeEngine
    blink::IntSize getSize(Part part) override;
    void paint(blink::WebCanvas *canvas, Part part, State state, const blink::IntRect &rect, const ExtraParams *extra) override;

    using GetMetricsType = decltype(GetSystemMetricsForDpi) *;
    GetMetricsType m_getMetrics;

    HMODULE m_uxtheme;
    void (WinThemeEngine::*m_paint)(HDC, Part, State, const blink::IntSize &, const ExtraParams *);

    using OpenThemeType = decltype(OpenThemeDataForDpi) *;
    OpenThemeType m_openTheme;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_THEME_ENGINE_H
