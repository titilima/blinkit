#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HashCountedSet.h
// Description: HashCountedSet Class
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HASH_COUNTED_SET_H
#define BLINKIT_BLINKIT_HASH_COUNTED_SET_H

namespace WTF {

template <typename T>
class HashCountedSet
{
public:
    using iterator = typename std::unordered_map<T, unsigned>::iterator;

    bool isEmpty(void) const { return m_impl.empty(); }
    size_t size(void) const { return m_impl.size(); }

    iterator begin(void) { return m_impl.begin(); }
    iterator end(void) { return m_impl.end(); }
    void erase(iterator it) { m_impl.erase(it); }
    iterator find(const T &o) { return m_impl.find(o); }

    bool contains(const T &o) const { return m_impl.end() != m_impl.find(o); }

    struct AddResult {
        bool is_new_entry;
    };
    AddResult insert(const T &value)
    {
        AddResult ret;
        auto it = m_impl.find(value);
        if (std::end(m_impl) == it)
        {
            m_impl.insert({ value, 1 });
            ret.is_new_entry = true;
        }
        else
        {
            ++(it->second);
            ret.is_new_entry = false;
        }
        return ret;
    }
    bool erase(const T &value)
    {
        auto it = m_impl.find(value);
        if (std::end(m_impl) == it)
            return false;
        m_impl.erase(it);
        return true;
    }
private:
    std::unordered_map<T, unsigned> m_impl;
};

} // namespace WTF

using WTF::HashCountedSet;

#endif // BLINKIT_BLINKIT_HASH_COUNTED_SET_H
