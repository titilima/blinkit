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
#include "third_party/blink/public/platform/web_theme_engine.h"

namespace BlinKit {

class WinThemeEngine final : public blink::WebThemeEngine
{
public:
    WinThemeEngine(void);
    ~WinThemeEngine(void);
private:
#if 0 // BKTODO:
    void PaintByUser32(HDC hdc, Part part, State state, const blink::WebSize &size, const ExtraParams *extra);
    void PaintByUxTheme(HDC hdc, Part part, State state, const blink::WebSize &size, const ExtraParams *extra);
    static void PaintScrollbarCorner(HDC hdc, const blink::WebSize &size);
    void PaintButtonByUxTheme(HDC hdc, State state, const blink::WebSize &size, const ButtonExtraParams *extra);
    void PaintScrollArrowByUxTheme(HDC hdc, Part part, State state, const blink::WebSize &size, const ScrollbarTrackExtraParams *extra);
    void PaintScrollPartByUxTheme(HDC hdc, Part part, State state, const blink::WebSize &size, const ScrollbarTrackExtraParams *extra);
    void PaintTextFieldByUxTheme(HDC hdc, State state, const blink::WebSize &size, const TextFieldExtraParams *extra);
    void Draw(HDC hdc, PCWSTR classList, int partId, int stateId, const blink::WebSize &size);
#endif

    // WebThemeEngine
    blink::WebSize GetSize(Part part) override;
    bool SupportsNinePatch(Part) const override
    {
        ASSERT(false); // BKTODO:
        return false;
    }
    blink::WebSize NinePatchCanvasSize(Part) const override
    {
        ASSERT(false); // BKTODO:
        return blink::WebSize();
    }
    blink::WebRect NinePatchAperture(Part) const override
    {
        ASSERT(false); // BKTODO:
        return blink::WebRect();
    }
    void GetOverlayScrollbarStyle(ScrollbarStyle *style) override
    {
        ASSERT(false); // BKTODO:
    }
    void Paint(cc::PaintCanvas*, Part, State, const blink::WebRect&, const ExtraParams*) override
    {
        ASSERT(false); // BKTODO:
    }

    typedef int(WINAPI * GetMetricsType)(int, UINT);
    GetMetricsType m_getMetrics;

    HMODULE m_uxtheme;
    void(WinThemeEngine::*m_paint)(HDC, Part, State, const blink::WebSize &, const ExtraParams *);

    typedef HTHEME(WINAPI * OpenThemeType)(HWND, PCWSTR, UINT);
    OpenThemeType m_openTheme;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_THEME_ENGINE_H
