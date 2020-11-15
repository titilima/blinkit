// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_heap.h
// Description: GCHeap Class
//      Author: Ziming Li
//     Created: 2020-11-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_HEAP_H
#define BLINKIT_BLINKIT_GC_HEAP_H

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "blinkit/gc/gc_def.h"

namespace BlinKit {

class GCHeap
{
public:
    GCHeap(void);
    ~GCHeap(void);

#ifdef NDEBUG
    GCObjectHeader* Alloc(size_t totalSize, GCTable *gcPtr);
#else
    GCObjectHeader* Alloc(size_t totalSize, GCTable *gcPtr, const char *name);
#endif

    template <class T>
    static void AddRootObject(GCRootImpl<T> &obj)
    {
        AddRootObjectImpl(static_cast<T *>(&obj), &T::Tracer);
    }

    template <class T>
    static void CollectGarbageForRootObject(GCRootImpl<T> &obj)
    {
        CollectGarbageForRootObjectImpl(static_cast<T *>(&obj));
    }
private:
    void Add(GCObjectHeader *obj);

    using Tracer = void (*)(void *, blink::Visitor *);
    static void AddRootObjectImpl(void *p, Tracer tracer);
    static void CollectGarbageForRootObjectImpl(void *p);

    std::unordered_map<void *, Tracer> m_rootObjects;
    std::unordered_set<void *> m_gcObjects;
};

template <class T>
class AutoGarbageCollector
{
public:
    AutoGarbageCollector(GCRootImpl<T> &rootObject) : m_rootObject(rootObject) {}
    ~AutoGarbageCollector(void) { GCHeap::CollectGarbageForRootObject<T>(m_rootObject); }
private:
    T &m_rootObject;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_HEAP_H
