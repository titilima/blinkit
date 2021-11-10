// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: event_factory.cpp
// Description: EventFactory::create
//      Author: Ziming Li
//     Created: 2021-11-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./event_factory.h"

#include "blinkit/blink/renderer/core/events/Event.h"

namespace blink {

GCRefPtr<Event> EventFactory::create(const String &eventType)
{
    if (equalIgnoringCase(eventType, "Event"))
        return Event::create();
#ifndef NDEBUG
    std::string s = eventType.stdUtf8();
    ASSERT_NOT_REACHED();
#endif
    return nullptr;
}

} // namespace blink
