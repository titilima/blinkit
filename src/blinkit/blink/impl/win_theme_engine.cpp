// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_theme_engine.cpp
// Description: WinThemeEngine Class
//      Author: Ziming Li
//     Created: 2019-03-15
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "./win_theme_engine.h"

#include <vsstyle.h>
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"
#include "blinkit/ui/animation/animation_frame.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkColorPriv.h"
#include "third_party/zed/include/zed/win/hmodule.hpp"

namespace BlinKit {

static int WINAPI GetMetricsWrapper(int index, UINT)
{
    return GetSystemMetrics(index);
}

static HTHEME WINAPI OpenThemeWrapper(HWND h, PCWSTR classList, UINT)
{
    return OpenThemeData(h, classList);
}

WinThemeEngine::WinThemeEngine(void)
    : m_getMetrics(GetMetricsWrapper)
    , m_paint(&WinThemeEngine::PaintByUser32)
{
    HMODULE user32 = GetModuleHandle(TEXT("User32.dll"));
    zed::hmodule::get_proc_address(user32, "GetSystemMetricsForDpi", m_getMetrics);

    m_uxtheme = LoadLibrary(TEXT("UxTheme.dll"));
    if (nullptr == m_uxtheme)
        return;

    if (!IsThemeActive())
        return;

    m_paint = &WinThemeEngine::PaintByUxTheme;
    if (!zed::hmodule::get_proc_address(m_uxtheme, "OpenThemeDataForDpi", m_openTheme))
        m_openTheme = OpenThemeWrapper;
}

WinThemeEngine::~WinThemeEngine(void)
{
    if (nullptr != m_uxtheme)
        FreeLibrary(m_uxtheme);
}

void WinThemeEngine::Draw(HDC hdc, PCWSTR classList, int partId, int stateId, const RECT &rect, const DTBGOPTS *options)
{
    const UINT dpi = GetDeviceCaps(hdc, LOGPIXELSY);
    ASSERT(dpi == GetDeviceCaps(hdc, LOGPIXELSX));

    HTHEME t = m_openTheme(nullptr, classList, dpi);
    DrawThemeBackgroundEx(t, hdc, partId, stateId, &rect, options);
    CloseThemeData(t);
}

void WinThemeEngine::Draw(HDC hdc, PCWSTR classList, int partId, int stateId, const IntSize &size)
{
    RECT rc;
    rc.left = rc.top = 0;
    rc.right = size.width();
    rc.bottom = size.height();
    Draw(hdc, classList, partId, stateId, rc, nullptr);
}

IntSize WinThemeEngine::GetViewportSize(Part part)
{
    IntSize size;
    switch (part)
    {
        case PartScrollbarDownArrow:
        case PartScrollbarUpArrow:
        case PartScrollbarVerticalThumb:
        case PartScrollbarVerticalTrack:
            size.setWidth(m_getMetrics(SM_CXVSCROLL, 96));
            if (0 == size.width())
                size.setWidth(17);
            size.setHeight(size.width());
            break;
        case PartScrollbarLeftArrow:
        case PartScrollbarRightArrow:
        case PartScrollbarHorizontalThumb:
        case PartScrollbarHorizontalTrack:
            size.setHeight(m_getMetrics(SM_CYHSCROLL, 96));
            if (0 == size.height())
                size.setHeight(17);
            size.setWidth(size.height());
            break;
        case PartCheckbox:
        case PartRadio:
            size.setWidth(13);
            size.setHeight(13);
            break;
        default:
            ASSERT_NOT_REACHED();
    }
    return size;
}

void WinThemeEngine::paint(WebCanvas *canvas, Part part, State state, const IntRect &rect, const ExtraParams *extra)
{
    if (rect.isEmpty())
        return;

    HDC hdc = CreateCompatibleDC(nullptr);

    AnimationFrame frame(rect.size());
    HGDIOBJ oldBitmap = SelectObject(hdc, frame);

    constexpr SkColor placeholder = SkColorSetARGBMacro(1, 0, 0, 0);
    frame.GetCanvas()->clear(placeholder);

    (this->*m_paint)(hdc, part, state, rect, extra);

    SkBitmap &bitmap = frame.GetBitmap();
    // Post-process the pixels to fix up the alpha values (see big comment above).
    const SkPMColor placeholderValue = SkPreMultiplyColor(placeholder);
    const int pixelCount = rect.width() * rect.height();
    SkPMColor *pixels = bitmap.getAddr32(0, 0);
    for (int i = 0; i < pixelCount; ++i)
    {
        if (placeholderValue == pixels[i])
        {
            // Pixel wasn't touched - make it fully transparent.
            pixels[i] = SkPackARGB32(0, 0, 0, 0);
        }
        else if (SkGetPackedA32(pixels[i]) == 0)
        {
            // Pixel was touched but has incorrect alpha of 0, make it fully opaque.
            pixels[i] = SkPackARGB32(0xFF,
                SkGetPackedR32(pixels[i]),
                SkGetPackedG32(pixels[i]),
                SkGetPackedB32(pixels[i]));
        }
    }
    bitmap.setImmutable();
    canvas->drawBitmap(bitmap, static_cast<SkScalar>(rect.x()), static_cast<SkScalar>(rect.y()));

    SelectObject(hdc, oldBitmap);
    DeleteDC(hdc);
}

void WinThemeEngine::PaintButtonByUxTheme(HDC hdc, State state, const IntSize &size, const ButtonExtraParams &extra)
{
    int stateId = -1;
    switch (state)
    {
        case StateNormal:
            stateId = extra.isDefault ? PBS_DEFAULTED : PBS_NORMAL;
            break;
        case StateHover:
            stateId = PBS_HOT;
            break;
        case StatePressed:
            stateId = PBS_PRESSED;
            break;
        case StateDisabled:
            stateId = PBS_DISABLED;
            break;
        default:
            ASSERT_NOT_REACHED();
            return;
    }

    Draw(hdc, VSCLASS_BUTTON, BP_PUSHBUTTON, stateId, size);
}

void WinThemeEngine::PaintByUser32(HDC hdc, Part part, State state, const IntRect &rect, const ExtraParams *extra)
{
    ASSERT(false); // BKTODO:
}

void WinThemeEngine::PaintByUxTheme(HDC hdc, Part part, State state, const IntRect &rect, const ExtraParams *extra)
{
    switch (part)
    {
        case PartScrollbarDownArrow:
        case PartScrollbarLeftArrow:
        case PartScrollbarRightArrow:
        case PartScrollbarUpArrow:
            PaintScrollArrowByUxTheme(hdc, part, state, rect.size(), extra->scrollbarTrack);
            break;
        case PartScrollbarHorizontalThumb:
        case PartScrollbarVerticalThumb:
        case PartScrollbarHorizontalTrack:
        case PartScrollbarVerticalTrack:
            PaintScrollPartByUxTheme(hdc, part, state, rect.size(), extra->scrollbarTrack);
            break;
        case PartScrollbarCorner:
            PaintScrollbarCorner(hdc, rect.size());
            break;
        case PartButton:
            PaintButtonByUxTheme(hdc, state, rect.size(), extra->button);
            break;
        case PartTextField:
            PaintTextFieldByUxTheme(hdc, state, rect.size(), extra->textField);
            break;
        case PartProgressBar:
            PaintProgressBarByUxTheme(hdc, state, rect, extra->progressBar);
            break;
        default:
            ASSERT_NOT_REACHED();
    }
}

void WinThemeEngine::PaintProgressBarByUxTheme(HDC hdc, State state, const IntRect &rect, const ProgressBarExtraParams &extra)
{
    Draw(hdc, VSCLASS_PROGRESS, PP_BAR, 0, rect.size());

    RECT rcValue;
    rcValue.left = extra.valueRectX - rect.x();
    rcValue.top = extra.valueRectY - rect.y();
    rcValue.right = extra.valueRectWidth;
    rcValue.bottom = extra.valueRectHeight;

    if (!extra.determinate)
    {
        ASSERT(false); // BKTODO:
    }

    DTBGOPTS options = { 0 };
    options.dwSize = sizeof(DTBGOPTS);
    if (rect.width() == rcValue.right && rect.x() != rcValue.left)
        options.dwFlags = DTBG_MIRRORDC;
    options.rcClip.right = rect.width();
    options.rcClip.bottom = rect.height();
    Draw(hdc, VSCLASS_PROGRESS, PP_FILL, 0, rcValue, &options);

    // BKTODO: Draw glossy overlay?
}

void WinThemeEngine::PaintScrollArrowByUxTheme(HDC hdc, Part part, State state, const IntSize &size, const ScrollbarTrackExtraParams &extra)
{
    int stateId = -1;

    switch (MAKEWORD(part, state))
    {
        case MAKEWORD(PartScrollbarDownArrow, StateDisabled):
            stateId = ABS_DOWNDISABLED;
            break;
        case MAKEWORD(PartScrollbarDownArrow, StateHover):
            stateId = ABS_DOWNHOT;
            break;
        case MAKEWORD(PartScrollbarDownArrow, StateNormal):
            stateId = ABS_DOWNNORMAL;
            break;
        case MAKEWORD(PartScrollbarDownArrow, StatePressed):
            stateId = ABS_DOWNPRESSED;
            break;
        case MAKEWORD(PartScrollbarLeftArrow, StateDisabled):
            stateId = ABS_LEFTDISABLED;
            break;
        case MAKEWORD(PartScrollbarLeftArrow, StateHover):
            stateId = ABS_LEFTHOT;
            break;
        case MAKEWORD(PartScrollbarLeftArrow, StateNormal):
            stateId = ABS_LEFTNORMAL;
            break;
        case MAKEWORD(PartScrollbarLeftArrow, StatePressed):
            stateId = ABS_LEFTPRESSED;
            break;
        case MAKEWORD(PartScrollbarRightArrow, StateDisabled):
            stateId = ABS_RIGHTDISABLED;
            break;
        case MAKEWORD(PartScrollbarRightArrow, StateHover):
            stateId = ABS_RIGHTHOT;
            break;
        case MAKEWORD(PartScrollbarRightArrow, StateNormal):
            stateId = ABS_RIGHTNORMAL;
            break;
        case MAKEWORD(PartScrollbarRightArrow, StatePressed):
            stateId = ABS_RIGHTPRESSED;
            break;
        case MAKEWORD(PartScrollbarUpArrow, StateDisabled):
            stateId = ABS_UPDISABLED;
            break;
        case MAKEWORD(PartScrollbarUpArrow, StateHover):
            stateId = ABS_UPHOT;
            break;
        case MAKEWORD(PartScrollbarUpArrow, StateNormal):
            stateId = ABS_UPNORMAL;
            break;
        case MAKEWORD(PartScrollbarUpArrow, StatePressed):
            stateId = ABS_UPPRESSED;
            break;
        default:
            ASSERT_NOT_REACHED();
            return;
    }

    Draw(hdc, VSCLASS_SCROLLBAR, SBP_ARROWBTN, stateId, size);
}

void WinThemeEngine::PaintScrollbarCorner(HDC hdc, const IntSize &size)
{
    COLORREF oldColor = SetBkColor(hdc, GetSysColor(COLOR_WINDOW));

    RECT rc = { 0, 0, size.width(), size.height() };
    ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, nullptr, 0, nullptr);

    SetBkColor(hdc, oldColor);
}

void WinThemeEngine::PaintScrollPartByUxTheme(HDC hdc, Part part, State state, const IntSize &size, const ScrollbarTrackExtraParams &extra)
{
    int partId = -1, stateId = -1;
    switch (part)
    {
        case PartScrollbarHorizontalThumb:
            partId = SBP_THUMBBTNHORZ;
            break;
        case PartScrollbarVerticalThumb:
            partId = SBP_THUMBBTNVERT;
            break;
        case PartScrollbarHorizontalTrack:
            partId = extra.isBack ? SBP_UPPERTRACKHORZ : SBP_LOWERTRACKHORZ;
            break;
        case PartScrollbarVerticalTrack:
            partId = extra.isBack ? SBP_UPPERTRACKVERT : SBP_LOWERTRACKVERT;
            break;
        default:
            ASSERT_NOT_REACHED();
            return;
    }

    switch (state)
    {
        case StateDisabled:
            stateId = SCRBS_DISABLED;
            break;
        case StateHover:
            stateId = SCRBS_HOT;
            break;
        case StateNormal:
            stateId = SCRBS_NORMAL;
            break;
        case StatePressed:
            stateId = SCRBS_PRESSED;
            break;
        default:
            ASSERT_NOT_REACHED();
            return;
    }

    Draw(hdc, VSCLASS_SCROLLBAR, partId, stateId, size);
}

void WinThemeEngine::PaintTextFieldByUxTheme(HDC hdc, State state, const IntSize &size, const TextFieldExtraParams &extra)
{
    int stateId = -1;
    switch (state)
    {
        case StateNormal:
            stateId = ETS_NORMAL;
            break;
        case StateFocused:
            stateId = ETS_FOCUSED;
            break;
        case StateHover:
            stateId = ETS_HOT;
            break;
        case StatePressed:
            stateId = ETS_SELECTED;
            break;
        case StateReadonly:
            stateId = ETS_READONLY;
            break;
        case StateDisabled:
            stateId = ETS_DISABLED;
            break;
        default:
            ASSERT_NOT_REACHED();
            return;
    }

    Draw(hdc, VSCLASS_EDIT, EP_BACKGROUNDWITHBORDER, stateId, size);
}

} // namespace BlinKit
