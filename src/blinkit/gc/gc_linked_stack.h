#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_linked_stack.h
// Description: GCLinkedStack Class
//      Author: Ziming Li
//     Created: 2021-09-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_LINKED_STACK_H
#define BLINKIT_BLINKIT_GC_LINKED_STACK_H

#include <list>
#include "blinkit/blink/renderer/platform/heap/Visitor.h"

namespace BlinKit {

template <typename T>
class GCLinkedStack : private std::list<T>
{
    using StdImpl = std::list<T>;
public:
    using StdImpl::empty;
    using StdImpl::size;

    void pop(void) { StdImpl::pop_back(); }
    void push(const T &o) { StdImpl::emplace_back(o); }
    const T& top(void) const
    {
        ASSERT(!StdImpl::empty());
        return StdImpl::back();
    }

    void trace(blink::Visitor *visitor)
    {
        for (auto it = StdImpl::begin(); StdImpl::end() != it; ++it)
            visitor->trace(*it);
    }
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_LINKED_STACK_H
