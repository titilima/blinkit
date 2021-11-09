#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: event_target_data_map.h
// Description: EventTargetDataMap Stuff
//      Author: Ziming Li
//     Created: 2021-11-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_EVENT_TARGET_DATA_MAP_H
#define BLINKIT_BLINKIT_EVENT_TARGET_DATA_MAP_H

#include "blinkit/gc/gc_def.h"

namespace blink {

class EventTargetData;
class Node;

using EventTargetDataMap = std::unordered_map<Node *, std::unique_ptr<EventTargetData>>;

} // namespace blink

namespace BlinKit {
template <>
struct GCGlobalWrapper<blink::EventTargetDataMap>
{
    static void IncRef(blink::EventTargetDataMap *)
    {
        ASSERT_NOT_REACHED();
    }
    static void Release(blink::EventTargetDataMap *m)
    {
        ASSERT(m->empty());
        delete m;
    }
};
}

#endif // BLINKIT_BLINKIT_EVENT_TARGET_DATA_MAP_H
