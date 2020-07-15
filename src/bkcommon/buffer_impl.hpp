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

#include <string>
#include <vector>
#include "bk_def.h"

namespace BlinKit {

template <typename Container>
class BufferAdaptor {
public:
    BufferAdaptor(Container &data)
    {
        m_buffer.Allocator = Alloc;
        m_buffer.UserData = &data;
    }
    operator BkBuffer* (void) const {
        BkBuffer *buf = const_cast<BkBuffer *>(&m_buffer);
        return buf;
    }
private:
    static void* BKAPI Alloc(size_t size, void *This) {
        Container *c = reinterpret_cast<Container *>(This);
        c->resize(size / sizeof(typename Container::value_type));
        return const_cast<typename Container::value_type *>(c->data());
    }
    BkBuffer m_buffer;
};

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

    template <typename CharType>
    static BufferAdaptor<std::basic_string<CharType>> Wrap(std::basic_string<CharType> &s)
    {
        return BufferAdaptor<std::basic_string<CharType>>(s);
    }

    static BufferAdaptor<std::vector<unsigned char>> Wrap(std::vector<unsigned char> &v)
    {
        return BufferAdaptor<std::vector<unsigned char>>(v);
    }
};

} // namespace BlinKit

#endif // BLINKIT_BKCOMMON_BUFFER_IMPL_HPP
