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
#include "third_party/blink/renderer/platform/heap/visitor.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

class Node;

template <typename T>
class GarbageCollected
{
public:
    using ObjectType = BlinKit::GCObjectType;
    void* operator new(size_t size, ObjectType type = ObjectType::Member)
    {
#ifdef NDEBUG
        return BlinKit::GCHeapAlloc(type, size, T::GCPtr());
#else
        return BlinKit::GCHeapAlloc(type, size, T::GCPtr(), T::NameForDebug());
#endif
    }
    void operator delete(void *p)
    {
        BlinKit::GCSetFlag(p, BlinKit::GCObjectFlag::Deleted);
    }
protected:
    GarbageCollected(void) = default;

    static void FillGCTable(BlinKit::GCTable &gcTable)
    {
        gcTable.Tracer = Tracer;
    }
    static BlinKit::GCTable* GCPtr(void)
    {
        static BlinKit::GCTable *s_gcPtr = nullptr;
        if (nullptr == s_gcPtr)
        {
            static BlinKit::GCTable s_gcTable = { 0 };
            T::FillGCTable(s_gcTable);
            ASSERT(nullptr != s_gcTable.Tracer);
            s_gcPtr = &s_gcTable;
        }
        return s_gcPtr;
    }
private:
    static void Tracer(void *ptr, Visitor *visitor)
    {
        reinterpret_cast<T *>(ptr)->Trace(visitor);
    }

    DISALLOW_COPY_AND_ASSIGN(GarbageCollected);
};


} // namespace blink

#endif // BLINKIT_BLINK_HEAP_H
