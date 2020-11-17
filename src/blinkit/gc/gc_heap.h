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
    void FreeRootObject(GCObjectHeader *hdr);
private:
    std::unordered_set<void *> m_rootObjects, m_memberObjects;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_HEAP_H
