// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: aligned_memory.h
// Description: Size-aligned Memory Helpers
//      Author: Ziming Li
//     Created: 2020-12-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_ALIGNED_MEMORY_H
#define BLINKIT_BASE_ALIGNED_MEMORY_H

#pragma once

#include <cstdlib>
#include "build/build_config.h"

namespace base {

inline void* AlignedAlloc(size_t size, size_t alignment)
{
#ifdef COMPILER_MSVC
    return _aligned_malloc(size, alignment);
#else
    return std::aligned_alloc(size, alignment);
#endif
}

struct AlignedFreeDeleter {
    inline void operator()(void *ptr) const
    {
#ifdef COMPILER_MSVC
        return _aligned_free(ptr);
#else
        std::free(ptr);
#endif
    }
};

} // namespace base

#endif // BLINKIT_BASE_ALIGNED_MEMORY_H
