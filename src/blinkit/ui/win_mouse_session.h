#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_mouse_session.h
// Description: WinMouseSession Class
//      Author: Ziming Li
//     Created: 2021-11-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_WIN_MOUSE_SESSION_H
#define BLINKIT_WIN_MOUSE_SESSION_H

#include <functional>
#include "blinkit/blink/public/web/WebInputEvent.h"
#include "blinkit/blink/renderer/platform/geometry/IntPoint.h"

namespace BlinKit {

class MouseEvent;

class WinMouseSession
{
public:
    using Callback = std::function<void(const MouseEvent &)>;
    void Process(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, const Callback &callback);

    void EnableDoubleClick(void) { m_doubleClickEnabled = true; }
    void DisableDoubleClick(void) { m_doubleClickEnabled = false; }
private:
    MouseEvent Prepare(UINT msg, WPARAM wParam, LPARAM lParam);
    blink::WebInputEvent::Type AdjustEventTypeForMouseMove(void);

    bool m_doubleClickEnabled = false;
    bool m_mouseEntered = false;
    double m_lastMouseDownTime;
    blink::IntPoint m_lastPosition, m_lastMouseDownPosition;
};

} // namespace BlinKit

#endif // BLINKIT_WIN_MOUSE_SESSION_H
