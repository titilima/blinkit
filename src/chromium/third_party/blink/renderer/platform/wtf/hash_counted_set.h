// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: hash_counted_set.h
// Description: HashCountedSet Class
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HASH_COUNTED_SET_H
#define BLINKIT_BLINK_HASH_COUNTED_SET_H

#pragma once

#include <unordered_map>

namespace blink {

template <typename T>
class HashCountedSet
{
public:
    bool IsEmpty(void) const { return m_impl.empty(); }

    auto begin(void) { return m_impl.begin(); }
    auto end(void) { return m_impl.end(); }

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

} // namespace blink

#endif // BLINKIT_BLINK_HASH_COUNTED_SET_H
