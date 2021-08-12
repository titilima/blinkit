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
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/gc/gc_visitor.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

static GCHeap *theHeap = nullptr;

struct GCObjectHeader {
    GCTable *gcPtr;

    // Flags
    unsigned deleted    : 1;
    unsigned jsRetained : 1;

#ifndef NDEBUG
    const char *name;
    GCObjectType type;
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

    CleanupGlobals();

    size_t count = m_rootObjects.size() + m_memberObjects.size();
    for (;;)
    {
        size_t count0 = count;
        CollectGarbage();
        count = m_rootObjects.size() + m_memberObjects.size();
        if (0 == count || count == count0)
            break;
    }
    ASSERT(0 == count);

    theHeap = nullptr;
}

#ifdef NDEBUG
GCObjectHeader* GCHeap::Alloc(GCObjectType type, size_t totalSize, GCTable *gcPtr)
#else
GCObjectHeader* GCHeap::Alloc(GCObjectType type, size_t totalSize, GCTable *gcPtr, const char *name)
#endif
{
    ASSERT(isMainThread());
    ASSERT(sizeof(GCObjectHeader) < totalSize);
    if (GCObjectHeader *ret = reinterpret_cast<GCObjectHeader *>(malloc(totalSize)))
    {
        memset(ret, 0, totalSize);
        ret->gcPtr = gcPtr;
#ifndef NDEBUG
        ret->name = name;
        ret->type = type;
#endif
        switch (type)
        {
            case GCObjectType::Member:
                m_memberObjects.insert(ret->Object());
                break;
            case GCObjectType::Stash:
                m_stashObjects.insert(ret->Object());
                break;
            case GCObjectType::Root:
                m_rootObjects.insert(ret->Object());
                break;
            default:
                ASSERT(GCObjectType::Global == type);
                m_globalObjects.insert(ret->Object());
        }
        return ret;
    }
    return nullptr;
}

void GCHeap::CleanupGlobals(void)
{
    for (void *o : m_globalObjects)
    {
        GCObjectHeader *hdr = GCObjectHeader::From(o);
        hdr->gcPtr->Deleter(o);
        free(hdr);
    }
    m_globalObjects.clear();
}

void GCHeap::CleanupMembers(GCVisitor &visitor)
{
    const GCObjectSet &objectsToGC = visitor.ObjectsToGC();

    for (void **slot : visitor.WeakSlots())
        FlushWeakSlot(slot, objectsToGC);

    for (void *o : objectsToGC)
    {
        m_memberObjects.erase(o);

        GCObjectHeader *hdr = GCObjectHeader::From(o);
        if (nullptr != hdr->gcPtr->Deleter)
            hdr->gcPtr->Deleter(o);
        free(hdr);
    }
}

void GCHeap::CleanupPersistentMembers(void)
{
    auto it = m_persistentMembers.begin();
    while (m_persistentMembers.end() != it)
    {
        if (0 == it->second)
            it = m_persistentMembers.erase(it);
        else
            ++it;
    }
}

void GCHeap::CleanupRoots(void)
{
    auto it = m_rootObjects.begin();
    while (m_rootObjects.end() != it)
    {
        GCObjectHeader *hdr = GCObjectHeader::From(*it);
        if (hdr->deleted)
        {
            free(hdr);
            it = m_rootObjects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void GCHeap::CleanupStashObjects(void)
{
    auto it = m_stashObjects.begin();
    while (m_stashObjects.end() != it)
    {
        void *o = *it;

        GCObjectHeader *hdr = GCObjectHeader::From(o);
        if (!hdr->jsRetained)
        {
            if (nullptr != hdr->gcPtr->Deleter)
                hdr->gcPtr->Deleter(o);
            free(hdr);
            it = m_stashObjects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void GCHeap::CollectGarbage(void)
{
    ASSERT(isMainThread());

#ifndef NDEBUG
    size_t rootCount = m_rootObjects.size();
    size_t memberCount = m_memberObjects.size();
    size_t persistentCount = m_persistentMembers.size();
    size_t stashCount = m_stashObjects.size();
#endif

    CleanupRoots();
    CleanupStashObjects();
    CleanupPersistentMembers();

    GCVisitor visitor(m_memberObjects);
    TraceObjects(m_globalObjects, visitor);
    TraceObjects(m_rootObjects, visitor);
    TracePersistentMembers(visitor);
    TraceObjects(m_stashObjects, visitor);

    CleanupMembers(visitor);

#ifndef NDEBUG
    BKLOG(
        "[GC]\n"
        "    Globals: %u\n"
        "    Roots: %u -> %u\n"
        "    Members: %u -> %u\n"
        "    Persistent Members: %u -> %u\n"
        "    Stash Objects: %u -> %u",
        m_globalObjects.size(),
        rootCount, m_rootObjects.size(),
        memberCount, m_memberObjects.size(),
        persistentCount, m_persistentMembers.size(),
        stashCount, m_stashObjects.size()
    );
#endif
}

void GCHeap::FlushWeakSlot(void **slot, const GCObjectSet &objectsToGC)
{
    void *o = *slot;
    if (nullptr == o)
        return;

    auto it = objectsToGC.find(o);
    if (std::end(objectsToGC) == it)
        return;

    *slot = nullptr;
}

void GCHeap::Persist(void *p)
{
    ASSERT(std::end(m_memberObjects) != m_memberObjects.find(p));
    ++m_persistentMembers[p];
}

void GCHeap::ReleasePersistentObject(GCObject &o)
{
    ASSERT(zed::key_exists(m_persistentObjects, &o));
    if (0 == o.DecRef())
    {
        m_persistentObjects.erase(&o);
        ASSERT(false); // BKTODO: Perform GC.
        delete &o;
    }
}

void GCHeap::RetainPersistentObject(GCObject &o)
{
    if (!zed::key_exists(m_persistentObjects, &o))
    {
        ASSERT(0 == o.m_refCnt);
        m_persistentObjects.insert(&o);
    }
    else
    {
        ASSERT(o.m_refCnt > 0);
    }
    o.IncRef();
}

void GCHeap::SetObjectFlag(const void *p, GCObjectFlag flag, bool b)
{
    GCObjectHeader *hdr = GCObjectHeader::From(const_cast<void *>(p));
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
    const GCObjectSet &objectsToGC = visitor.ObjectsToGC();
    if (objectsToGC.empty())
        return;

    for (void *o : owners)
    {
        Trace(o, &visitor);
        if (objectsToGC.empty())
            return;
    }
}

void GCHeap::TracePersistentMembers(GCVisitor &visitor)
{
    const GCObjectSet &objectsToGC = visitor.ObjectsToGC();
    if (objectsToGC.empty())
        return;

    for (auto &it : m_persistentMembers)
    {
        visitor.trace(it.first);
        if (objectsToGC.empty())
            return;

        Trace(it.first, &visitor);
        if (objectsToGC.empty())
            return;
    }
}

void GCHeap::Unpersist(void *p)
{
    ASSERT(std::end(m_persistentMembers) != m_persistentMembers.find(p));
    ASSERT(m_persistentMembers[p] > 0);
    --m_persistentMembers[p];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GCClearFlag(const void *p, GCObjectFlag flag)
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

void GCPersist(const void *p)
{
    theHeap->Persist(const_cast<void *>(p));
}

void GCReleasePersistentObject(GCObject *o)
{
    if (nullptr != o)
        theHeap->ReleasePersistentObject(*o);
}

void GCRetainPersistentObject(GCObject *o)
{
    if (nullptr != o)
        theHeap->RetainPersistentObject(*o);
}

void GCSetFlag(const void *p, GCObjectFlag flag)
{
    GCHeap::SetObjectFlag(p, flag, true);
}

void GCUnpersist(const void *p)
{
    theHeap->Unpersist(const_cast<void *>(p));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AutoGarbageCollector::~AutoGarbageCollector(void)
{
    theHeap->CollectGarbage();
}

} // namespace BlinKit
