// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LinkedHashSet.h
// Description: LinkedHashSet Class
//      Author: Ziming Li
//     Created: 2020-09-10
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LINKED_HASH_SET_H
#define BLINKIT_BLINKIT_LINKED_HASH_SET_H

#pragma once

#include <list>
#include <unordered_map>

namespace WTF {

template <typename T, typename Hash = std::hash<T>>
class LinkedHashSetBase : protected std::list<T>
{
public:
    using const_iterator         = typename std::list<T>::const_iterator;
    using const_reverse_iterator = typename std::list<T>::const_reverse_iterator;
    using iterator               = typename std::list<T>::iterator;

    using std::list<T>::back;
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
        if (this->end() != it)
        {
            size_t h = Hash{}(*it);
            m_indices.erase(h);
            std::list<T>::erase(it);
        }
    }

    bool isEmpty(void) const { return std::list<T>::empty(); }

    void Swap(LinkedHashSetBase<T, Hash> &other)
    {
        std::list<T>::swap(other);
        std::swap(m_indices, other.m_indices);
    }
protected:
    const_iterator FindByHash(size_t h) const
    {
        auto it = m_indices.find(h);
        if (m_indices.end() == it)
            return this->end();
        return it->second;
    }
    iterator FindByHash(size_t h)
    {
        auto it = m_indices.find(h);
        if (m_indices.end() == it)
            return this->end();
        return it->second;
    }

    std::unordered_map<size_t, iterator> m_indices;
};

template <typename T, typename Hash = std::hash<T>, typename V = int, typename W = int>
class LinkedHashSet : public LinkedHashSetBase<T, Hash>
{
public:
    bool contains(const T &o) const { return this->end() != this->find(o); }

    using LinkedHashSetBase<T, Hash>::erase;
    void erase(const T &o)
    {
        LinkedHashSetBase<T, Hash>::erase(this->find(o));
    }

    auto find(const T &o) const
    {
        return this->FindByHash(Hash{}(o));
    }
    auto find(const T &o)
    {
        return this->FindByHash(Hash{}(o));
    }

    auto insert(const T &o)
    {
        size_t h = Hash{}(o);
        auto ret = std::list<T>::insert(this->end(), o);
        this->m_indices.insert({ h, ret });
        return ret;
    }
};

template <typename T>
class LinkedHashSet<std::unique_ptr<T>> : public LinkedHashSetBase<std::unique_ptr<T>>
{
public:
    bool Contains(const T *o) const
    {
        size_t h = reinterpret_cast<size_t>(o);
        return this->m_indices.end() != this->m_indices.find(h);
    }

    auto find(const T *o) const
    {
        return this->FindByHash(reinterpret_cast<size_t>(o));
    }
    auto find(const T *o)
    {
        return this->FindByHash(reinterpret_cast<size_t>(o));
    }

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

#endif // BLINKIT_BLINKIT_LINKED_HASH_SET_H
