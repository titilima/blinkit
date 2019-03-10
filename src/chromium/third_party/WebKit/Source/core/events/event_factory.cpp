// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_factory.cpp
// Description: EventFactory Class
//      Author: Ziming Li
//     Created: 2018-08-14
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "EventFactory.h"

#include "core/events/Event.h"

namespace blink {

PassRefPtrWillBeRawPtr<Event> EventFactory::create(const String &type)
{
    assert(false); // BKTODO:
    return nullptr;
}

} // namespace blink
