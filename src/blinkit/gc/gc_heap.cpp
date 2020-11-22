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
    CollectGarbage(GCType::Full);
    ASSERT(m_ownerObjects.empty());
    ASSERT(m_memberObjects.empty());
    ASSERT(m_stashObjects.empty());
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
            case GCObjectType::Owner:
                m_ownerObjects.insert(ret->Object());
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

void GCHeap::CollectGarbage(GCType type)
{
    ASSERT(IsMainThread());

    GCObjectSet objectsToGC;

    for (void *o : m_ownerObjects)
    {
        GCObjectHeader *hdr = GCObjectHeader::From(o);
        if (!hdr->deleted)
            continue;

        objectsToGC.insert(o);
        free(hdr);
    }

    if (!objectsToGC.empty() || GCType::Full == type)
    {
        for (void *o : objectsToGC)
            m_ownerObjects.erase(o);
        objectsToGC.clear();

        GCVisitor visitor(objectsToGC);
        objectsToGC.insert(m_memberObjects.begin(), m_memberObjects.end());
        for (void *o : m_ownerObjects)
            Trace(o, &visitor);

        FreeObjects(objectsToGC, &m_memberObjects);
        objectsToGC.clear();
    }

    for (void *o : m_stashObjects)
    {
        GCObjectHeader *hdr = GCObjectHeader::From(o);
        if (hdr->jsRetained)
            continue;

        objectsToGC.insert(o);
    }
    FreeObjects(objectsToGC, &m_stashObjects);
}

void GCHeap::FreeObjects(const GCObjectSet &objectsToGC, GCObjectSet *sourcePool)
{
    if (nullptr != sourcePool)
    {
        for (void *o : objectsToGC)
        {
            sourcePool->erase(o);

            GCObjectHeader *hdr = GCObjectHeader::From(o);
            if (nullptr != hdr->gcPtr)
                hdr->gcPtr->Deleter(o);
            free(hdr);
        }
    }
    else
    {
        for (void *o : objectsToGC)
        {
            GCObjectHeader *hdr = GCObjectHeader::From(o);
            if (nullptr != hdr->gcPtr)
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
    theHeap->CollectGarbage(m_type);
}

} // namespace BlinKit
