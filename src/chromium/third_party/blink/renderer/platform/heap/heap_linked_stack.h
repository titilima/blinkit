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

namespace blink {

template <typename T>
class HeapLinkedStack : public std::stack<T>
{
public:
    bool IsEmpty(void) const { return std::stack::empty(); }
};

} // namespace blink

#endif // BLINKIT_BLINK_HEAP_LINKED_STACK_H
