// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HeapTerminatedArray.h
// Description: HeapTerminatedArray Class
//      Author: Ziming Li
//     Created: 2021-07-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HeapTerminatedArray_h
#define HeapTerminatedArray_h

#include "platform/heap/Heap.h"
#include "wtf/TerminatedArray.h"
#include "wtf/TerminatedArrayBuilder.h"

namespace blink {

template<typename T>
class HeapTerminatedArray : public TerminatedArray<T> {
    DISALLOW_NEW();
public:
    using TerminatedArray<T>::begin;
    using TerminatedArray<T>::end;

    DEFINE_INLINE_TRACE()
    {
        for (typename TerminatedArray<T>::iterator it = begin(); it != end(); ++it)
            visitor->trace(*it);
    }

private:
    // Allocator describes how HeapTerminatedArrayBuilder should create new intances
    // of TerminateArray and manage their lifetimes.
    struct Allocator {
        typedef HeapTerminatedArray* PassPtr;
        typedef RawPtr<HeapTerminatedArray> Ptr;

        static PassPtr create(size_t capacity)
        {
            ASSERT(false); // BKTODO: return reinterpret_cast<HeapTerminatedArray*>(Heap::allocate<HeapTerminatedArray>(capacity * sizeof(T), IsEagerlyFinalizedType<T>::value));
            return nullptr;
        }

        static PassPtr resize(PassPtr ptr, size_t capacity)
        {
            ASSERT(false); // BKTODO: return reinterpret_cast<HeapTerminatedArray*>(Heap::reallocate<HeapTerminatedArray>(ptr, capacity * sizeof(T)));
            return nullptr;
        }
    };

    // Prohibit construction. Allocator makes HeapTerminatedArray instances for
    // HeapTerminatedArrayBuilder by pointer casting.
    HeapTerminatedArray();

    template<typename U, template <typename> class> friend class WTF::TerminatedArrayBuilder;
};

#if 0 // BKTODO:
template<typename T>
class TraceEagerlyTrait<HeapTerminatedArray<T>> {
public:
    static const bool value = TraceEagerlyTrait<T>::value;
};
#endif

} // namespace blink

#endif // HeapTerminatedArray_h
