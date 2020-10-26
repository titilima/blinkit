// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: persistent.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2020-08-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PERSISTENT_H
#define BLINKIT_BLINK_PERSISTENT_H

#pragma once

#include "blinkit/app/heap_storage.h"
#include "third_party/blink/renderer/platform/heap/heap_allocator.h"

namespace blink {

template <class T>
class Persistent
{
public:
    Persistent(void) = default;
    Persistent(T *raw) : m_raw(raw)
    {
        if (nullptr != raw)
            BlinKit::HeapStorage::SavePersistentObject(*raw);
    }
    Persistent(T &raw)
    {
        ASSERT(false); // BKTODO:
    }
    explicit operator bool() const { return nullptr != m_raw; }
    T* operator->() const { return m_raw; }
    T& operator*() const { return *m_raw; }
    operator T*() const { return m_raw; }
    T* Get(void) const { return m_raw; }

    void Clear(void)
    {
        ASSERT(false); // BKTODO:
    }
private:
    T *m_raw = nullptr;
};

template <typename T, typename U>
inline bool operator==(const Persistent<T> &a, const Persistent<U> &b)
{
    return a.Get() == b.Get();
}

template <typename T, typename U>
inline bool operator!=(const Persistent<T> &a, const Persistent<U> &b)
{
    return a.Get() != b.Get();
}

template <class T> using WeakPersistent = Persistent<T>;

} // BLINKIT_BLINK_PERSISTENT_H

#endif // BLINKIT_BLINK_PERSISTENT_H
