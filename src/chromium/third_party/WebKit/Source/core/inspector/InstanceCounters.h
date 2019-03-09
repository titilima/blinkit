// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: InstanceCounters.h
// Description: Placeholders for Inspector
//      Author: Ziming Li
//     Created: 2018-08-31
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_INSTANCE_COUNTERS_H
#define BLINKIT_BLINK_INSTANCE_COUNTERS_H

#pragma once

#if ENABLE(ASSERT)
#include "wtf/MainThread.h"
#endif

namespace blink {

class InstanceCounters
{
public:
    enum CounterType {
        ActiveDOMObjectCounter,
        AudioHandlerCounter,
        DocumentCounter,
        FrameCounter,
        JSEventListenerCounter,
        LayoutObjectCounter,
        NodeCounter,
        ResourceCounter,
        ScriptPromiseCounter,
        V8PerContextDataCounter,

        // This value must be the last.
        CounterTypeLength,
    };

    static inline void incrementCounter(CounterType) { ASSERT(isMainThread()); }
    static inline void decrementCounter(CounterType) { ASSERT(isMainThread()); }
};

} // namespace blink

#endif // BLINKIT_BLINK_INSTANCE_COUNTERS_H
