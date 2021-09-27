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
#include "third_party/zed/include/zed/memory.hpp"

namespace blink {
class Visitor;
}

namespace BlinKit {

template <class> class GCMember;
class GCStub;

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
    template <class T> friend class GCMember;
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
 * Global/Persistent APIs
 */

void GCSetGlobalObject(GCObject *o);

template <class T>
T* GCWrapGlobal(T *p)
{
    GCSetGlobalObject(static_cast<GCObject *>(p));
    return p;
}

template <class T, class... Types>
T* GCMakeGlobal(Types&&... args)
{
    return GCWrapGlobal(new T(std::forward<Types>(args)...));
}

void GCRetainPersistentObject(GCObject *o, void **slot);
void GCReleasePersistentObject(GCObject *o, void **slot);

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
 * GCPassPtr
 */

template <class T>
class GCPassPtr final
{
    template <class> friend class GCMemberBase;
    template <class> friend class GCMember;
    template <class> friend class GCPersistentMember;
public:
    GCPassPtr(void) = default;
#ifdef NDEBUG
    GCPassPtr(T *ptr) : m_ptr(ptr) {}
#else
    GCPassPtr(T *ptr, bool shouldBeAdopted = false) : m_ptr(ptr), m_shouldBeAdopted(shouldBeAdopted) {}
#endif
    template <class U>
    GCPassPtr(GCPassPtr<U> &&o)
#ifndef NDEBUG
        : m_shouldBeAdopted(o.ShouldBeAdopted())
#endif
    {
        o.PassTo(m_ptr);
    }
    template <class U>
    GCPassPtr(const GCMember<U> &m);
    ~GCPassPtr(void)
    {
        ASSERT(!m_shouldBeAdopted || nullptr == m_ptr);
    }

    operator bool() const { return nullptr != m_ptr; }
    bool operator!() const { return nullptr == m_ptr; }
    T* get(void) const { return m_ptr; }

    void Clear(void) { m_ptr = nullptr; }
#ifndef NDEBUG
    bool ShouldBeAdopted(void) const { return m_shouldBeAdopted; }
#endif

    template <class U>
    GCPassPtr<U> PassTo(void)
    {
        U *ret = static_cast<U *>(m_ptr);
        m_ptr = nullptr;
#ifdef NDEBUG
        return GCPassPtr<U>(ret);
#else
        return GCPassPtr<U>(ret, m_shouldBeAdopted);
#endif
    }
    template <class U>
    void PassTo(U *&dst)
    {
        dst = m_ptr;
#ifndef NDEBUG
        if (m_shouldBeAdopted)
            Clear();
#endif
    }
private:
#ifndef NDEBUG
    const bool m_shouldBeAdopted = false;
#endif
    T *m_ptr = nullptr;
};

template <class T>
GCPassPtr<T> WrapLeaked(T *rawPtr)
{
#ifdef NDEBUG
    return GCPassPtr<T>(rawPtr);
#else
    return GCPassPtr<T>(rawPtr, true);
#endif
}

/**
 * GCPtr Stuff
 */

class GCPtrBase
{
public:
    ~GCPtrBase(void)
    {
        if (nullptr != m_object)
            m_object->Release();
    }
protected:
    GCPtrBase(GCObject *object) : m_object(object)
    {
        if (nullptr != m_object)
            m_object->IncRef();
    }

    template <class T>
    T* cast_to(void) const
    {
        return static_cast<T *>(m_object);
    }
private:
    GCObject *m_object;
};

class GCGuard final : public GCPtrBase
{
public:
    GCGuard(GCObject &o) : GCPtrBase(&o) {}
    GCGuard(GCStub *stub) : GCPtrBase(stub->ObjectForGC()) {}
};

template <class T>
class GCPtr final : public GCPtrBase
{
public:
    explicit GCPtr(T *p = nullptr) : GCPtrBase(p) {}

    template <class U>
    GCPtr(const WTF::RawPtr<U> &ptr) : GCPtr(ptr.get()) {}

    T* get(void) const { return GCPtrBase::cast_to<T>(); }
};

/**
 * Member Stuff
 */

template <class T>
class GCMemberBase : public zed::ptr_base<T>
{
protected:
    GCMemberBase(T *ptr) : zed::ptr_base<T>(ptr) {}
    GCMemberBase(GCPassPtr<T> &&o) : zed::ptr_base<T>(nullptr)
    {
        o.PassTo(this->m_ptr);
    }

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
class GCMember final : public GCMemberBase<T>
{
    friend class blink::Visitor;
public:
    explicit GCMember(T *ptr = nullptr) : GCMemberBase<T>(ptr)
    {
        ASSERT(!IsCollectingGarbage());
        if (nullptr != this->m_ptr)
            IncRef();
    }
    explicit GCMember(T &rawObj) : GCMember(&rawObj) {}
    GCMember(GCMember<T> &&o) : GCMemberBase<T>(o.m_ptr)
    {
        ASSERT(!IsCollectingGarbage());
        o.m_ptr = nullptr;
    }
    GCMember(const GCMember<T> &o) : GCMemberBase<T>(o.m_ptr)
    {
        ASSERT(!IsCollectingGarbage());
        if (nullptr != this->m_ptr)
            IncRef();
    }
    GCMember(GCPassPtr<T> &&other) : GCMemberBase<T>(std::move(other))
    {
        ASSERT(!IsCollectingGarbage());
        if (nullptr != this->m_ptr)
            IncRef();
    }
    ~GCMember(void)
    {
        ASSERT(!IsCollectingGarbage());
        if (nullptr != this->m_ptr)
            Release(GCOption::Auto);
    }

    GCMember& operator=(const GCMember<T> &other)
    {
        if (nullptr != this->m_ptr)
            Release(GCOption::Auto);
        GCMemberBase<T>::operator=(other.get());
        if (nullptr != this->m_ptr)
            IncRef();
        return *this;
    }
    template <typename U>
    GCMember& operator=(U *other)
    {
        if (nullptr != other)
            static_cast<GCObject *>(other)->IncRef();
        if (nullptr != this->m_ptr)
            Release(GCOption::Auto);
        GCMemberBase<T>::operator=(other);
        return *this;
    }
    template <typename U>
    GCMember& operator=(const WTF::RawPtr<U> &other)
    {
        if (other)
            static_cast<GCObject *>(other)->IncRef();
        if (nullptr != this->m_ptr)
            Release(GCOption::Auto);
        GCMemberBase<T>::operator=(other.get());
        return *this;
    }
    template <typename U>
    GCMember& operator=(GCPassPtr<U> &&other)
    {
        if (this->m_ptr != other.m_ptr)
        {
            if (nullptr != this->m_ptr)
                Release(GCOption::Auto);
            other.PassTo(this->m_ptr);
            if (nullptr != this->m_ptr)
                IncRef();
        }
        else
        {
            other.Clear();
        }
        return *this;
    }

    GCPassPtr<T> release(void)
    {
        T *ret = this->m_ptr;
        if (nullptr != ret)
        {
            this->AccessGCObject()->DecRef();
            this->m_ptr = nullptr;
        }
        return WrapLeaked(ret);
    }
    void clear(GCOption option = GCOption::Auto)
    {
        if (nullptr != this->m_ptr)
            Release(option);
    }
    void swap(GCMember<T> &o)
    {
        ASSERT(!IsCollectingGarbage());
        std::swap(this->m_ptr, o.m_ptr);
    }
private:
    void IncRef(void)
    {
        ASSERT(nullptr != this->m_ptr);
        this->AccessGCObject()->IncRef();
    }
    void Release(GCOption option)
    {
        ASSERT(nullptr != this->m_ptr);
        GCObject *o = this->AccessGCObject();
        this->m_ptr = nullptr;
        o->Release(option);
    }
};

template <class T>
class GCPersistentMember : public GCMemberBase<T>
{
public:
    GCPersistentMember(void) : GCMemberBase<T>(nullptr) {}
    GCPersistentMember(GCPassPtr<T> &&o) : GCMemberBase<T>(std::move(o))
    {
        if (nullptr != this->m_ptr)
            Retain();
    }
    ~GCPersistentMember(void)
    {
        if (nullptr != this->m_ptr)
            Release();
    }

    GCPersistentMember& operator=(const GCPersistentMember<T> &other)
    {
        if (nullptr != this->m_ptr)
            Release();
        GCMemberBase<T>::operator=(other.m_ptr);
        if (nullptr != this->m_ptr)
            Retain();
        return *this;
    }
    template <typename U>
    GCPersistentMember& operator=(const WTF::RawPtr<U> &other)
    {
        if (nullptr != this->m_ptr)
            Release();
        GCMemberBase<T>::operator=(other.get());
        if (nullptr != this->m_ptr)
            Retain();
        return *this;
    }
    template <typename U>
    GCPersistentMember& operator=(GCPassPtr<U> &&other)
    {
        if (this->m_ptr != other.m_ptr)
        {
            if (nullptr != this->m_ptr)
                Release();
            other.PassTo(this->m_ptr);
            if (nullptr != this->m_ptr)
                Retain();
        }
        else
        {
            other.Clear();
        }
        return *this;
    }
protected:
    void Retain(void)
    {
        ASSERT(nullptr != this->m_ptr);
        GCRetainPersistentObject(this->AccessGCObject(), this->GetSlot());
    }
    void Release(void)
    {
        ASSERT(nullptr != this->m_ptr);
        GCReleasePersistentObject(this->AccessGCObject(), this->GetSlot());
        ASSERT(nullptr == this->m_ptr);
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

/**
 * Implementations
 */

template <class T>
template <class U>
GCPassPtr<T>::GCPassPtr(const GCMember<U> &m) : m_ptr(m.get()) {}

} // namespace BlinKit

using BlinKit::GCPassPtr;

namespace std {

template <class T>
struct hash<BlinKit::GCMember<T>>
{
    size_t operator()(const BlinKit::GCMember<T> &m) const noexcept
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
