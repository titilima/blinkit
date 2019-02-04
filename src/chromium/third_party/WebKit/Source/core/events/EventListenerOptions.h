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

namespace blink {

class EventListenerOptions
{
public:
    bool capture(void) const { return m_capture; }
    void setCapture(bool capture) { m_capture = capture; }

    bool passive(void) const { return m_passive; }
    void setPassive(bool passive) { m_passive = passive; }
private:
    bool m_capture = false, m_passive = false;
};

class EventListenerOptionsOrBoolean
{
};

} // namespace blink

#endif // BLINKIT_BLINK_EVENT_LISTENER_OPTIONS_H
