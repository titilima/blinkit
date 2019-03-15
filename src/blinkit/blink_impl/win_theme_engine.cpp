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

using namespace blink;

namespace BlinKit {

static int WINAPI GetMetricsWrapper(int index, UINT)
{
    return GetSystemMetrics(index);
}

WinThemeEngine::WinThemeEngine(void) : m_getMetrics(GetMetricsWrapper)
{
    HMODULE user32 = GetModuleHandle(TEXT("User32.dll"));
    FARPROC getMetrics = GetProcAddress(user32, "GetSystemMetricsForDpi");
    if (nullptr != getMetrics)
        m_getMetrics = reinterpret_cast<GetMetricsType>(getMetrics);
}

WebSize WinThemeEngine::getSize(Part part)
{
    WebSize size;
    switch (part)
    {
        case PartScrollbarDownArrow:
        case PartScrollbarUpArrow:
        case PartScrollbarVerticalThumb:
        case PartScrollbarVerticalTrack:
            size.width = m_getMetrics(SM_CXVSCROLL, 96);
            if (0 == size.width)
                size.width = 17;
            size.height = size.width;
            break;
        case PartScrollbarLeftArrow:
        case PartScrollbarRightArrow:
        case PartScrollbarHorizontalThumb:
        case PartScrollbarHorizontalTrack:
            size.height = m_getMetrics(SM_CYHSCROLL, 96);
            if (0 == size.height)
                size.height = 17;
            size.width = size.height;
            break;
        case PartCheckbox:
        case PartRadio:
            size.width = size.height = 13;
            break;
        default:
            assert(false); // Not reached!
    }
    return size;
}

void WinThemeEngine::paint(WebCanvas *canvas, Part part, State state, const WebRect &rect, const ExtraParams *extra)
{
    assert(false); // BKTODO:
}

} // namespace BlinKit
