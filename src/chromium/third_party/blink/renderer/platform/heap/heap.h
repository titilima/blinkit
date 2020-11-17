// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: heap.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2019-09-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HEAP_H
#define BLINKIT_BLINK_HEAP_H

#pragma once

#include "base/gtest_prod_util.h"
#include "base/macros.h"
#include "blinkit/gc/gc_def.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

class Node;

template <typename T>
class GarbageCollected
{
public:
    void* operator new(size_t size, BlinKit::GCObjectType type = BlinKit::GCObjectType::Member)
    {
#ifdef NDEBUG
        return BlinKit::GCHeapAlloc(type, size, T::GCPtr());
#else
        return BlinKit::GCHeapAlloc(type, size, T::GCPtr(), T::NameForDebug());
#endif
    }
    void operator delete(void *p)
    {
        BlinKit::GCHeapFreeRootObject(p);
    }
protected:
    GarbageCollected(void) = default;

private:
    friend class BlinKit::GCHeap;

    static BlinKit::GCTable* GCPtr(void)
    {
        static BlinKit::GCTable *s_gcPtr = nullptr;
        if (nullptr == s_gcPtr)
        {
            static BlinKit::GCTable s_gcTable = { 0 };
            T::FillGCTable(s_gcTable);
            ASSERT(nullptr != s_gcTable.Deleter);
            ASSERT(nullptr != s_gcTable.Tracer);
            s_gcPtr = &s_gcTable;
        }
        return s_gcPtr;
    }
    static void FillGCTable(BlinKit::GCTable &)
    {
        NOTREACHED();
    }

    DISALLOW_COPY_AND_ASSIGN(GarbageCollected);
};


} // namespace blink

#endif // BLINKIT_BLINK_HEAP_H
