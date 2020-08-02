// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: garbage_collected.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2019-09-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_GARBAGE_COLLECTED_H
#define BLINKIT_BLINK_GARBAGE_COLLECTED_H

#pragma once

#include "third_party/blink/renderer/platform/heap/heap.h"

namespace blink {

class GarbageCollectedMixin {};

template <typename T>
class GarbageCollectedFinalized : public GarbageCollected<T>
{
protected:
    GarbageCollectedFinalized(void) = default;

    DISALLOW_COPY_AND_ASSIGN(GarbageCollectedFinalized);
};

} // namespace blink

#define GC_PLUGIN_IGNORE(bug)
#define USING_GARBAGE_COLLECTED_MIXIN(TYPE)

#endif // BLINKIT_BLINK_GARBAGE_COLLECTED_H
