// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: persistent.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2020-08-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_PERSISTENT_H
#define BLINKIT_BLINKIT_PERSISTENT_H

#pragma once

#include "blinkit/blink/renderer/platform/heap/HeapAllocator.h"

namespace blink {

template <class T>
class Persistent
{
public:
    Persistent(void) = default;
    Persistent(const Persistent &other) : m_raw(other.m_raw)
    {
        if (nullptr != m_raw)
            ASSERT(false); // BKTODO: Remove it later!
    }
    Persistent(T *raw) : m_raw(raw)
    {
        if (nullptr != m_raw)
            ASSERT(false); // BKTODO: Remove it later!
    }
    Persistent(T &raw) : m_raw(&raw)
    {
        ASSERT(false); // BKTODO: Remove it later!
    }
    ~Persistent(void)
    {
        if (nullptr != m_raw)
            ASSERT(false); // BKTODO: Remove it later!
    }

    explicit operator bool() const { return nullptr != m_raw; }
    T* operator->() const { return m_raw; }
    T& operator*() const { return *m_raw; }
    operator RawPtr<T>() const { return m_raw; }
    Persistent& operator=(const Persistent &other)
    {
        Assign(other.m_raw);
        return *this;
    }
    template<typename U>
    Persistent& operator=(const RawPtr<U> &other)
    {
        Assign(other.get());
        return *this;
    }
    operator T*() const { return m_raw; }
    T* get(void) const { return m_raw; }

    void clear(void)
    {
        if (nullptr != m_raw)
        {
            ASSERT(false); // BKTODO: Remove it later!
            m_raw = nullptr;
        }
    }
private:
    template <typename U>
    void Assign(U *p)
    {
        if (nullptr != m_raw)
            ASSERT(false); // BKTODO: Remove it later!
        m_raw = p;
        if (nullptr != m_raw)
            ASSERT(false); // BKTODO: Remove it later!
    }

    T *m_raw = nullptr;
};

template <typename T, typename U>
inline bool operator==(const Persistent<T> &a, const Persistent<U> &b)
{
    return a.get() == b.get();
}

template <typename T, typename U>
inline bool operator!=(const Persistent<T> &a, const Persistent<U> &b)
{
    return a.get() != b.get();
}

template <class T> using WeakPersistent = WeakMember<T>;

} // BLINKIT_BLINK_PERSISTENT_H

#endif // BLINKIT_BLINKIT_PERSISTENT_H
