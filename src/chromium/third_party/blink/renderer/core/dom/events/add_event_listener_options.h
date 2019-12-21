// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: add_event_listener_options.h
// Description: RegisteredEventListener Class
//      Author: Ziming Li
//     Created: 2019-12-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_ADD_EVENT_LISTENER_OPTIONS_H
#define BLINKIT_BLINK_ADD_EVENT_LISTENER_OPTIONS_H

#pragma once

#include <optional>
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/dom/events/event_listener_options.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class AddEventListenerOptions : public EventListenerOptions
{
public:
    bool once(void) const { return m_once; }
    void setOnce(bool value) { m_once = value; }

    bool hasPassive(void) const { return m_passive.has_value(); }
    bool passive(void) const
    {
        ASSERT(m_passive.has_value());
        return m_passive.value();
    }
    void setPassive(bool value) { m_passive = value; }
private:
    bool m_once = false;

    std::optional<bool> m_passive;
};

} // namespace blink

#endif // BLINKIT_BLINK_ADD_EVENT_LISTENER_OPTIONS_H
