#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HeapAllocator.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2021-07-02
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HEAP_ALLOCATOR_H
#define BLINKIT_BLINKIT_HEAP_ALLOCATOR_H

#include <unordered_set>
#include "blinkit/blink/renderer/platform/heap/Heap.h"
#include "blinkit/blink/renderer/wtf/HashCountedSet.h"
#include "blinkit/blink/renderer/wtf/ListHashSet.h"
#include "blinkit/blink/renderer/wtf/Vector.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace blink {

template <typename T, size_t inlineCapacity = 0>
class HeapDeque : private std::list<T>
{
public:
    using typename std::list<T>::const_reverse_iterator;
    using typename std::list<T>::iterator;

    using std::list<T>::back;
    using std::list<T>::begin;
    using std::list<T>::clear;
    using std::list<T>::end;
    using std::list<T>::erase;
    using std::list<T>::front;
    using std::list<T>::pop_back;
    using std::list<T>::pop_front;
    using std::list<T>::push_back;
    using std::list<T>::push_front;
    using std::list<T>::rbegin;
    using std::list<T>::rend;
    using std::list<T>::size;

    bool isEmpty(void) const { return this->empty(); }

    T TakeFirst(void)
    {
        T first = this->front();
        this->pop_front();
        return first;
    }
};

template <typename T>
class HeapHashCountedSet : public WTF::HashCountedSet<T>
{
};

template <typename K, typename V, typename H>
class HeapHashMapBase : protected std::unordered_map<K, V, H>
{
public:
    using key_type    = typename K;
    using mapped_type = typename V;

    using typename std::unordered_map<K, V, H>::const_iterator;
    using typename std::unordered_map<K, V, H>::iterator;

    using std::unordered_map<K, V, H>::begin;
    using std::unordered_map<K, V, H>::clear;
    using std::unordered_map<K, V, H>::emplace;
    using std::unordered_map<K, V, H>::empty;
    using std::unordered_map<K, V, H>::end;
    using std::unordered_map<K, V, H>::erase;
    using std::unordered_map<K, V, H>::find;
    using std::unordered_map<K, V, H>::insert;
    using std::unordered_map<K, V, H>::size;
    using std::unordered_map<K, V, H>::operator[];

    bool isEmpty(void) const { return this->empty(); }

    bool contains(const K &k) const
    {
        return zed::key_exists(*this, k);
    }
    void swap(HeapHashMapBase<K, V, H> &o)
    {
        std::unordered_map<K, V, H>::swap(o);
    }

    void ReserveCapacityForSize(size_t size) {}
};

template <typename K, typename V, typename H = std::hash<K>>
class HeapHashMap : public HeapHashMapBase<K, V, H>
{
public:
    using std::unordered_map<K, V, H>::at;
};

template <typename K, typename T, typename H>
class HeapHashMap<K, Member<T>, H> : public HeapHashMapBase<K, Member<T>, H>
{
public:
    T* at(const K &k) const
    {
        auto it = this->find(k);
        if (this->end() == it)
            return nullptr;
        return it->second.Get();
    }
};

template <typename T>
class HeapHashSet : private std::unordered_set<T>
{
public:
    using typename std::unordered_set<T>::const_iterator;
    using typename std::unordered_set<T>::iterator;

    using std::unordered_set<T>::begin;
    using std::unordered_set<T>::clear;
    using std::unordered_set<T>::end;
    using std::unordered_set<T>::size;

    void add(const T &o)
    {
        this->insert(o);
    }
    bool contains(const T &o) const
    {
        return this->find(o) != this->end();
    }
    bool isEmpty(void) const
    {
        return this->empty();
    }
    void remove(const T &o)
    {
        this->erase(o);
    }
};

template <typename T>
class HeapLinkedHashSet : public LinkedHashSet<T>
{
};

template <typename T, size_t inlineCapacity = 0>
class HeapListHashSet : public ListHashSet<T, inlineCapacity>
{
};

template <typename T, size_t inlineCapacity = 0>
class HeapVector : public std::vector<T>
{
public:
    void append(const T &o) { this->emplace_back(o); }
    auto first(void) { return this->front(); }
    auto first(void) const { return this->front(); }
    bool isEmpty(void) const { return this->empty(); }
    auto last(void) { return this->back(); }
    auto last(void) const { return this->back(); }
    void shrink(size_t size)
    {
        ASSERT(size <= this->size());
        this->resize(size);
        this->shrink_to_fit();
    }
    void shrinkToFit(void) { this->shrink_to_fit(); }
};

} // namespace blink

namespace BlinKit {

template <typename T>
struct TracePolicy<blink::HeapHashCountedSet<T>>
{
    static void Impl(blink::HeapHashCountedSet<T> &s, blink::Visitor *visitor)
    {
        for (auto &it : s)
            visitor->trace(it.first);
    }
};

template <typename K, typename V, typename H>
struct TracePolicy<blink::HeapHashMap<K, V, H>>
{
    static void Impl(blink::HeapHashMap<K, V, H> &m, blink::Visitor *visitor)
    {
        for (auto &[_, v] : m)
            visitor->trace(v);
    }
};

template <typename T>
struct TracePolicy<blink::HeapHashSet<T>>
{
    static void Impl(blink::HeapHashSet<T> &s, blink::Visitor *visitor)
    {
        for (auto &o : s)
            visitor->trace(o);
    }
};

template <typename T>
struct TracePolicy<blink::HeapLinkedHashSet<T>>
{
    static void Impl(blink::HeapLinkedHashSet<T> &s, blink::Visitor *visitor)
    {
        for (auto it = s.begin(); it != s.end(); ++it)
            visitor->trace(*it);
    }
};

template <typename T, size_t inlineCapacity>
struct TracePolicy<blink::HeapListHashSet<T, inlineCapacity>>
{
    static void Impl(blink::HeapListHashSet<T, inlineCapacity> &s, blink::Visitor *visitor)
    {
        for (auto it = s.begin(); it != s.end(); ++it)
            visitor->trace(*it);
    }
};

template <typename T, size_t inlineCapacity>
struct TracePolicy<blink::HeapVector<T, inlineCapacity>>
{
    static void Impl(blink::HeapVector<T, inlineCapacity>& v, blink::Visitor* visitor)
    {
        for (auto& o : v)
            visitor->trace(o);
    }
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_HEAP_ALLOCATOR_H
