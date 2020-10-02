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
#include "base/memory/scoped_refptr.h"

namespace base {

template <typename REF_COUNT>
class RefCountedBase
{
public:
    bool HasOneRef(void) const { return 1 == m_refCount; }
    void AddRef(void) const
    {
#ifndef NDEBUG
        assert(!m_inDtor);
#endif
        ++m_refCount;
    }
protected:
#ifndef NDEBUG
    ~RefCountedBase(void)
    {
        assert(m_inDtor); // RefCounted object deleted without calling Release()!
    }
#endif
    bool Release(void) const
    {
#ifndef NDEBUG
        assert(!m_inDtor);
#endif
        if (--m_refCount == 0)
        {
#ifndef NDEBUG
            m_inDtor = true;
#endif
            return true;
        }
        return false;
    }
private:
    mutable REF_COUNT m_refCount{ 0 };
#ifndef NDEBUG
    mutable bool m_inDtor = false;
#endif
};

template <class T, typename Traits>
class RefCounted;

template <typename T>
struct DefaultRefCountedTraits
{
    static void Destruct(const T *x)
    {
        RefCounted<T, DefaultRefCountedTraits>::DeleteInternal(x);
    }
};

template <class T, typename Traits = DefaultRefCountedTraits<T>>
class RefCounted : public RefCountedBase<unsigned>
{
public:
    void Release(void) const
    {
        if (RefCountedBase::Release())
            Traits::Destruct(static_cast<const T *>(this));
    }
protected:
    RefCounted(void) = default;
    ~RefCounted(void) = default;
private:
    friend struct DefaultRefCountedTraits<T>;
    template <typename U>
    static void DeleteInternal(const U *x)
    {
        delete x;
    }

    DISALLOW_COPY_AND_ASSIGN(RefCounted);
};

template <class T, typename Traits = DefaultRefCountedTraits<T>>
class RefCountedThreadSafe : public RefCountedBase<std::atomic<unsigned>>
{
public:
    void Release(void) const
    {
        if (RefCountedBase::Release())
            Traits::Destruct(static_cast<const T *>(this));
    }
protected:
    RefCountedThreadSafe(void) = default;
    ~RefCountedThreadSafe(void) = default;
private:
    friend struct DefaultRefCountedTraits<T>;
    template <typename U>
    static void DeleteInternal(const U *x)
    {
        delete x;
    }

    DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafe);
};

} // namespace base

#define REQUIRE_ADOPTION_FOR_REFCOUNTED_TYPE()

#endif // BLINKIT_BASE_REF_COUNTED_H
