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
    ASSERT(m_memberObjects.empty());
    ASSERT(m_rootObjects.empty());
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
#ifndef NDEBUG
        ret->name = name;
#endif
        if (GCObjectType::Root == type)
            m_rootObjects.insert(ret->Object());
        else
            m_memberObjects.insert(ret->Object());
        return ret;
    }
    return nullptr;
}

void GCHeap::FreeRootObject(GCObjectHeader *hdr)
{
    ASSERT(IsMainThread());

    ASSERT(std::end(m_rootObjects) != m_rootObjects.find(hdr->Object()));
    m_rootObjects.erase(hdr->Object());
    free(hdr);

    GCVisitor visitor(m_memberObjects);
    for (void *rootObject : m_rootObjects)
    {
        GCObjectHeader::From(rootObject)->gcPtr->Tracer(rootObject, &visitor);
    }
    for (void *memberObject : visitor.ObjectsToGC())
    {
        m_memberObjects.erase(memberObject);

        hdr = GCObjectHeader::From(memberObject);
        hdr->gcPtr->Deleter(memberObject);
        free(hdr);
    }
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

void GCHeapFreeRootObject(void *p)
{
    theHeap->FreeRootObject(GCObjectHeader::From(p));
}

} // namespace BlinKit
