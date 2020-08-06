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
private:
    std::unordered_map<T, unsigned> m_impl;
};

} // namespace blink

#endif // BLINKIT_BLINK_HASH_COUNTED_SET_H
