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
#include "third_party/zed/include/zed/memory.hpp"

namespace blink {
class Visitor;
}

namespace BlinKit {

/**
 * GCObject
 */

class GCObject
{
public:
    virtual ~GCObject(void) = default;
protected:
    GCObject(void) = default;

    bool IsUnique(void) const
    {
        ASSERT(0 != m_refCnt);
        return 1 == m_refCnt;
    }
private:
    friend class GCHeap;
    template <class> friend class GCMember;

    void IncRef(void);
    unsigned DecRef(void);
    void Release(void);

    unsigned m_refCnt = 0;
};

void GCRetainPersistentObject(GCObject *o);
void GCReleasePersistentObject(GCObject *o);

/**
 * GCRefCounted
 */

class GCRefCounted : public GCObject
{
public:
    bool hasOneRef(void) const { return IsUnique(); }
    void ref(void) { GCRetainPersistentObject(this); }
    void deref(void) { GCReleasePersistentObject(this); }
};

/**
 * GCLeaked
 */

template <class T>
class GCLeaked
{
public:
    GCLeaked(T *&rawPtr) : m_rawPtr(rawPtr)
    {
        if (nullptr != m_rawPtr)
            rawPtr = nullptr;
    }
    ~GCLeaked(void)
    {
        ASSERT(nullptr == m_rawPtr);
    }
private:
    template <class> friend class GCMemberBase;
    template <class> friend class GCMember;

    T *m_rawPtr;
};

template <class T>
GCLeaked<T> WrapLeaked(T *rawPtr)
{
    return GCLeaked<T>(rawPtr);
}

/**
 * Member Stuff
 */

template <class T>
class GCMemberBase : public zed::ptr_base<T>
{
protected:
    GCMemberBase(T *ptr) : zed::ptr_base<T>(ptr) {}
    GCMemberBase(GCLeaked<T> &&o) : zed::ptr_base<T>(o.m_rawPtr)
    {
        o.m_rawPtr = nullptr;
    }

    template <typename U>
    GCMemberBase& operator=(U *other)
    {
        this->m_ptr = other;
        return *this;
    }
};

template <class T>
class GCMember final : public GCMemberBase<T>
{
public:
    GCMember(T *ptr = nullptr) : GCMemberBase<T>(ptr)
    {
        if (nullptr != this->m_ptr)
            this->m_ptr->IncRef();
    }
    GCMember(T &rawObj) : GCMember(&rawObj) {}

    template <typename U>
    GCMember& operator=(const WTF::RawPtr<U> &other)
    {
        if (other)
            static_cast<GCObject *>(other)->IncRef();
        if (nullptr != this->m_ptr)
            this->m_ptr->Release();
        GCMemberBase<T>::operator=(other.get());
        return *this;
    }
    template <typename U>
    GCMember& operator=(GCLeaked<U> &&other)
    {
        if (this->m_ptr != other.m_rawPtr)
        {
            if (nullptr != this->m_ptr)
                this->m_ptr->Release();
            GCMemberBase<T>::operator=(other.m_rawPtr);
        }
        else
        {
            if (nullptr != other.m_rawPtr)
                other.m_rawPtr->DecRef();
        }
        other.m_rawPtr = nullptr;
        return *this;
    }

    GCLeaked<T> release(void)
    {
        return GCLeaked<T>(this->m_ptr);
    }
    void clear(void)
    {
        if (nullptr != this->m_ptr)
        {
            this->m_ptr->Release();
            this->m_ptr = nullptr;
        }
    }
};

template <class T>
class GCPersistentMember final : public GCMemberBase<T>
{
public:
    GCPersistentMember(void) : GCMemberBase<T>(nullptr) {}
    GCPersistentMember(GCLeaked<T> &&o) : GCMemberBase<T>(std::move(o))
    {
        ASSERT(nullptr != this->m_ptr);
        GCRetainPersistentObject(static_cast<GCObject *>(this->m_ptr));
    }
    ~GCPersistentMember(void)
    {
        if (nullptr != this->m_ptr)
            GCReleasePersistentObject(static_cast<GCObject *>(this->m_ptr));
    }

    template <typename U>
    GCPersistentMember& operator=(const WTF::RawPtr<U> &other)
    {
        if (nullptr != this->m_ptr)
            GCReleasePersistentObject(static_cast<GCObject *>(this->m_ptr));
        GCMemberBase<T>::operator=(other.get());
        if (nullptr != this->m_ptr)
            GCRetainPersistentObject(static_cast<GCObject *>(this->m_ptr));
        return *this;
    }
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

template <class T>
using LeakedPtr = BlinKit::GCLeaked<T>;

#ifdef NDEBUG
#   define BK_DECLARE_GC_NAME(Class)    \
    typedef int PlaceHolderFor ## Class;
#else
#   define BK_DECLARE_GC_NAME(Class)    \
    static const char* NameForDebug(void) { return # Class; }
#endif

#endif // BLINKIT_BLINKIT_GC_DEF_H
