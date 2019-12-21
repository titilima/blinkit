// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_listener_options.h
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
    virtual ~EventListenerOptions(void) = default;

    bool capture(void) const { return m_capture; }
    void setCapture(bool capture) { m_capture = capture; }
private:
    bool m_capture = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_EVENT_LISTENER_OPTIONS_H
