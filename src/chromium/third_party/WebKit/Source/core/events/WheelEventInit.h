// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: WheelEventInit.h
// Description: WheelEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_WHEEL_EVENT_INIT_H
#define BLINKIT_BLINK_WHEEL_EVENT_INIT_H

#pragma once

#include "core/events/MouseEventInit.h"

namespace blink {

class WheelEventInit : public MouseEventInit
{
public:
    bool hasDeltaMode(void) const { return true; }
    unsigned deltaMode(void) const { return m_deltaMode; }
    void setDeltaMode(unsigned value) { m_deltaMode = value; }

    bool hasDeltaX(void) const { return true; }
    double deltaX(void) const { return m_deltaX; }
    void setDeltaX(double value) { m_deltaX = value; }

    bool hasDeltaY(void) const { return true; }
    double deltaY(void) const { return m_deltaY; }
    void setDeltaY(double value) { m_deltaY = value; }

    bool hasDeltaZ(void) const { return true; }
    double deltaZ(void) const { return m_deltaZ; }
    void setDeltaZ(double value) { m_deltaZ = value; }

    bool hasWheelDeltaX(void) const { return true; }
    int wheelDeltaX(void) const { return m_wheelDeltaX; }
    void setWheelDeltaX(int value) { m_wheelDeltaX = value; }

    bool hasWheelDeltaY(void) const { return true; }
    int wheelDeltaY(void) const { return m_wheelDeltaY; }
    void setWheelDeltaY(int value) { m_wheelDeltaY = value; }
private:
    unsigned m_deltaMode = 0;
    double m_deltaX = 0.0;
    double m_deltaY = 0.0;
    double m_deltaZ = 0.0;
    int m_wheelDeltaX = 0;
    int m_wheelDeltaY = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_WHEEL_EVENT_INIT_H
