// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: thread_safe_ref_counted.h
// Description: Placeholders for WTF Part
//      Author: Ziming Li
//     Created: 2020-08-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_THREAD_SAFE_REF_COUNTED_H
#define BLINKIT_BLINK_THREAD_SAFE_REF_COUNTED_H

#pragma once

#include "base/memory/ref_counted.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace WTF {

template <typename T, typename Traits>
class ThreadSafeRefCounted;

template <typename T>
struct DefaultThreadSafeRefCountedTraits
{
    static void Destruct(const T *x)
    {
        WTF::ThreadSafeRefCounted<T, DefaultThreadSafeRefCountedTraits>::DeleteInternal(x);
    }
};

template <typename T, typename Traits = DefaultThreadSafeRefCountedTraits<T>>
class ThreadSafeRefCounted : public base::RefCountedThreadSafe<T, Traits>
{
protected:
    ThreadSafeRefCounted(void)
    {
        this->AddRef(); // Adoption required
    }
private:
    friend struct DefaultThreadSafeRefCountedTraits<T>;

    template <typename U>
    static void DeleteInternal(const U *x)
    {
        delete x;
    }
};

} // namespace WTF

using WTF::ThreadSafeRefCounted;

#endif // BLINKIT_BLINK_THREAD_SAFE_REF_COUNTED_H
