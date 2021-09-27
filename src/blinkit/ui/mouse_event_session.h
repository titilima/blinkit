#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: mouse_event_session.h
// Description: MouseEventSession Class
//      Author: Ziming Li
//     Created: 2021-09-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_MOUSE_EVENT_SESSION_H
#define BLINKIT_BLINKIT_MOUSE_EVENT_SESSION_H

#include "blinkit/blink/public/web/WebInputEvent.h"
#include "blinkit/blink/renderer/platform/geometry/IntPoint.h"

namespace BlinKit {

class MouseEventSession
{
public:
    void PreProcess(blink::WebMouseEvent &e);
    void PostProcess(const blink::WebMouseEvent &e);

    void SetHasDoubleClickEvent(void) { m_hasDoubleClickEvent = true; }

    bool MouseEntered(void) const { return m_mouseEntered; }

    const blink::IntPoint& LastMouseDownPosition(void) const { return m_lastMouseDownPosition; }
    void SetLastMouseDownPosition(int x, int y);
private:
    static const double DoubleClickInterval;

    bool m_hasDoubleClickEvent = false;
    bool m_mouseEntered = false;
    double m_lastMouseDownTime;
    blink::IntPoint m_lastPosition, m_lastMouseDownPosition;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_MOUSE_EVENT_SESSION_H
