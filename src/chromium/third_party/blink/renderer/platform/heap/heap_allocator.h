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

#include <deque>
#include <vector>
#include <unordered_map>
#include "third_party/blink/renderer/platform/heap/heap.h"
#include "third_party/blink/renderer/platform/wtf/linked_hash_set.h"
#include "third_party/blink/renderer/platform/wtf/list_hash_set.h"
#include "third_party/blink/renderer/platform/wtf/hash_counted_set.h"
#include "third_party/blink/renderer/platform/wtf/hash_set.h"
#include "third_party/blink/renderer/platform/wtf/wtf_size_t.h"

namespace blink {

template <typename T>
class HeapDeque : public std::deque<T>
{
public:
    bool IsEmpty(void) const { return this->empty(); }
};

template <typename K, typename V>
class HeapHashMap : public std::unordered_map<K, V>
{
public:
    bool IsEmpty(void) const { return this->empty(); }

    bool Contains(const K &k) const
    {
        auto it = this->find(k);
        return this->end() != it;
    }
    void ReserveCapacityForSize(size_t size) {}
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

template <typename T>
class HeapLinkedHashSet : public LinkedHashSet<T, int, int, int>
{
};

template <typename T>
class HeapListHashSet : public ListHashSet<T>
{
};

template <typename T, wtf_size_t inlineCapacity = 0>
class HeapVector : public std::vector<T>
{
public:
    HeapVector(void) = default;
    explicit HeapVector(wtf_size_t size) : std::vector<T>(size) {}

    template <typename U>
    void push_front(U &&v)
    {
        this->insert(this->begin(), std::forward<U>(v));
    }

    bool IsEmpty(void) const { return this->empty(); }
    void Grow(wtf_size_t size) { this->resize(size); }
    void ReserveCapacity(wtf_size_t new_capacity) { this->reserve(new_capacity); }
    void ReserveInitialCapacity(wtf_size_t initial_capacity) { this->reserve(initial_capacity); }
    void Shrink(wtf_size_t size) { this->resize(size); }

    void EraseAt(wtf_size_t position)
    {
        this->erase(this->begin() + position);
    }
    void UncheckedAppend(const T &val)
    {
        this->push_back(val);
    }
    void AppendVector(const HeapVector<T> &v)
    {
        this->insert(this->end(), v.begin(), v.end());
    }
};

template <typename T>
class HeapHashCountedSet : public HashCountedSet<T>
{
};

} // namespace blink

#endif // BLINKIT_BLINK_HEAP_ALLOCATOR_H
