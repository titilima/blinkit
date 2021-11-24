// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_mouse_session.cpp
// Description: WinMouseSession Class
//      Author: Ziming Li
//     Created: 2021-11-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./win_mouse_session.h"

#include <windowsx.h>
#include "blinkit/ui/input_events.h"
#include "chromium/base/time/time.h"

using namespace blink;

namespace BlinKit {

WebInputEvent::Type WinMouseSession::AdjustEventTypeForMouseMove(void)
{
    if (m_mouseEntered)
        return WebInputEvent::MouseMove;
    m_mouseEntered = true;
    return WebInputEvent::MouseEnter;
}

void WinMouseSession::Process(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, const Callback &callback)
{
    bool trackLeave = !m_mouseEntered;

    if (WM_LBUTTONDOWN == msg)
        SetCapture(hWnd);

    MouseEvent e = Prepare(msg, wParam, lParam);

    callback(e);

    m_lastPosition = e.Position();
    switch (e.GetType())
    {
        case WebInputEvent::MouseDown:
            m_lastMouseDownTime = e.TimeStampInSeconds();
            break;
        case WebInputEvent::MouseLeave:
            m_mouseEntered = false;
            break;
    }

    if (WM_LBUTTONUP == msg)
        ReleaseCapture();

    if (trackLeave)
    {
        TRACKMOUSEEVENT tme = { 0 };
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = hWnd;
        TrackMouseEvent(&tme);
    }
}

MouseEvent WinMouseSession::Prepare(UINT msg, WPARAM wParam, LPARAM lParam)
{
    WebInputEvent::Type type = WebInputEvent::Undefined;
    WebPointerProperties::Button button = WebMouseEvent::ButtonNone;

    UINT flags = wParam;
    switch (msg)
    {
        case WM_LBUTTONDOWN:
            type = WebInputEvent::MouseDown;
            button = WebPointerProperties::ButtonLeft;
            break;
        case WM_LBUTTONUP:
            type = WebInputEvent::MouseUp;
            button = WebPointerProperties::ButtonLeft;
            break;
        case WM_MOUSEMOVE:
            type = AdjustEventTypeForMouseMove();
            if (MK_LBUTTON & flags)
                button = WebPointerProperties::ButtonLeft;
            else if (MK_RBUTTON & flags)
                button = WebPointerProperties::ButtonRight;
            else if (MK_MBUTTON & flags)
                button = WebPointerProperties::ButtonMiddle;
            break;
        case WM_RBUTTONDOWN:
            type = WebInputEvent::MouseDown;
            button = WebPointerProperties::ButtonRight;
            break;
        case WM_RBUTTONUP:
            type = WebInputEvent::MouseUp;
            button = WebPointerProperties::ButtonRight;
            break;
        case WM_MOUSELEAVE:
            type = WebInputEvent::MouseLeave;
            break;
        case WM_MBUTTONDOWN:
            type = WebInputEvent::MouseDown;
            button = WebPointerProperties::ButtonMiddle;
            break;
        case WM_MBUTTONUP:
            type = WebInputEvent::MouseUp;
            button = WebPointerProperties::ButtonMiddle;
            break;
        default:
            NOTREACHED();
    }

    int x = GET_X_LPARAM(lParam);
    int y = GET_Y_LPARAM(lParam);

    MouseEvent ret(type, button, x, y);
    ret.AdjustMovement(m_lastPosition);
    if (WebInputEvent::MouseDown == type)
        ret.AdjustClickCount(m_lastMouseDownTime, m_lastMouseDownPosition);
    return ret;
}

} // namespace BlinKit
