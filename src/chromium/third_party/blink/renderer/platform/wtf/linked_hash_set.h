// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: linked_hash_set.h
// Description: LinkedHashSet Class
//      Author: Ziming Li
//     Created: 2020-09-10
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_LINKED_HASH_SET_H
#define BLINKIT_BLINK_LINKED_HASH_SET_H

#pragma once

#include <list>
#include <unordered_map>

namespace WTF {

template <typename T>
class LinkedHashSetBase : protected std::list<T>
{
public:
    using const_iterator         = typename std::list<T>::const_iterator;
    using const_reverse_iterator = typename std::list<T>::const_reverse_iterator;
    using iterator               = typename std::list<T>::iterator;

    using std::list<T>::begin;
    using std::list<T>::end;
    using std::list<T>::rbegin;
    using std::list<T>::rend;
    using std::list<T>::size;

    void clear(void)
    {
        std::list<T>::clear();
        m_indices.clear();
    }
    void erase(iterator it)
    {
        if (end() != it)
        {
            size_t h = std::hash<T>{}(*it);
            m_indices.erase(h);
            std::list<T>::erase(it);
        }
    }
    void erase(const T &o) { erase(find(o)); }

    const_iterator find(const T &o) const
    {
        size_t h = std::hash<T>{}(o);
        auto it = m_indices.find(h);
        if (std::end(m_indices) == it)
            return end();
        return it->second;
    }
    iterator find(const T &o)
    {
        size_t h = std::hash<T>{}(o);
        auto it = m_indices.find(h);
        if (std::end(m_indices) == it)
            return end();
        return it->second;
    }

    bool Contains(const T &o) const { return end() != find(o); }
    bool IsEmpty(void) const { return std::list<T>::empty(); }

    void Swap(LinkedHashSetBase<T> &other)
    {
        std::list<T>::swap(other);
        std::swap(m_indices, other.m_indices);
    }
protected:
    std::unordered_map<size_t, iterator> m_indices;
};

template <typename T, typename U = int, typename V = int, typename W = int>
class LinkedHashSet : public LinkedHashSetBase<T>
{
public:
    auto insert(const T &o)
    {
        size_t h = std::hash<T>{}(o);
        auto ret = std::list<T>::insert(this->end(), o);
        this->m_indices.insert({ h, ret });
        return ret;
    }
};

template <typename T>
class LinkedHashSet<std::unique_ptr<T>> : public LinkedHashSetBase<std::unique_ptr<T>>
{
public:
    auto insert(std::unique_ptr<T> o)
    {
        size_t h = reinterpret_cast<size_t>(o.get());
        auto ret = std::list<std::unique_ptr<T>>::insert(this->end(), std::move(o));
        this->m_indices.insert({ h, ret });
        return ret;
    }
};

} // namespace WTF

using WTF::LinkedHashSet;

#endif // BLINKIT_BLINK_LINKED_HASH_SET_H
