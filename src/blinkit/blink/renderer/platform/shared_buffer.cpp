// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: shared_buffer.cpp
// Description: SharedBuffer Class
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "./SharedBuffer.h"

#include "third_party/zed/include/zed/memory.hpp"

namespace blink {

SharedBuffer::SharedBuffer(const char *data, size_t length)
{
    if (length > 0)
        append(data, length);
}

std::shared_ptr<SharedBuffer> SharedBuffer::adoptVector(std::vector<char> &v)
{
    auto ret = create();
    if (ret)
        ret->m_data.swap(v);
    return ret;
}

void SharedBuffer::append(const char *data, size_t length)
{
    ASSERT(length > 0);

    size_t oldLength = m_data.size();
    m_data.resize(oldLength + length);
    memcpy(m_data.data() + oldLength, data, length);
}

std::shared_ptr<SharedBuffer> SharedBuffer::create(void)
{
    return zed::wrap_shared(new SharedBuffer);
}

std::shared_ptr<SharedBuffer> SharedBuffer::create(const char *data, size_t length)
{
    return zed::wrap_shared(new SharedBuffer(data, length));
}

std::shared_ptr<SharedBuffer> SharedBuffer::create(const unsigned char* data, size_t length)
{
    return zed::wrap_shared(new SharedBuffer(reinterpret_cast<const char *>(data), length));
}

} // namespace blink
