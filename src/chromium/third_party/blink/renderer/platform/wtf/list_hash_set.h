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
#include <unordered_set>

namespace WTF {

template <typename T>
class ListHashSet : public std::unordered_set<T>
{
public:
    bool IsEmpty(void) const { return this->empty(); }
};

} // namespace WTF

using WTF::ListHashSet;

#endif // BLINKIT_BLINK_LIST_HASH_SET_H
