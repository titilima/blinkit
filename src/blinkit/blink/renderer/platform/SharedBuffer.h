#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: SharedBuffer.h
// Description: SharedBuffer Class
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_SHARED_BUFFER_H
#define BLINKIT_BLINKIT_SHARED_BUFFER_H

namespace blink {

class SharedBuffer : public std::enable_shared_from_this<SharedBuffer>
{
public:
    static std::shared_ptr<SharedBuffer> create(const char *data, size_t length);

    const char* data(void) const { return m_data.empty() ? nullptr : m_data.data(); }
    size_t size(void) const { return m_data.size(); }

    void append(const char *data, size_t length);
private:
    SharedBuffer(const char *data, size_t length);

    std::vector<char> m_data;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_SHARED_BUFFER_H
