// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: instance_counters.h
// Description: Placeholder for InstanceCounters Class
//      Author: Ziming Li
//     Created: 2020-10-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_INSTANCE_COUNTERS_H
#define BLINKIT_BLINK_INSTANCE_COUNTERS_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

class InstanceCounters
{
    STATIC_ONLY(InstanceCounters);
public:
    enum CounterType {
        kLayoutObjectCounter,
        kUACSSResourceCounter
    };
    static inline void IncrementCounter(CounterType type) {}
    static inline void DecrementCounter(CounterType type) {}
};

} // namespace blink

#endif // BLINKIT_BLINK_INSTANCE_COUNTERS_H
