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

#include <unordered_set>
#include <utility>
#include "blinkit/gc/gc_def.h"

namespace BlinKit {

struct GCObjectHeader;
class GCVisitor;

class GCHeap
{
public:
    GCHeap(void);
    ~GCHeap(void);

#ifdef NDEBUG
    GCObjectHeader* Alloc(GCObjectType type, size_t totalSize, GCTable *gcPtr);
#else
    GCObjectHeader* Alloc(GCObjectType type, size_t totalSize, GCTable *gcPtr, const char *name);
#endif

    void CollectGarbage(void);

    static void SetObjectFlag(void *p, GCObjectFlag flag, bool b);
    static void Trace(void *p, blink::Visitor *visitor);
private:
    using GCObjectSet = std::unordered_set<void *>;
    static void FreeObjects(const GCObjectSet &objectsToGC, GCObjectSet *sourcePool = nullptr);
    static void TraceObjects(const GCObjectSet &owners, GCVisitor &visitor);

    void CleanupRoots(void);
    void CleanupStashObjects(void);
    void CleanupGlobals(void);

    GCObjectSet m_rootObjects, m_memberObjects, m_stashObjects, m_globalObjects;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_HEAP_H
