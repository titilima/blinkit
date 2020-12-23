// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: scoped_refptr.h
// Description: scoped_refptr Class
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_SCOPED_REFPTR_H
#define BLINKIT_BASE_SCOPED_REFPTR_H

#pragma once

template <class T>
class scoped_refptr;

namespace base {

namespace subtle {
enum AdoptRefTag { kAdoptRefTag };
enum StartRefCountFromZeroTag { kStartRefCountFromZeroTag };
enum StartRefCountFromOneTag { kStartRefCountFromOneTag };
}

template <typename T>
scoped_refptr<T> AdoptRef(T *obj);

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
    template <typename U> friend scoped_refptr<U> base::AdoptRef(U *);
    scoped_refptr(T *p, base::subtle::AdoptRefTag) : m_ptr(p) {}

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

template <typename T, typename U>
inline bool operator==(const scoped_refptr<T> &lhs, const U *rhs)
{
    return lhs.get() == rhs;
}

template <typename T, typename U>
inline bool operator==(const T *lhs, const scoped_refptr<U> &rhs)
{
    return lhs == rhs.get();
}

template <typename T, typename U>
bool operator!=(const scoped_refptr<T> &lhs, const U *rhs)
{
    return !operator==(lhs, rhs);
}

template <typename T, typename U>
bool operator!=(const T *lhs, const scoped_refptr<U> &rhs)
{
    return !operator==(lhs, rhs);
}

namespace base {

namespace subtle {

template <typename T>
scoped_refptr<T> AdoptRefIfNeeded(T *obj, StartRefCountFromZeroTag)
{
    return scoped_refptr<T>(obj);
}

template <typename T>
scoped_refptr<T> AdoptRefIfNeeded(T *obj, StartRefCountFromOneTag)
{
    return AdoptRef(obj);
}

}

template <typename T>
scoped_refptr<T> AdoptRef(T *obj)
{
    DCHECK(obj);
    DCHECK(obj->HasOneRef());
    return scoped_refptr<T>(obj, subtle::kAdoptRefTag);
}

// Constructs an instance of T, which is a ref counted type, and wraps the
// object into a scoped_refptr<T>.
template <typename T, typename... Args>
scoped_refptr<T> MakeRefCounted(Args&&... args)
{
    T* obj = new T(std::forward<Args>(args)...);
    return AdoptRefIfNeeded(obj, T::kRefCountPreference);
}

} // namespace base

#endif // BLINKIT_BASE_SCOPED_REFPTR_H
