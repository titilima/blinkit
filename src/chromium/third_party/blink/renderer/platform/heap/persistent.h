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

namespace blink {

template <class T>
class Persistent
{
public:
    Persistent(void) = default;
    Persistent(const T *)
    {
        ASSERT(false); // BKTODO:
    }
    operator bool() const
    {
        ASSERT(false); // BKTODO:
        return false;
    }
    T* operator->() const
    {
        ASSERT(false); // BKTODO:
        return nullptr;
    }
    T& operator*() const
    {
        ASSERT(false); // BKTODO:
        return *Get();
    }
    T* Get(void) const
    {
        ASSERT(false); // BKTODO:
        return nullptr;
    }
    void Clear(void)
    {
        ASSERT(false); // BKTODO:
    }
};

template <class T> using WeakPersistent = Persistent<T>;

} // BLINKIT_BLINK_PERSISTENT_H

#endif // BLINKIT_BLINK_PERSISTENT_H
