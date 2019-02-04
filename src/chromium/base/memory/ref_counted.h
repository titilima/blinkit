// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: ref_counted.h
// Description: Reference Counted Classes & Helpers
//      Author: Ziming Li
//     Created: 2018-08-16
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_REF_COUNTED_H
#define BLINKIT_BASE_REF_COUNTED_H

#pragma once

#include <atomic>
#include "base/macros.h"

namespace base {

template <typename REF_COUNT>
class RefCountedBase
{
public:
    bool HasOneRef(void) const { return 1 == m_refCount; }
    void AddRef(void) const
    {
#ifdef _DEBUG
        assert(!m_inDtor);
#endif
        ++m_refCount;
    }
protected:
#ifdef _DEBUG
    ~RefCountedBase(void)
    {
        assert(m_inDtor); // RefCounted object deleted without calling Release()!
    }
#endif
    bool Release(void) const
    {
#ifdef _DEBUG
        assert(!m_inDtor);
#endif
        if (--m_refCount == 0)
        {
#ifdef _DEBUG
            m_inDtor = true;
#endif
            return true;
        }
        return false;
    }
private:
    mutable REF_COUNT m_refCount{ 0 };
#ifdef _DEBUG
    bool m_inDtor = false;
#endif
};

template <class T>
class RefCounted : public RefCountedBase<unsigned>
{
    DISALLOW_COPY_AND_ASSIGN(RefCounted<T>);
public:
    void Release(void) const
    {
        if (RefCountedBase::Release())
            delete static_cast<T *>(this);
    }
protected:
    ~RefCounted(void) = default;
};

template <class T>
class RefCountedThreadSafe : public RefCountedBase<std::atomic<unsigned>>
{
    DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafe<T>);
public:
    void Release(void) const
    {
        if (RefCountedBase::Release())
            delete static_cast<T *>(this);
    }
protected:
    ~RefCountedThreadSafe(void) = default;
};

} // namespace base

template <class T>
class scoped_refptr
{
public:
    typedef T element_type;

    scoped_refptr(void) : m_ptr(nullptr) {}
    scoped_refptr(T *p) : m_ptr(p)
    {
        if (nullptr != m_ptr)
            AddRef(m_ptr);
    }

    // Copy constructor.
    scoped_refptr(const scoped_refptr<T> &r) : m_ptr(r.m_ptr)
    {
        if (nullptr != m_ptr)
            AddRef(m_ptr);
    }

    // Copy conversion constructor.
    template <typename U>
    scoped_refptr(const scoped_refptr<U> &r) : m_ptr(r.get())
    {
        if (nullptr != m_ptr)
            AddRef(m_ptr);
    }

    // Move constructor. This is required in addition to the conversion
    // constructor below in order for clang to warn about pessimizing moves.
    scoped_refptr(scoped_refptr &&r) : m_ptr(r.get()) { r.m_ptr = nullptr; }

    // Move conversion constructor.
    template <typename U>
    scoped_refptr(scoped_refptr<U> &&r) : m_ptr(r.get()) { r.m_ptr = nullptr; }

    ~scoped_refptr(void)
    {
        if (nullptr != m_ptr)
            Release(m_ptr);
    }

    T* get(void) const { return m_ptr; }

    T& operator*() const {
        assert(m_ptr != nullptr);
        return *m_ptr;
    }

    T* operator->() const {
        assert(m_ptr != nullptr);
        return m_ptr;
    }

    operator bool() const { return nullptr != m_ptr; }

    scoped_refptr<T>& operator=(T *p)
    {
        // AddRef first so that self assignment should work
        if (nullptr != p)
            AddRef(p);
        T *old_ptr = m_ptr;
        m_ptr = p;
        if (nullptr != old_ptr)
            Release(old_ptr);
        return *this;
    }

    scoped_refptr<T>& operator=(const scoped_refptr<T> &r) { return *this = r.m_ptr; }

    template <typename U>
    scoped_refptr<T>& operator=(const scoped_refptr<U> &r) { return *this = r.get(); }

    scoped_refptr<T>& operator=(scoped_refptr<T> &&r)
    {
        scoped_refptr<T>(std::move(r)).swap(*this);
        return *this;
    }

    template <typename U>
    scoped_refptr<T>& operator=(scoped_refptr<U> &&r)
    {
        scoped_refptr<T>(std::move(r)).swap(*this);
        return *this;
    }

    void swap(T **pp)
    {
        T *p = m_ptr;
        m_ptr = *pp;
        *pp = p;
    }

    void swap(scoped_refptr<T> &r) { swap(&r.m_ptr); }

private:
    template <typename U> friend class scoped_refptr;
public:
    template <typename U>
    bool operator==(const scoped_refptr<U> &rhs) const { return m_ptr == rhs.get(); }

    template <typename U>
    bool operator!=(const scoped_refptr<U> &rhs) const { return !operator==(rhs); }

    template <typename U>
    bool operator<(const scoped_refptr<U> &rhs) const { return m_ptr < rhs.get(); }

protected:
    T *m_ptr;

private:
    // Non-inline helpers to allow:
    //     class Opaque;
    //     extern template class scoped_refptr<Opaque>;
    // Otherwise the compiler will complain that Opaque is an incomplete type.
    static void AddRef(T *ptr);
    static void Release(T *ptr);
};

template <typename T>
void scoped_refptr<T>::AddRef(T *ptr) { ptr->AddRef(); }

template <typename T>
void scoped_refptr<T>::Release(T *ptr) { ptr->Release(); }

#endif // BLINKIT_BASE_REF_COUNTED_H
