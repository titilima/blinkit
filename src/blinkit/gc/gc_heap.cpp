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

struct GCObjectHeader {
    GCTable *gcPtr;

    // Flags
    unsigned deleted    : 1;
    unsigned jsRetained : 1;

#ifndef NDEBUG
    const char *name;
#endif

    void* Object(void) { return this + 1; }

    static GCObjectHeader* From(void *p)
    {
        return reinterpret_cast<GCObjectHeader *>(p) - 1;
    }
    template <class T>
    T* To(void)
    {
        return reinterpret_cast<T *>(this->Object());
    }
};

GCHeap::GCHeap(void)
{
    ASSERT(nullptr == theHeap);
    theHeap = this;
}

GCHeap::~GCHeap(void)
{
    ASSERT(this == theHeap);
    CollectGarbage();
    theHeap = nullptr;
}

#ifdef NDEBUG
GCObjectHeader* GCHeap::Alloc(GCObjectType type, size_t totalSize, GCTable *gcPtr)
#else
GCObjectHeader* GCHeap::Alloc(GCObjectType type, size_t totalSize, GCTable *gcPtr, const char *name)
#endif
{
    ASSERT(IsMainThread());
    ASSERT(sizeof(GCObjectHeader) < totalSize);
    if (GCObjectHeader *ret = reinterpret_cast<GCObjectHeader *>(malloc(totalSize)))
    {
        ret->gcPtr = gcPtr;
        ret->deleted    = false;
        ret->jsRetained = false;
#ifndef NDEBUG
        ret->name = name;
#endif
        switch (type)
        {
            case GCObjectType::Root:
                m_rootObjects.insert(ret->Object());
                break;
            case GCObjectType::Stash:
                m_stashObjects.insert(ret->Object());
                break;
            default:
                ASSERT(GCObjectType::Member == type);
                m_memberObjects.insert(ret->Object());
        }
        return ret;
    }
    return nullptr;
}

void GCHeap::CleanupRoots(void)
{
    std::vector<void *> deletedRoots;
    for (void *o : m_rootObjects)
    {
        GCObjectHeader *hdr = GCObjectHeader::From(o);
        if (!hdr->deleted)
            continue;

        deletedRoots.push_back(o);
        free(hdr);
    }

    for (void *o : deletedRoots)
        m_rootObjects.erase(o);
}

void GCHeap::CleanupStashObjects(void)
{
    GCObjectSet objectsToGC;
    for (void *o : m_stashObjects)
    {
        GCObjectHeader *hdr = GCObjectHeader::From(o);
        if (hdr->jsRetained)
            continue;

        objectsToGC.insert(o);
    }
    FreeObjects(objectsToGC, &m_stashObjects);
}

void GCHeap::CollectGarbage(void)
{
    ASSERT(IsMainThread());

#ifndef NDEBUG
    size_t rootCount = m_rootObjects.size();
    size_t memberCount = m_memberObjects.size();
    size_t stashCount = m_stashObjects.size();
#endif

    CleanupRoots();
    CleanupStashObjects();

    GCVisitor visitor(m_memberObjects);
    TraceObjects(m_rootObjects, visitor);
    TraceObjects(m_stashObjects, visitor);
    FreeObjects(visitor.ObjectsToGC(), &m_memberObjects);

#ifndef NDEBUG
    BKLOG(
        "[GC]\n    Roots: %u -> %u\n    Members: %u -> %u\n    Stash Objects: %u -> %u",
        rootCount, m_rootObjects.size(),
        memberCount, m_memberObjects.size(),
        stashCount, m_stashObjects.size()
    );
#endif
}

void GCHeap::FreeObjects(const GCObjectSet &objectsToGC, GCObjectSet *sourcePool)
{
    if (nullptr != sourcePool)
    {
        for (void *o : objectsToGC)
        {
            sourcePool->erase(o);

            GCObjectHeader *hdr = GCObjectHeader::From(o);
            if (nullptr != hdr->gcPtr->Deleter)
                hdr->gcPtr->Deleter(o);
            free(hdr);
        }
    }
    else
    {
        for (void *o : objectsToGC)
        {
            GCObjectHeader *hdr = GCObjectHeader::From(o);
            if (nullptr != hdr->gcPtr->Deleter)
                hdr->gcPtr->Deleter(o);
            free(hdr);
        }
    }
}

void GCHeap::SetObjectFlag(void *p, GCObjectFlag flag, bool b)
{
    GCObjectHeader *hdr = GCObjectHeader::From(p);
    switch (flag)
    {
        case GCObjectFlag::Deleted:
            hdr->deleted = b;
            break;
        case GCObjectFlag::JSRetained:
            hdr->jsRetained = b;
            break;
        default:
            NOTREACHED();
    }
}

void GCHeap::Trace(void *p, blink::Visitor *visitor)
{
    GCObjectHeader::From(p)->gcPtr->Tracer(p, visitor);
}

void GCHeap::TraceObjects(const GCObjectSet &owners, GCVisitor &visitor)
{
    if (visitor.ObjectsToGC().empty())
        return;

    for (void *o : owners)
    {
        Trace(o, &visitor);
        if (visitor.ObjectsToGC().empty())
            return;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GCClearFlag(void *p, GCObjectFlag flag)
{
    GCHeap::SetObjectFlag(p, flag, false);
}

#ifdef NDEBUG
void* GCHeapAlloc(GCObjectType type, size_t size, GCTable *gcPtr)
{
    GCObjectHeader *hdr = theHeap->Alloc(type, size + sizeof(GCObjectHeader), gcPtr);
    return hdr->Object();
}
#else
void* GCHeapAlloc(GCObjectType type, size_t size, GCTable *gcPtr, const char *name)
{
    GCObjectHeader *hdr = theHeap->Alloc(type, size + sizeof(GCObjectHeader), gcPtr, name);
    return hdr->Object();
}
#endif

void GCSetFlag(void *p, GCObjectFlag flag)
{
    GCHeap::SetObjectFlag(p, flag, true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AutoGarbageCollector::~AutoGarbageCollector(void)
{
    theHeap->CollectGarbage();
}

} // namespace BlinKit