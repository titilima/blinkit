// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: EventListenerOptions.h
// Description: EventListenerOptions Classes
//      Author: Ziming Li
//     Created: 2019-02-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EVENT_LISTENER_OPTIONS_H
#define BLINKIT_BLINK_EVENT_LISTENER_OPTIONS_H

#pragma once

#include <optional>

namespace blink {

class EventListenerOptions
{
public:
    bool hasCapture(void) const { return m_capture.has_value(); }
    bool capture(void) const {
        assert(m_capture.has_value());
        return m_capture.value();
    }
    void setCapture(bool capture) { m_capture = capture; }

    bool hasPassive(void) const { return m_passive.has_value(); }
    bool passive(void) const {
        assert(m_passive.has_value());
        return m_passive.value();
    }
    void setPassive(bool passive) { m_passive = passive; }
private:
    std::optional<bool> m_capture, m_passive;
};

class EventListenerOptionsOrBoolean
{
};

} // namespace blink

#endif // BLINKIT_BLINK_EVENT_LISTENER_OPTIONS_H
