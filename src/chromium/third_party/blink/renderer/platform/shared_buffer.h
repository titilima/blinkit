// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: shared_buffer.h
// Description: SharedBuffer Class
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) Research In Motion Limited 2009-2010. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_SHARED_BUFFER_H
#define BLINKIT_BLINK_SHARED_BUFFER_H

#pragma once

#include <vector>

namespace blink {

class SharedBuffer : public std::enable_shared_from_this<SharedBuffer>
{
public:
    static std::shared_ptr<SharedBuffer> Create(void);
    static std::shared_ptr<SharedBuffer> Create(const char *data, size_t length);

    // Iterator for ShreadBuffer contents. An Iterator will get invalid once the
    // associated SharedBuffer is modified (e.g., Append() is called). An Iterator
    // doesn't retain the associated container.
    class Iterator final
    {
    public:
        ~Iterator(void) = default;

        Iterator& operator++();
        bool operator==(const Iterator &o) const { return m_data == o.m_data; }
        bool operator!=(const Iterator &o) const { return !(*this == o); }
        const std::vector<char>& operator*() const { return *m_data; }

        const char* data(void) const { return m_data->data(); }
        size_t size(void) const { return m_data->size(); }
    private:
        friend class SharedBuffer;
        Iterator(const std::vector<char> *data) : m_data(data) {}

        const std::vector<char> *m_data = nullptr;
    };

    Iterator begin(void) const;
    Iterator end(void) const;
    size_t size(void) const { return m_data.size(); }

    void Append(const char *data, size_t length);

    bool GetBytes(void *dest, size_t destSize) const;
private:
    SharedBuffer(void) = default;
    SharedBuffer(const char *data, size_t length);

    std::vector<char> m_data;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_PLATFORM_SHARED_BUFFER_H_
