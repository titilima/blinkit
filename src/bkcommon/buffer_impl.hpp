// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: buffer_impl.hpp
// Description: Buffer Implementations
//      Author: Ziming Li
//     Created: 2020-06-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKCOMMON_BUFFER_IMPL_HPP
#define BLINKIT_BKCOMMON_BUFFER_IMPL_HPP

#pragma once

#include "bk_def.h"

namespace BlinKit {

class BufferImpl
{
public:
    template <class T>
    static void Set(BkBuffer *dst, const T &src)
    {
        Set(dst, src.data(), src.size());
    }

    static void Set(BkBuffer *dst, const void *src, size_t size)
    {
        ASSERT(nullptr != dst->Allocator);
        if (size > 0)
        {
            void *buf = dst->Allocator(size, dst->UserData);
            memcpy(buf, src, size);
        }
    }
};

} // namespace BlinKit

#endif // BLINKIT_BKCOMMON_BUFFER_IMPL_HPP
