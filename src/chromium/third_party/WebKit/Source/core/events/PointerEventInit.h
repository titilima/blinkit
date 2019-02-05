// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: PointerEventInit.h
// Description: PointerEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_POINTER_EVENT_INIT_H
#define BLINKIT_BLINK_POINTER_EVENT_INIT_H

#pragma once

#include "core/events/MouseEventInit.h"
#include "wtf/text/WTFString.h"

namespace blink {

class PointerEventInit : public MouseEventInit
{
public:
    bool hasHeight(void) const { return true; }
    double height(void) const { return m_height; }
    void setHeight(double value) { m_height = value; }

    bool hasIsPrimary(void) const { return true; }
    bool isPrimary(void) const { return m_isPrimary; }
    void setIsPrimary(bool value) { m_isPrimary = value; }

    bool hasPointerId(void) const { return true; }
    int pointerId(void) const { return m_pointerId; }
    void setPointerId(int value) { m_pointerId = value; }

    bool hasPointerType(void) const { return !m_pointerType.isNull(); }
    const String& pointerType(void) const { return m_pointerType; }
    void setPointerType(const String &value) { m_pointerType = value; }

    bool hasPressure(void) const { return true; }
    float pressure(void) const { return m_pressure; }
    void setPressure(float value) { m_pressure = value; }

    bool hasTiltX(void) const { return true; }
    int tiltX(void) const { return m_tiltX; }
    void setTiltX(int value) { m_tiltX = value; }

    bool hasTiltY(void) const { return true; }
    int tiltY(void) const { return m_tiltY; }
    void setTiltY(int value) { m_tiltY = value; }

    bool hasWidth(void) const { return true; }
    double width(void) const { return m_width; }
    void setWidth(double value) { m_width = value; }
private:
    double m_height = 0.0;
    bool m_isPrimary = false;
    int m_pointerId = 0;
    String m_pointerType{ "" };
    float m_pressure = 0.0;
    int m_tiltX = 0;
    int m_tiltY = 0;
    double m_width = 0.0;
};

} // namespace blink

#endif // BLINKIT_BLINK_POINTER_EVENT_INIT_H
