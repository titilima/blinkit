// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: heap_allocator.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2020-08-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HEAP_ALLOCATOR_H
#define BLINKIT_BLINK_HEAP_ALLOCATOR_H

#pragma once

#include <vector>
#include <unordered_set>

namespace blink {

template <typename T>
class HeapHashSet : public std::unordered_set<T>
{
};

template <typename T>
class HeapVector : public std::vector<T>
{
};

} // namespace blink

#endif // BLINKIT_BLINK_HEAP_ALLOCATOR_H
