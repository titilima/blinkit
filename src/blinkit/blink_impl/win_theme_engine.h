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
#include "blinkit/win/bk_bitmap.h"
#include "third_party/blink/public/platform/web_theme_engine.h"

namespace BlinKit {

class WinThemeEngine final : public blink::WebThemeEngine
{
public:
    WinThemeEngine(void);
    ~WinThemeEngine(void);
private:
    void PrepareDC(int width, int height);

    void PaintByUser32(HDC hdc, Part part, State state, LPRECT rc, const ExtraParams *extra);
    void PaintButtonByUser32(HDC hdc, State state, LPRECT rc, const ButtonExtraParams *extra);

    static HMODULE LoadUxTheme(void);
    void PaintByUxTheme(HDC hdc, Part part, State state, LPRECT rc, const ExtraParams *extra);
#if 0 // BKTODO:
    static void PaintScrollbarCorner(HDC hdc, const blink::WebSize &size);
#endif
    void PaintButtonByUxTheme(HDC hdc, State state, LPCRECT rc, const ButtonExtraParams *extra);
#if 0 // BKTODO:
    void PaintScrollArrowByUxTheme(HDC hdc, Part part, State state, const blink::WebSize &size, const ScrollbarTrackExtraParams *extra);
    void PaintScrollPartByUxTheme(HDC hdc, Part part, State state, const blink::WebSize &size, const ScrollbarTrackExtraParams *extra);
    void PaintTextFieldByUxTheme(HDC hdc, State state, const blink::WebSize &size, const TextFieldExtraParams *extra);
#endif
    void Draw(HDC hdc, PCWSTR classList, int partId, int stateId, LPCRECT rc);

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
    void Paint(cc::PaintCanvas *canvas, Part part, State state, const blink::WebRect &rect,
        const ExtraParams *extra) override;

    HDC m_hdc;
    HBITMAP m_oldBitmap = nullptr;
    BkBitmap m_bitmap;

    typedef int(WINAPI * GetMetricsType)(int, UINT);
    GetMetricsType m_getMetrics;

    HMODULE m_uxtheme = nullptr;
    void(WinThemeEngine::*m_paint)(HDC, Part, State, LPRECT, const ExtraParams *);

    typedef HTHEME(WINAPI * OpenThemeType)(HWND, PCWSTR, UINT);
    OpenThemeType m_openTheme = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_THEME_ENGINE_H
