// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ListHashSet.h
// Description: ListHashSet Class
//      Author: Ziming Li
//     Created: 2020-09-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LIST_HASH_SET_H
#define BLINKIT_BLINKIT_LIST_HASH_SET_H

#pragma once

#include "blinkit/blink/renderer/wtf/LinkedHashSet.h"

namespace WTF {

template <typename T, size_t inlineCapacity = 0>
class ListHashSet : public LinkedHashSet<T>
{
};

} // namespace WTF

using WTF::ListHashSet;

#endif // BLINKIT_BLINKIT_LIST_HASH_SET_H
