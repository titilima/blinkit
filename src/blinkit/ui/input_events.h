#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: input_events.h
// Description: Input Event Classes
//      Author: Ziming Li
//     Created: 2021-11-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_INPUT_EVENTS_H
#define BLINKIT_INPUT_EVENTS_H

#include "blinkit/blink/public/web/WebInputEvent.h"
#include "blinkit/blink/renderer/platform/geometry/int_point.h"

namespace BlinKit {

class InputEvent
{
    STACK_ALLOCATED();
public:
    WebInputEvent::Type GetType(void) const { return m_type; }
    double TimeStampInSeconds(void) const { return m_timeStampSeconds; }
protected:
    InputEvent(WebInputEvent::Type type);

    void Translate(WebInputEvent &dst) const;
private:
    const WebInputEvent::Type m_type;
    const double m_timeStampSeconds;
};

class MouseEvent final : public InputEvent
{
public:
    MouseEvent(WebInputEvent::Type type, WebPointerProperties::Button button, int x, int y);

    const IntPoint& Position(void) const { return m_position; }

    void AdjustMovement(const IntPoint &lastPoint);
    void AdjustClickCount(double lastMouseDownTime, const IntPoint &lastMouseDownPoint);
    WebMouseEvent Translate(void) const;
private:
    const WebPointerProperties::Button m_button;
    const IntPoint m_position;
    int m_movementX = 0, m_movementY = 0;
    int m_clickCount = 0;
};

} // namespace BlinKit

#endif // BLINKIT_INPUT_EVENTS_H
