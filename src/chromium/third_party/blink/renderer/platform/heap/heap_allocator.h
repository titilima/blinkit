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

#include <list>
#include <unordered_map>
#include <unordered_set>
#include "third_party/blink/renderer/platform/heap/heap.h"
#include "third_party/blink/renderer/platform/wtf/linked_hash_set.h"
#include "third_party/blink/renderer/platform/wtf/list_hash_set.h"
#include "third_party/blink/renderer/platform/wtf/hash_counted_set.h"
#include "third_party/blink/renderer/platform/wtf/hash_set.h"
#include "third_party/blink/renderer/platform/wtf/wtf_size_t.h"

namespace blink {

template <typename T>
class HeapDeque : private std::list<T>
{
public:
    using std::list<T>::begin;
    using std::list<T>::end;
    using std::list<T>::erase;
    using std::list<T>::front;
    using std::list<T>::pop_front;
    using std::list<T>::push_back;

    bool IsEmpty(void) const { return this->empty(); }

    T TakeFirst(void)
    {
        T first = this->front();
        this->pop_front();
        return first;
    }
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
class HeapHashSet : private std::unordered_set<T>
{
public:
    using std::unordered_set<T>::begin;
    using std::unordered_set<T>::clear;
    using std::unordered_set<T>::end;
    using std::unordered_set<T>::erase;
    using std::unordered_set<T>::find;
    using std::unordered_set<T>::insert;
    using std::unordered_set<T>::size;

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

namespace BlinKit {

template <typename T>
struct TracePolicy<blink::HeapDeque<T>>
{
    static void Impl(blink::HeapDeque<T> &q, blink::Visitor *visitor)
    {
        for (auto &o : q)
            visitor->Trace(o);
    }
};

template <typename K, typename V>
struct TracePolicy<blink::HeapHashMap<K, V>>
{
    static void Impl(blink::HeapHashMap<K, V> &m, blink::Visitor *visitor)
    {
        for (auto &it : m)
            visitor->Trace(it.second);
    }
};

template <typename T>
struct TracePolicy<blink::HeapHashSet<T>>
{
    static void Impl(blink::HeapHashSet<T> &s, blink::Visitor *visitor)
    {
        for (auto &o : s)
            visitor->Trace(o);
    }
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_HEAP_ALLOCATOR_H
