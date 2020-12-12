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

#include "third_party/blink/renderer/platform/wtf/linked_hash_set.h"

namespace WTF {

template <typename T, size_t inlineCapacity = 0>
class ListHashSet : public LinkedHashSet<T>
{
};

} // namespace WTF

using WTF::ListHashSet;

#endif // BLINKIT_BLINK_LIST_HASH_SET_H
