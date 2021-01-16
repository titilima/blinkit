// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: shared_buffer.cpp
// Description: SharedBuffer Class
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2008 Apple Inc. All rights reserved.
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

#include "shared_buffer.h"

#include "base/memory/ptr_util.h"

namespace blink {

SharedBuffer::Iterator& SharedBuffer::Iterator::operator++()
{
    ASSERT(nullptr != m_data);
    m_data = nullptr;
    return *this;
}

SharedBuffer::SharedBuffer(const char *data, size_t length)
{
    if (length > 0)
        Append(data, length);
}

void SharedBuffer::Append(const char *data, size_t length)
{
    ASSERT(length > 0);

    size_t oldLength = m_data.size();
    m_data.resize(oldLength + length);
    memcpy(m_data.data() + oldLength, data, length);
}

SharedBuffer::Iterator SharedBuffer::begin(void) const
{
    return Iterator(&m_data);
}

std::shared_ptr<SharedBuffer> SharedBuffer::Create(void)
{
    return base::WrapShared(new SharedBuffer);
}

std::shared_ptr<SharedBuffer> SharedBuffer::Create(const char *data, size_t length)
{
    return base::WrapShared(new SharedBuffer(data, length));
}

SharedBuffer::Iterator SharedBuffer::end(void) const
{
    return Iterator(nullptr);
}

bool SharedBuffer::GetBytes(void *dest, size_t destSize) const
{
    if (nullptr == dest)
        return false;
    if (m_data.size() < destSize)
        return false;
    memcpy(dest, m_data.data(), destSize);
    return true;
}

}  // namespace blink
