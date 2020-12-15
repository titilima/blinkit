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

struct GCTable {
    void (*Deleter)(void *);
    void (*Tracer)(void *, blink::Visitor *);
};

enum class GCObjectType {
    Member = 0, // Member objects, using tracing to keep alive.
    Root   = 1, // Root objects, using new/delete to manage lifecycle.
    Stash  = 2, // Stash objects, will be finalized while performing GC.
    Global = 3, // Global objects, will be auto finalized in `BkFinalize`.
};

#ifdef NDEBUG
void* GCHeapAlloc(GCObjectType type, size_t size, GCTable *gcPtr);
#else
void* GCHeapAlloc(GCObjectType type, size_t size, GCTable *gcPtr, const char *name);
#endif

enum class GCObjectFlag {
    Deleted = 0, JSRetained
};
void GCSetFlag(const void *p, GCObjectFlag flag);
void GCClearFlag(const void *p, GCObjectFlag flag);

void GCPersist(const void *p);
void GCUnpersist(const void *p);

class AutoGarbageCollector
{
public:
    AutoGarbageCollector(void) = default;
    ~AutoGarbageCollector(void);
};

} // namespace BlinKit

#ifdef NDEBUG
#   define BK_DECLARE_GC_NAME(Class)    \
    typedef int PlaceHolderFor ## Class;
#else
#   define BK_DECLARE_GC_NAME(Class)    \
    static const char* NameForDebug(void) { return # Class; }
#endif

#endif // BLINKIT_BLINKIT_GC_DEF_H
