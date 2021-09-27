// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: mouse_event_session.cpp
// Description: MouseEventSession Class
//      Author: Ziming Li
//     Created: 2021-09-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./mouse_event_session.h"

using namespace blink;

namespace BlinKit {

#if OS_WIN
const double MouseEventSession::DoubleClickInterval = static_cast<double>(GetDoubleClickTime()) / 1000.0;
#endif

void MouseEventSession::PostProcess(const WebMouseEvent &e)
{
    m_lastPosition.setX(e.x);
    m_lastPosition.setY(e.y);
    switch (e.type)
    {
        case WebInputEvent::MouseDown:
            m_lastMouseDownTime = e.timeStampSeconds;
            break;
        case WebInputEvent::MouseLeave:
            m_mouseEntered = false;
            break;
    }
}

void MouseEventSession::PreProcess(WebMouseEvent &e)
{
    e.movementX = m_lastPosition.x() - e.x;
    e.movementY = m_lastPosition.y() - e.y;
    switch (e.type)
    {
        case WebInputEvent::MouseMove:
            if (!m_mouseEntered)
            {
                e.type = WebInputEvent::MouseEnter;
                m_mouseEntered = true;
            }
            break;
        case WebMouseEvent::MouseDown:
            e.clickCount = 1;
            if (!m_hasDoubleClickEvent)
            {
                constexpr int delta = 15;
                if (e.timeStampSeconds - m_lastMouseDownTime < DoubleClickInterval
                    && std::abs(e.movementX) < delta && std::abs(e.movementY) < delta)
                {
                    e.clickCount = 2;
                }
            }
            break;
    }
}

void MouseEventSession::SetLastMouseDownPosition(int x, int y)
{
    m_lastMouseDownPosition.setX(x);
    m_lastMouseDownPosition.setY(y);
}

} // namespace BlinKit
