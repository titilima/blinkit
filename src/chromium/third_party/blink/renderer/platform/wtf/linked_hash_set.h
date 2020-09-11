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
#include <unordered_set>

namespace WTF {

template <typename T, typename U, typename V, typename W>
class LinkedHashSet : public std::unordered_set<T>
{
public:
    bool Contains(const T &o) const { return std::unordered_set<T>::end() != std::unordered_set<T>::find(o); }
};

} // namespace WTF

using WTF::LinkedHashSet;

#endif // BLINKIT_BLINK_LINKED_HASH_SET_H
