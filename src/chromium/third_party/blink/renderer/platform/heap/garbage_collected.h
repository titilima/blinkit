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
#include "third_party/blink/renderer/platform/heap/thread_state.h"

namespace blink {

class GarbageCollectedMixin
{
public:
    virtual void Trace(Visitor *visitor) {}
};

template <typename T>
class GarbageCollectedFinalized : public GarbageCollected<T>
{
    template <class T> friend class GarbageCollected;
protected:
    GarbageCollectedFinalized(void) = default;

    static void FillGCTable(BlinKit::GCTable &gcTable)
    {
        GarbageCollected<T>::FillGCTable(gcTable);
        gcTable.Deleter = Deleter;
    }

    // FinalizeGarbageCollectedObject is called when the object is freed from
    // the heap.  By default finalization means calling the destructor on the
    // object.  FinalizeGarbageCollectedObject can be overridden to support
    // calling the destructor of a subclass.  This is useful for objects without
    // vtables that require explicit dispatching.  The name is intentionally a
    // bit long to make name conflicts less likely.
    void FinalizeGarbageCollectedObject(void) { static_cast<T *>(this)->~T(); }
private:
    static void Deleter(void *ptr)
    {
        reinterpret_cast<T *>(ptr)->FinalizeGarbageCollectedObject();
    }

    DISALLOW_COPY_AND_ASSIGN(GarbageCollectedFinalized);
};

} // namespace blink

#define GC_PLUGIN_IGNORE(bug)
#define USING_GARBAGE_COLLECTED_MIXIN(TYPE)

#endif // BLINKIT_BLINK_GARBAGE_COLLECTED_H
