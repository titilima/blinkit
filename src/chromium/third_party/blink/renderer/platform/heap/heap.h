// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: heap.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2019-09-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HEAP_H
#define BLINKIT_BLINK_HEAP_H

#pragma once

#include "base/gtest_prod_util.h"
#include "base/macros.h"

namespace blink {

template <typename T>
class GarbageCollected
{
protected:
    GarbageCollected(void) = default;

    DISALLOW_COPY_AND_ASSIGN(GarbageCollected);
};


} // namespace blink

#endif // BLINKIT_BLINK_HEAP_H
