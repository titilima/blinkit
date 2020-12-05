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

#include "win_theme_engine.h"

#if 0 // BKTODO:
#include <vsstyle.h>
#include <SkCanvas.h>
#include <SkColorPriv.h>
#include "public/platform/WebRect.h"

#include "win/dib_section.h"
#endif

using namespace blink;

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
#if 0 // BKTODO:
    , m_paint(&WinThemeEngine::PaintByUser32)
#endif
{
    HMODULE user32 = GetModuleHandle(TEXT("User32.dll"));
    FARPROC getMetrics = GetProcAddress(user32, "GetSystemMetricsForDpi");
    if (nullptr != getMetrics)
        m_getMetrics = reinterpret_cast<GetMetricsType>(getMetrics);

#if 0 // BKTODO:
    m_uxtheme = LoadLibrary(TEXT("UxTheme.dll"));
    if (nullptr == m_uxtheme)
        return;

    if (!IsThemeActive())
        return;

    m_paint = &WinThemeEngine::PaintByUxTheme;

    m_openTheme = reinterpret_cast<OpenThemeType>(GetProcAddress(m_uxtheme, "OpenThemeDataForDpi"));
    if (nullptr == m_openTheme)
        m_openTheme = OpenThemeWrapper;
#endif
}

WinThemeEngine::~WinThemeEngine(void)
{
#if 0 // BKTODO:
    if (nullptr != m_uxtheme)
        FreeLibrary(m_uxtheme);
#endif
}

#if 0 // BKTODO:
void WinThemeEngine::Draw(HDC hdc, PCWSTR classList, int partId, int stateId, const WebSize &size)
{
    RECT rc;
    rc.left = rc.top = 0;
    rc.right = size.width;
    rc.bottom = size.height;

    const UINT dpi = GetDeviceCaps(hdc, LOGPIXELSY);
    assert(dpi == GetDeviceCaps(hdc, LOGPIXELSX));

    HTHEME t = m_openTheme(nullptr, classList, dpi);
    DrawThemeBackground(t, hdc, partId, stateId, &rc, &rc);
    CloseThemeData(t);
}
#endif

WebSize WinThemeEngine::GetSize(Part part)
{
    WebSize size;
    switch (part)
    {
        case kPartScrollbarDownArrow:
        case kPartScrollbarUpArrow:
        case kPartScrollbarVerticalThumb:
        case kPartScrollbarVerticalTrack:
            size.width = m_getMetrics(SM_CXVSCROLL, 96);
            if (0 == size.width)
                size.width = 17;
            size.height = size.width;
            break;
        case kPartScrollbarLeftArrow:
        case kPartScrollbarRightArrow:
        case kPartScrollbarHorizontalThumb:
        case kPartScrollbarHorizontalTrack:
            size.height = m_getMetrics(SM_CYHSCROLL, 96);
            if (0 == size.height)
                size.height = 17;
            size.width = size.height;
            break;
        case kPartCheckbox:
        case kPartRadio:
            size.width = size.height = 13;
            break;
        default:
            NOTREACHED();
    }
    return size;
}

#if 0 // BKTODO:
void WinThemeEngine::paint(WebCanvas *canvas, Part part, State state, const WebRect &rect, const ExtraParams *extra)
{
    if (0 == rect.width || 0 == rect.height)
        return;

    HDC hdc = CreateCompatibleDC(nullptr);

    DIBSection bitmap(rect.width, rect.height, hdc);
    HGDIOBJ oldBitmap = SelectObject(hdc, bitmap.GetHBITMAP());

    SkCanvas dcCanvas(bitmap);

    const SkColor placeholder = SkColorSetARGB(1, 0, 0, 0);
    dcCanvas.clear(placeholder);

    (this->*m_paint)(hdc, part, state, WebSize(rect.width, rect.height), extra);

    // Post-process the pixels to fix up the alpha values (see big comment above).
    const SkPMColor placeholder_value = SkPreMultiplyColor(placeholder);
    const int pixel_count = rect.width * rect.height;
    SkPMColor *pixels = bitmap.getAddr32(0, 0);
    for (int i = 0; i < pixel_count; ++i)
    {
        if (placeholder_value == pixels[i])
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
    canvas->drawBitmap(bitmap, static_cast<SkScalar>(rect.x), static_cast<SkScalar>(rect.y));

    SelectObject(hdc, oldBitmap);
    DeleteDC(hdc);
}

void WinThemeEngine::PaintButtonByUxTheme(HDC hdc, State state, const WebSize &size, const ButtonExtraParams *extra)
{
    int stateId = -1;
    switch (state)
    {
        case StateNormal:
            stateId = extra->isDefault ? PBS_DEFAULTED : PBS_NORMAL;
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
            assert(false); // Not reached!
            return;
    }

    Draw(hdc, VSCLASS_BUTTON, BP_PUSHBUTTON, stateId, size);
}

void WinThemeEngine::PaintByUser32(HDC hdc, Part part, State state, const WebSize &size, const ExtraParams *extra)
{
    assert(false); // BKTODO:
}

void WinThemeEngine::PaintByUxTheme(HDC hdc, Part part, State state, const WebSize &size, const ExtraParams *extra)
{
    switch (part)
    {
        case PartScrollbarDownArrow:
        case PartScrollbarLeftArrow:
        case PartScrollbarRightArrow:
        case PartScrollbarUpArrow:
            PaintScrollArrowByUxTheme(hdc, part, state, size, &extra->scrollbarTrack);
            break;
        case PartScrollbarHorizontalThumb:
        case PartScrollbarVerticalThumb:
        case PartScrollbarHorizontalTrack:
        case PartScrollbarVerticalTrack:
            PaintScrollPartByUxTheme(hdc, part, state, size, &extra->scrollbarTrack);
            break;
        case PartScrollbarCorner:
            PaintScrollbarCorner(hdc, size);
            break;
        case PartButton:
            PaintButtonByUxTheme(hdc, state, size, &extra->button);
            break;
        case PartTextField:
            PaintTextFieldByUxTheme(hdc, state, size, &extra->textField);
            break;
        default:
            assert(false); // Not reached!
    }
}

void WinThemeEngine::PaintScrollArrowByUxTheme(HDC hdc, Part part, State state, const blink::WebSize &size, const ScrollbarTrackExtraParams *extra)
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
            assert(false); // Not reached!
            return;
    }

    Draw(hdc, VSCLASS_SCROLLBAR, SBP_ARROWBTN, stateId, size);
}

void WinThemeEngine::PaintScrollbarCorner(HDC hdc, const WebSize &size)
{
    COLORREF oldColor = SetBkColor(hdc, GetSysColor(COLOR_WINDOW));

    RECT rc = { 0, 0, size.width, size.height };
    ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, nullptr, 0, nullptr);

    SetBkColor(hdc, oldColor);
}

void WinThemeEngine::PaintScrollPartByUxTheme(HDC hdc, Part part, State state, const WebSize &size, const ScrollbarTrackExtraParams *extra)
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
            partId = extra->isBack ? SBP_UPPERTRACKHORZ : SBP_LOWERTRACKHORZ;
            break;
        case PartScrollbarVerticalTrack:
            partId = extra->isBack ? SBP_UPPERTRACKVERT : SBP_LOWERTRACKVERT;
            break;
        default:
            assert(false); // Not reached!
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
            assert(false); // Not reached!
            return;
    }

    Draw(hdc, VSCLASS_SCROLLBAR, partId, stateId, size);
}

void WinThemeEngine::PaintTextFieldByUxTheme(HDC hdc, State state, const WebSize &size, const TextFieldExtraParams *extra)
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
            assert(false); // Not reached!
            return;
    }

    Draw(hdc, VSCLASS_EDIT, EP_BACKGROUNDWITHBORDER, stateId, size);
}
#endif

} // namespace BlinKit
