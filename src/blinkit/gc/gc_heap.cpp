// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_heap.cpp
// Description: GCHeap Class
//      Author: Ziming Li
//     Created: 2020-11-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "gc_heap.h"

#include <cstdlib>
#include "blinkit/gc/gc_visitor.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace BlinKit {

static GCHeap *theHeap = nullptr;

GCHeap::GCHeap(void)
{
    ASSERT(nullptr == theHeap);
    theHeap = this;
}

GCHeap::~GCHeap(void)
{
    ASSERT(this == theHeap);
    ASSERT(m_gcObjects.empty());
    theHeap = nullptr;
}

void GCHeap::Add(GCObjectHeader *obj)
{
    void *p = obj->Object();
    ASSERT(std::end(m_gcObjects) == m_gcObjects.find(p));
    m_gcObjects.insert(p);
}

void GCHeap::AddRootObjectImpl(void *p, Tracer tracer)
{
    ASSERT(IsMainThread());
    auto &rootObjects = theHeap->m_rootObjects;
    ASSERT(std::end(rootObjects) == rootObjects.find(p));
    rootObjects[p] = tracer;
}

#ifdef NDEBUG
GCObjectHeader* GCHeap::Alloc(size_t totalSize, GCTable *gcPtr)
#else
GCObjectHeader* GCHeap::Alloc(size_t totalSize, GCTable *gcPtr, const char *name)
#endif
{
    ASSERT(IsMainThread());
    ASSERT(sizeof(GCObjectHeader) < totalSize);
    if (GCObjectHeader *ret = reinterpret_cast<GCObjectHeader *>(malloc(totalSize)))
    {
        ret->gcPtr = gcPtr;
#ifndef NDEBUG
        ret->name = name;
#endif
        return ret;
    }
    return nullptr;
}

void GCHeap::CollectGarbageForRootObjectImpl(void *p)
{
    ASSERT(IsMainThread());

    auto &rootObjects = theHeap->m_rootObjects;
    auto it = rootObjects.find(p);
    if (std::end(rootObjects) == it)
        return;

    GCVisitor visitor;
    it->second(p, &visitor);
    rootObjects.erase(it);

    visitor.SwitchToSaveMode();
    for (const auto &it : rootObjects)
        it.second(it.first, &visitor);

    ASSERT(false); // BKTODO:
}

#ifdef NDEBUG
void* GCHeapAlloc(size_t size, GCTable *gcPtr)
{
    return theHeap->Alloc(size + sizeof(GCObjectHeader), gcPtr);
}
#else
void* GCHeapAlloc(size_t size, GCTable *gcPtr, const char *name)
{
    return theHeap->Alloc(size + sizeof(GCObjectHeader), gcPtr, name);
}
#endif

} // namespace BlinKit
