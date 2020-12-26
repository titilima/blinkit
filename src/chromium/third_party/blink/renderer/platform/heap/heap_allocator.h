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
#include "third_party/blink/renderer/platform/wtf/list_hash_set.h"
#include "third_party/blink/renderer/platform/wtf/hash_counted_set.h"
#include "third_party/blink/renderer/platform/wtf/hash_set.h"
#include "third_party/blink/renderer/platform/wtf/wtf_size_t.h"

namespace blink {

template <typename T>
class HeapDeque : private std::list<T>
{
public:
    using std::list<T>::back;
    using std::list<T>::begin;
    using std::list<T>::clear;
    using std::list<T>::const_reverse_iterator;
    using std::list<T>::end;
    using std::list<T>::erase;
    using std::list<T>::front;
    using std::list<T>::iterator;
    using std::list<T>::pop_front;
    using std::list<T>::push_back;
    using std::list<T>::rbegin;
    using std::list<T>::rend;
    using std::list<T>::size;

    bool IsEmpty(void) const { return this->empty(); }

    T TakeFirst(void)
    {
        T first = this->front();
        this->pop_front();
        return first;
    }
};

template <typename K, typename V, typename H = std::hash<K>>
class HeapHashMap : public std::unordered_map<K, V, H>
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

template <typename K, typename T, typename H>
class HeapHashMap<K, Member<T>, H> : public std::unordered_map<K, Member<T>, H>
{
public:
    bool IsEmpty(void) const { return this->empty(); }

    T* at(const K &k) const
    {
        auto it = this->find(k);
        if (this->end() == it)
            return nullptr;
        return it->second.Get();
    }

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
    using std::unordered_set<T>::const_iterator;
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
class HeapLinkedHashSet : public LinkedHashSet<T>
{
};

template <typename T, size_t inlineCapacity = 0>
class HeapListHashSet : public ListHashSet<T, inlineCapacity>
{
};

template <typename T, wtf_size_t inlineCapacity = 0>
class HeapVector : public std::vector<T>
{
public:
    HeapVector(void) = default;
    explicit HeapVector(wtf_size_t size) : std::vector<T>(size) {}

    class GCForbiddenScope // Just a placeholder.
    {
    public:
        GCForbiddenScope(void) {}
    };

    template <typename U>
    void push_front(U &&v)
    {
        this->insert(this->begin(), std::forward<U>(v));
    }

    bool IsEmpty(void) const { return this->empty(); }
    void Grow(wtf_size_t size)
    {
        ASSERT(size >= this->size());
        this->resize(size);
    }
    void ReserveCapacity(wtf_size_t new_capacity) { this->reserve(new_capacity); }
    void ReserveInitialCapacity(wtf_size_t initial_capacity) { this->reserve(initial_capacity); }
    void Shrink(wtf_size_t size)
    {
        this->resize(size);
        this->shrink_to_fit();
    }

    void EraseAt(wtf_size_t position)
    {
        this->erase(this->begin() + position);
    }
    void EraseAt(wtf_size_t position, wtf_size_t length)
    {
        this->erase(this->begin() + position, this->begin() + position + length);
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

template <typename T>
struct TracePolicy<blink::HeapHashCountedSet<T>>
{
    static void Impl(blink::HeapHashCountedSet<T> &s, blink::Visitor *visitor)
    {
        for (auto &it : s)
            visitor->Trace(it.first);
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

template <typename T>
struct TracePolicy<blink::HeapLinkedHashSet<T>>
{
    static void Impl(blink::HeapLinkedHashSet<T> &s, blink::Visitor *visitor)
    {
        for (auto it = s.begin(); it != s.end(); ++it)
            visitor->Trace(*it);
    }
};

template <typename T, size_t inlineCapacity>
struct TracePolicy<blink::HeapListHashSet<T, inlineCapacity>>
{
    static void Impl(blink::HeapListHashSet<T, inlineCapacity> &s, blink::Visitor *visitor)
    {
        for (auto it = s.begin(); it != s.end(); ++it)
            visitor->Trace(*it);
    }
};

template <typename T, wtf_size_t inlineCapacity>
struct TracePolicy<blink::HeapVector<T, inlineCapacity>>
{
    static void Impl(blink::HeapVector<T, inlineCapacity> &v, blink::Visitor *visitor)
    {
        for (auto &o : v)
            visitor->Trace(o);
    }
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_HEAP_ALLOCATOR_H
