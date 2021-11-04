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

size_t SharedBuffer::getSomeData(const char *&data, size_t position) const
{
    if (position >= m_data.size())
    {
        data = nullptr;
        return 0;
    }

    ASSERT(position < m_data.size());
    data = m_data.data();
    return m_data.size();
}

bool SharedBuffer::isLocked(void) const
{
    return true; // BKTODO: Check the logic.
}

#ifdef BLINKIT_UI_ENABLED
PassRefPtr<SkData> SharedBuffer::getAsSkData() const
{
    size_t bufferLength = size();

    SkData *data = SkData::NewUninitialized(bufferLength);
    char *buffer = static_cast<char *>(data->writable_data());
    const char *segment = nullptr;
    size_t position = 0;
    while (size_t segmentSize = getSomeData(segment, position))
    {
        memcpy(buffer + position, segment, segmentSize);
        position += segmentSize;
    }

    if (position != bufferLength)
    {
        ASSERT_NOT_REACHED();
        // Don't return the incomplete SkData.
        return nullptr;
    }
    return adoptRef(data);
}
#endif

} // namespace blink
