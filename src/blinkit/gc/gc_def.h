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

#include <type_traits>
#include "blinkit/blink/renderer/wtf/build_config.h"
#include "blinkit/blink/renderer/wtf/RawPtr.h"

namespace blink {
class Visitor;
}

namespace BlinKit {

class GCObject
{
public:
    virtual ~GCObject(void) = default;
protected:
    GCObject(void) = default;
private:
    template <class> friend class GCMember;
    void IncRef(void);
    unsigned DecRef(void);
    void Release(void);

    unsigned m_refCnt = 0;
};

template <class T>
class ReleasedGCMember
{
public:
    ReleasedGCMember(T *&rawPtr) : m_rawPtr(rawPtr)
    {
        if (nullptr != m_rawPtr)
            rawPtr = nullptr;
    }
    ~ReleasedGCMember(void)
    {
        ASSERT(nullptr == m_rawPtr);
    }
private:
    template <class> friend class GCMember;

    T *m_rawPtr;
};

template <class T>
class GCMember
{
public:
    GCMember(T *ptr = nullptr) : m_rawPtr(ptr)
    {
        if (nullptr != m_rawPtr)
            m_rawPtr->IncRef();
    }
    GCMember(T &rawObj) : m_rawPtr(&rawObj)
    {
        m_rawPtr->IncRef();
    }

    T& operator*() const { return *m_rawPtr; }
    T* operator->() const { return m_rawPtr; }
    operator bool() const { return nullptr != m_rawPtr; }
    bool operator!() const { return nullptr == m_rawPtr; }

    template <typename U>
    GCMember& operator=(const WTF::RawPtr<U> &other)
    {
        if (other)
            static_cast<GCObject *>(other)->IncRef();
        if (nullptr != m_rawPtr)
            m_rawPtr->Release();
        m_rawPtr = other;
        return *this;
    }
    template <typename U>
    GCMember& operator=(ReleasedGCMember<U> &&other)
    {
        if (m_rawPtr != other.m_rawPtr)
        {
            if (nullptr != m_rawPtr)
                m_rawPtr->Release();
            m_rawPtr = other.m_rawPtr;
        }
        else if (nullptr != other.m_rawPtr)
        {
            other.m_rawPtr->DecRef();
        }
        other.m_rawPtr = nullptr;
        return *this;
    }

    T* get(void) const { return m_rawPtr; }
    ReleasedGCMember<T> release(void)
    {
        return ReleasedGCMember(m_rawPtr);
    }
    void clear(void)
    {
        if (nullptr != m_rawPtr)
        {
            m_rawPtr->Release();
            m_rawPtr = nullptr;
        }
    }
private:
    T *m_rawPtr;
};

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
