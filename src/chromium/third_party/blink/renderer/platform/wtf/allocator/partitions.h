// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: partitions.h
// Description: Placeholders for WTF Part
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PARTITIONS_H
#define BLINKIT_BLINK_PARTITIONS_H

#pragma once

namespace WTF {

class Partitions
{
public:
    static void* FastMalloc(size_t n, const char *)
    {
        return malloc(n);
    }
    static void* FastZeroedMalloc(size_t n, const char *)
    {
        void *ret = malloc(n);
        memset(ret, 0, n);
        return ret;
    }
    static void* BufferMalloc(size_t n, const char *)
    {
        return malloc(n);
    }
    static void BufferFree(void *p)
    {
        free(p);
    }
    static void FastFree(void *p)
    {
        free(p);
    }
};

} // namespace WTF

#endif  // BLINKIT_BLINK_PARTITIONS_H
