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
#include <unordered_map>
#include <unordered_set>
#include "third_party/blink/renderer/platform/wtf/wtf_size_t.h"

namespace blink {

template <typename K, typename V>
class HeapHashMap : public std::unordered_map<K, V>
{
};

template <typename T>
class HeapHashSet : public std::unordered_set<T>
{
public:
    bool Contains(const T& o) const
    {
        auto it = this->find(o);
        return this->end() != it;
    }
    bool IsEmpty(void) const { return this->empty(); }
};

template <typename T, wtf_size_t inlineCapacity = 0>
class HeapVector : public std::vector<T>
{
public:
    HeapVector(void) = default;
    explicit HeapVector(wtf_size_t size) : std::vector<T>(size) {}

    bool IsEmpty(void) const { return this->empty(); }
    void Grow(wtf_size_t size) { this->resize(size); }

    void EraseAt(wtf_size_t position)
    {
        this->erase(this->begin() + position);
    }
};

} // namespace blink

#endif // BLINKIT_BLINK_HEAP_ALLOCATOR_H
