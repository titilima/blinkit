// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_def.h
// Description: GC Definitions
//      Author: Ziming Li
//     Created: 2020-11-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_DEF_H
#define BLINKIT_BLINKIT_GC_DEF_H

#pragma once

namespace blink {
class Visitor;
}

namespace BlinKit {

class GCHeap;

struct GCTable {
    void (*Deleter)(void *);
    void (*Tracer)(void *, blink::Visitor *);
};

struct GCObjectHeader {
    GCTable *gcPtr;
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

enum class GCObjectType {
    Member = 0, Root
};

#ifdef NDEBUG
void* GCHeapAlloc(GCObjectType type, size_t size, GCTable *gcPtr);
#else
void* GCHeapAlloc(GCObjectType type, size_t size, GCTable *gcPtr, const char *name);
#endif
void GCHeapFreeRootObject(void *p);

} // namespace BlinKit

#ifdef NDEBUG
#   define BK_DECLARE_GC_NAME(Class)    \
    typedef int PlaceHolderFor ## Class;
#else
#   define BK_DECLARE_GC_NAME(Class)    \
    static const char* NameForDebug(void) { return # Class; }
#endif

#endif // BLINKIT_BLINKIT_GC_DEF_H
