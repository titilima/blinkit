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

#include <functional>
#include <type_traits>
#include "blinkit/blink/renderer/wtf/build_config.h"
#include "blinkit/blink/renderer/wtf/RawPtr.h"
#include "blinkit/gc/garbage_collector.h"
#include "third_party/zed/include/zed/memory.hpp"

namespace blink {
class Visitor;
}

namespace BlinKit {

#ifndef NDEBUG
bool IsCollectingGarbage(void);
#endif

enum class GCOption { Auto, Full };

/**
 * GCObject
 */

class GCObject
{
public:
    virtual ~GCObject(void);

    enum Category { Default, TreeNode };
    virtual Category GCCategory(void) const { return Default; }
    virtual bool IsRetainedInTree(void) const { return false; }

    bool IsUnique(void) const
    {
        ASSERT(0 != m_refCnt);
        return 1 == m_refCnt;
    }
    void IncRef(void);
    void Release(GCOption option = GCOption::Auto);

    static GCObject* GCCast(GCObject *o)
    {
        return o;
    }

    virtual void trace(blink::Visitor *visitor) = 0;
protected:
    GCObject(void);
private:
    friend class GCCleanupVisitor;
    friend class GCHeap;
    friend class GCRefPtrBase;
    friend class GarbageCollector;

    GCObject(const GCObject &o) = delete;

    unsigned DecRef(void);

    unsigned m_refCnt = 0;
};

/**
 * GCStub
 */

class GCStub
{
public:
    virtual ~GCStub(void) = default;

    static GCObject* GCCast(GCStub *o)
    {
        return nullptr != o ? o->ObjectForGC() : nullptr;
    }

    virtual GCObject* ObjectForGC(void) = 0;
protected:
    GCStub(void) = default;
};

template <class T, class B, typename = typename std::enable_if<std::is_base_of<GCStub, B>::value>::type>
class GCStubImpl : public B
{
public:
    GCObject* ObjectForGC(void) final { return static_cast<T *>(this); }
};

/**
 * Global Stuff
 */

void GCAddGlobalFinalizer(const std::function<void()> &finalizer);

template <class T>
struct GCGlobalWrapper
{
    static void IncRef(T *p) { p->IncRef(); }
    static void Release(T *p) { p->Release(GCOption::Full); }
};

template <class T>
T* GCWrapGlobal(T *p)
{
    GCGlobalWrapper<T>::IncRef(p);
    GCAddGlobalFinalizer(std::bind(&GCGlobalWrapper<T>::Release, p));
    return p;
}

/**
 * Lifecycle Stuff
 */

class GCLifecycleObserver
{
public:
    virtual ~GCLifecycleObserver(void);

    virtual void ObjectFinalized(GCObject *o) = 0;
protected:
    void Observe(GCObject *o);
    void Unobserve(GCObject *o);
};

void GCAttachWeakSlot(GCObject *o, void **weakSlot);
void GCDetachWeakSlot(GCObject *o, void **weakSlot);

/**
 * GCRefPtr Stuff
 */

class GCRefPtrBase
{
    friend class GCCleanupVisitor;
    friend class GCTraceVisitor;
public:
    ~GCRefPtrBase(void)
    {
        if (nullptr != m_object)
            m_object->Release();
    }

    void clear(GCOption option = GCOption::Auto)
    {
        if (nullptr != m_object)
        {
            m_object->Release(option);
            m_object = nullptr;
        }
    }

    operator bool() const { return nullptr != m_object; }
    bool operator!() const { return nullptr == m_object; }

    bool operator==(const GCObject *object) const { return m_object == object; }
    bool operator!=(const GCObject *object) const { return m_object != object; }
protected:
    GCRefPtrBase(GCObject *object) : m_object(object)
    {
        if (nullptr != m_object)
            m_object->IncRef();
    }

    template <class T>
    T* cast_to(void) const
    {
        return static_cast<T *>(m_object);
    }

    template <class T>
    T* release_to(void)
    {
        T *ret = nullptr;
        if (nullptr != m_object)
        {
            m_object->DecRef();
            ret = static_cast<T *>(m_object);
            m_object = nullptr;
        }
        return ret;
    }

    template <class T>
    void reset_from(T *p)
    {
        GCObject *object = p;

        if (m_object == object)
            return;

        if (nullptr != m_object)
            m_object->Release();
        m_object = object;
        if (nullptr != m_object)
            m_object->IncRef();
    }
private:
    GCObject *m_object;
};

class GCGuard final : private GCRefPtrBase
{
public:
    GCGuard(GCObject &o) : GCRefPtrBase(&o) {}
    GCGuard(GCStub *stub) : GCRefPtrBase(stub->ObjectForGC()) {}
};

template <class T>
class GCRefPtr final : public GCRefPtrBase
{
public:
    explicit GCRefPtr(T *p = nullptr) : GCRefPtrBase(p) {}
    GCRefPtr(std::nullptr_t) : GCRefPtrBase(nullptr) {}

    template <class U>
    GCRefPtr(const WTF::RawPtr<U> &ptr) : GCRefPtr(ptr.get()) {}

    GCRefPtr(const GCRefPtr &o) : GCRefPtr(o.get()) {}
    template <class U>
    GCRefPtr(const GCRefPtr<U> &o) : GCRefPtr(o.get()) {}

    T* get(void) const { return GCRefPtrBase::cast_to<T>(); }

    T& operator*() const { return *get(); }
    T* operator->() const { return get(); }

    template <class U>
    GCRefPtr& operator=(U *p)
    {
        GCRefPtrBase::reset_from<T>(p);
        return *this;
    }

    template <class U>
    GCRefPtr& operator=(const WTF::RawPtr<U> &ptr)
    {
        GCRefPtrBase::reset_from<T>(ptr.get());
        return *this;
    }

    GCRefPtr& operator=(const GCRefPtr &o)
    {
        GCRefPtrBase::reset_from<T>(o.get());
        return *this;
    }
    template <class U>
    GCRefPtr& operator=(const GCRefPtr<U> &o)
    {
        GCRefPtrBase::reset_from<T>(o.get());
        return *this;
    }

    T* release(void) { return GCRefPtrBase::release_to<T>(); }
};

template <class T>
GCRefPtr<T> GCWrapShared(T *p)
{
    return GCRefPtr<T>(p);
}

/**
 * GCUniquePtr Stuff
 */

template <class T>
struct GCDeleter final : public std::default_delete<T>
{
    void operator()(T *p) const
    {
        GarbageCollector::PerformOnRoot(*p);
        std::default_delete<T>::operator()(p);
    }
};

template <class T>
using GCUniquePtr = std::unique_ptr<T, GCDeleter<T>>;

template <class T>
GCUniquePtr<T> GCWrapUnique(T *p)
{
    return GCUniquePtr<T>(p);
}

template <class T, class... Types>
GCUniquePtr<T> GCMakeUnique(Types&&... args)
{
    return GCWrapUnique(new T(std::forward<Types>(args)...));
}

/**
 * Member Stuff
 */

template <class T>
class GCMemberBase : public zed::ptr_base<T>
{
protected:
    GCMemberBase(T *ptr) : zed::ptr_base<T>(ptr) {}

    template <typename U>
    GCMemberBase& operator=(U *other)
    {
        this->m_ptr = other;
        return *this;
    }

    GCObject* AccessGCObject(void)
    {
        return static_cast<GCObject *>(const_cast<std::remove_const_t<T> *>(this->m_ptr));
    }
    void** GetSlot(void)
    {
        return reinterpret_cast<void **>(&this->m_ptr);
    }
};

template <class T>
class GCWeakMember final : public GCMemberBase<T>
{
public:
    GCWeakMember(T *ptr) : GCMemberBase<T>(ptr)
    {
        if (nullptr != this->m_ptr)
            Attach();
    }
    ~GCWeakMember(void)
    {
        if (nullptr != this->m_ptr)
            Detach();
    }

    void Clear(void)
    {
        if (nullptr != this->m_ptr)
        {
            Detach();
            this->m_ptr = nullptr;
        }
    }

    template <typename U>
    GCWeakMember& operator=(U *other)
    {
        if (nullptr != this->m_ptr)
            Detach();
        GCMemberBase<T>::operator=(other);
        if (nullptr != this->m_ptr)
            Attach();
        return *this;
    }
private:
    void Attach(void)
    {
        ASSERT(nullptr != this->m_ptr);
        GCAttachWeakSlot(this->AccessGCObject(), this->GetSlot());
    }
    void Detach(void)
    {
        ASSERT(nullptr != this->m_ptr);
        GCDetachWeakSlot(this->AccessGCObject(), this->GetSlot());
    }
};

} // namespace BlinKit

namespace blink {
using BlinKit::GCRefPtr;
using BlinKit::GCUniquePtr;
}

namespace std {

template <class T>
struct hash<BlinKit::GCRefPtr<T>>
{
    size_t operator()(const BlinKit::GCRefPtr<T> &m) const noexcept
    {
        return reinterpret_cast<size_t>(m.get());
    }
};

template <class T>
struct hash<BlinKit::GCWeakMember<T>>
{
    size_t operator()(const BlinKit::GCWeakMember<T> &m) const noexcept
    {
        return reinterpret_cast<size_t>(m.get());
    }
};

} // namespace std

#define BLINKIT_DEFINE_GC_CASTER(Class)             \
    static BlinKit::GCObject* GCCast(Class *o) {    \
        return static_cast<BlinKit::GCObject *>(o); \
    }

#endif // BLINKIT_BLINKIT_GC_DEF_H
