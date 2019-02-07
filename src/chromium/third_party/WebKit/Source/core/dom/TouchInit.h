// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: TouchInit.h
// Description: TouchInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_TOUCH_INIT_H
#define BLINKIT_BLINK_TOUCH_INIT_H

#pragma once

#include <optional>
#include "core/events/EventTarget.h"
#include "platform/heap/Handle.h"

namespace blink {

class TouchInit
{
public:
    int identifier(void) const {
        assert(m_identifier.has_value());
        return m_identifier.value();
    }

    EventTarget* target(void) const { return m_target.get(); }

    double clientX(void) const { return m_clientX; }
    double clientY(void) const { return m_clientY; }

    double screenX(void) const { return m_screenX; }
    double screenY(void) const { return m_screenY; }

    double pageX(void) const { return m_pageX; }
    double pageY(void) const { return m_pageY; }

    float radiusX(void) const { return m_radiusX; }
    float radiusY(void) const { return m_radiusY; }

    float rotationAngle(void) const { return m_rotationAngle; }
    float force(void) const { return m_force; }
private:
    std::optional<int> m_identifier;
    RefPtrWillBeMember<EventTarget> m_target;
    double m_clientX = 0.0, m_clientY = 0.0;
    double m_screenX = 0.0, m_screenY = 0.0;
    double m_pageX = 0.0, m_pageY = 0.0;
    float m_radiusX = 0.0, m_radiusY = 0.0;
    float m_rotationAngle = 0.0;
    float m_force = 0.0;
};

} // namespace blink

#endif // BLINKIT_BLINK_TOUCH_INIT_H
