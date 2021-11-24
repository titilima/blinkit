// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: input_events.cpp
// Description: Input Event Classes
//      Author: Ziming Li
//     Created: 2021-11-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./input_events.h"

#include "chromium/base/time/time.h"

using namespace blink;

namespace BlinKit {

InputEvent::InputEvent(WebInputEvent::Type type) : m_type(type), m_timeStampSeconds(base::Time::Now().ToDoubleT())
{
}

void InputEvent::Translate(WebInputEvent &dst) const
{
    dst.timeStampSeconds = m_timeStampSeconds;
    dst.type = m_type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if OS_WIN
static const double DoubleClickInterval = static_cast<double>(GetDoubleClickTime()) / 1000.0;
#endif

MouseEvent::MouseEvent(WebInputEvent::Type type, WebPointerProperties::Button button, int x, int y)
    : InputEvent(type), m_button(button), m_position(x, y)
{
}

void MouseEvent::AdjustClickCount(double lastMouseDownTime, const IntPoint &lastMouseDownPoint)
{
    constexpr int delta = 15;

    ASSERT(WebInputEvent::MouseDown == GetType());
    if (TimeStampInSeconds() - lastMouseDownTime < DoubleClickInterval
        && std::abs(m_movementX) < delta && std::abs(m_movementY) < delta)
    {
        m_clickCount = 2;
    }
    else
    {
        m_clickCount = 1;
    }
}

void MouseEvent::AdjustMovement(const IntPoint &lastPoint)
{
    m_movementX = lastPoint.x() - m_position.x();
    m_movementY = lastPoint.y() - m_position.y();
}

WebMouseEvent MouseEvent::Translate(void) const
{
    WebMouseEvent ret;
    InputEvent::Translate(ret);

    switch (InputEvent::GetType())
    {
        case WebInputEvent::MouseDown:
        case WebInputEvent::MouseMove:
        {
            switch (m_button)
            {
                case WebPointerProperties::ButtonLeft:
                    ret.modifiers |= WebInputEvent::LeftButtonDown;
                    break;
                case WebPointerProperties::ButtonRight:
                    ret.modifiers |= WebInputEvent::RightButtonDown;
                    break;
                case WebPointerProperties::ButtonMiddle:
                    ret.modifiers |= WebInputEvent::MiddleButtonDown;
                    break;
            }
            break;
        }
    }

    ret.x = ret.windowX = ret.globalX = m_position.x();
    ret.y = ret.windowY = ret.globalY = m_position.y();
    ret.button = m_button;
    ret.movementX = m_movementX;
    ret.movementY = m_movementY;
    ret.clickCount = m_clickCount;
    return ret;
}

} // namespace BlinKit
