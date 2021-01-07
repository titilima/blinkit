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

#include <windowsx.h>
#include "cc/paint/paint_canvas.h"
#include <vsstyle.h>

#define TEST_USER32_PAINTER 0

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
    : m_hdc(CreateCompatibleDC(nullptr))
    , m_getMetrics(GetMetricsWrapper)
    , m_paint(&WinThemeEngine::PaintByUser32)
{
    HMODULE user32 = GetModuleHandle(TEXT("User32.dll"));
    FARPROC getMetrics = GetProcAddress(user32, "GetSystemMetricsForDpi");
    if (nullptr != getMetrics)
        m_getMetrics = reinterpret_cast<GetMetricsType>(getMetrics);

#if TEST_USER32_PAINTER
    return;
#endif

    m_uxtheme = LoadUxTheme();
    if (nullptr == m_uxtheme)
        return;

    if (!IsThemeActive())
        return;

    m_paint = &WinThemeEngine::PaintByUxTheme;

    m_openTheme = reinterpret_cast<OpenThemeType>(GetProcAddress(m_uxtheme, "OpenThemeDataForDpi"));
    if (nullptr == m_openTheme)
        m_openTheme = OpenThemeWrapper;
}

WinThemeEngine::~WinThemeEngine(void)
{
    if (nullptr != m_oldBitmap)
        SelectBitmap(m_hdc, m_oldBitmap);
    DeleteDC(m_hdc);

    if (nullptr != m_uxtheme)
        FreeLibrary(m_uxtheme);
}

void WinThemeEngine::Draw(HDC hdc, PCWSTR classList, int partId, int stateId, LPCRECT rc)
{
    const UINT dpi = GetDeviceCaps(hdc, LOGPIXELSY);
    ASSERT(dpi == GetDeviceCaps(hdc, LOGPIXELSX));

    HTHEME t = m_openTheme(nullptr, classList, dpi);
    DrawThemeBackground(t, hdc, partId, stateId, rc, rc);
    CloseThemeData(t);
}

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

HMODULE WinThemeEngine::LoadUxTheme(void)
{
    std::wstring path(MAX_PATH, L'\0');
    UINT cch = ::GetSystemDirectoryW(const_cast<PWSTR>(path.c_str()), path.length() + 1);
    if (0 == cch)
    {
        NOTREACHED();
        return nullptr;
    }

    if (path.length() < cch)
    {
        path.resize(cch - 1);
        ::GetSystemDirectoryW(const_cast<PWSTR>(path.c_str()), cch);
    }
    else if (cch < path.length())
    {
        path.resize(cch);
    }

    if (path.back() != L'\\')
        path.push_back(L'\\');
    path.append(L"UxTheme.dll");
    return ::LoadLibraryW(path.c_str());
}

void WinThemeEngine::Paint(cc::PaintCanvas *canvas, Part part, State state, const WebRect &rect, const ExtraParams *extra)
{
    if (rect.IsEmpty())
        return;

    PrepareDC(rect.width, rect.height);
    m_bitmap.erase(SK_ColorTRANSPARENT, SkIRect::MakeWH(rect.width, rect.height));

    RECT rc = { 0, 0, rect.width, rect.height };
    (this->*m_paint)(m_hdc, part, state, &rc, extra);

    cc::PaintImage image = cc::PaintImage::CreateFromBitmap(m_bitmap);
    SkRect src = SkRect::MakeWH(rect.width, rect.height);
    SkRect dst = SkRect::MakeXYWH(rect.x, rect.y, rect.width, rect.height);
    canvas->drawImageRect(image, src, dst, nullptr, cc::PaintCanvas::kFast_SrcRectConstraint);
}

void WinThemeEngine::PaintButtonByUser32(HDC hdc, State state, LPRECT rc, const ButtonExtraParams *extra)
{
    UINT uState = DFCS_BUTTONPUSH;
    switch (state)
    {
        case kStateNormal:
            // BKTODO: stateId = extra->is_default ? PBS_DEFAULTED : PBS_NORMAL;
            break;
        case kStateHover:
            uState |= DFCS_HOT;
            break;
        case kStatePressed:
            uState |= DFCS_PUSHED;
            break;
        case kStateDisabled:
            uState |= DFCS_INACTIVE;
            break;
        default:
            NOTREACHED();
            return;
    }
    DrawFrameControl(hdc, rc, DFC_BUTTON, uState);
}

void WinThemeEngine::PaintButtonByUxTheme(HDC hdc, State state, LPCRECT rc, const ButtonExtraParams *extra)
{
    int stateId = -1;
    switch (state)
    {
        case kStateNormal:
            stateId = extra->is_default ? PBS_DEFAULTED : PBS_NORMAL;
            break;
        case kStateHover:
            stateId = PBS_HOT;
            break;
        case kStatePressed:
            stateId = PBS_PRESSED;
            break;
        case kStateDisabled:
            stateId = PBS_DISABLED;
            break;
        default:
            NOTREACHED();
            return;
    }

    Draw(hdc, VSCLASS_BUTTON, BP_PUSHBUTTON, stateId, rc);
}

void WinThemeEngine::PaintByUser32(HDC hdc, Part part, State state, LPRECT rc, const ExtraParams *extra)
{
    switch (part)
    {
        case kPartButton:
            PaintButtonByUser32(hdc, state, rc, &extra->button);
            break;
        default:
            NOTREACHED();
    }
}

void WinThemeEngine::PaintByUxTheme(HDC hdc, Part part, State state, LPRECT rc, const ExtraParams *extra)
{
    switch (part)
    {
#if 0 // BKTODO:
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
#endif
        case kPartButton:
            PaintButtonByUxTheme(hdc, state, rc, &extra->button);
            break;
#if 0 // BKTODO:
        case PartTextField:
            PaintTextFieldByUxTheme(hdc, state, size, &extra->textField);
            break;
#endif
        default:
            NOTREACHED();
    }
}

#if 0 // BKTODO:
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

void WinThemeEngine::PrepareDC(int width, int height)
{
    do {
        if (m_bitmap.width() < width)
            break;
        if (m_bitmap.height() < height)
            break;
        if (nullptr != m_oldBitmap)
            return;
    } while (false);

    width = std::max(m_bitmap.width(), width);
    height = std::max(m_bitmap.height(), height);

    HBITMAP hBitmap = m_bitmap.InstallDIBSection(width, height, m_hdc);
    HBITMAP hOldBmp = SelectBitmap(m_hdc, hBitmap);
    if (nullptr == m_oldBitmap)
        m_oldBitmap = hOldBmp;
}

} // namespace BlinKit
