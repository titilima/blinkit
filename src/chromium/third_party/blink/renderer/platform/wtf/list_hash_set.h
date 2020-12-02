// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: list_hash_set.h
// Description: ListHashSet Class
//      Author: Ziming Li
//     Created: 2020-09-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_LIST_HASH_SET_H
#define BLINKIT_BLINK_LIST_HASH_SET_H

#pragma once

#include <list>
#include <unordered_map>

namespace WTF {

template <typename T, size_t inlineCapacity = 0>
class ListHashSet : private std::list<T>
{
public:
    using std::list<T>::begin;
    using std::list<T>::const_iterator;
    using std::list<T>::end;
    using std::list<T>::iterator;
    using std::list<T>::rbegin;
    using std::list<T>::rend;
    using std::list<T>::size;

    void clear(void)
    {
        std::list<T>::clear();
        m_hashMap.clear();
    }
    void erase(typename std::list<T>::iterator it)
    {
        ASSERT(false); // BKTODO:
    }
    void erase(const T &o)
    {
        auto it = m_hashMap.find(o);
        if (std::end(m_hashMap) != it)
        {
            std::list<T>::erase(it->second);
            m_hashMap.erase(it);
        }
    }
    auto find(const T &o)
    {
        auto it = m_hashMap.find(o);
        if (std::end(m_hashMap) == it)
            return this->end();
        else
            return it->second;
    }
    auto insert(const T &o)
    {
        auto ret = std::list<T>::insert(this->end(), o);
        m_hashMap.insert({ o, ret });
        return ret;
    }

    bool IsEmpty(void) const { return this->empty(); }
    bool Contains(const T &o) const { return std::end(m_hashMap) != m_hashMap.find(o); }
private:
    std::unordered_map<T, typename std::list<T>::iterator> m_hashMap;
};

} // namespace WTF

using WTF::ListHashSet;

#endif // BLINKIT_BLINK_LIST_HASH_SET_H
