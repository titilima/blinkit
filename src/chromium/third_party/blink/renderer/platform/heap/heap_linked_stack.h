// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: heap_linked_stack.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2020-08-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HEAP_LINKED_STACK_H
#define BLINKIT_BLINK_HEAP_LINKED_STACK_H

#pragma once

#include <stack>
#include "third_party/blink/renderer/platform/heap/heap.h"
#include "third_party/blink/renderer/platform/heap/visitor.h"

namespace blink {

template <typename T>
class HeapLinkedStack : public GarbageCollected<HeapLinkedStack<T>>, private std::vector<T>
{
public:
    using std::vector<T>::size;

    void Trace(Visitor *visitor)
    {
        for (auto it = this->begin(); it != this->end(); ++it)
            visitor->Trace(*it);
    }

    bool IsEmpty(void) const { return this->empty(); }
    void Push(const T &o) { this->push_back(o); }
    const T& Peek(void) { return this->back(); }
    void Pop(void) { this->pop_back(); }
};

} // namespace blink

#endif // BLINKIT_BLINK_HEAP_LINKED_STACK_H
