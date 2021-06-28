// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebData.cpp
// Description: WebData Class
//      Author: Ziming Li
//     Created: 2021-06-28
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "public/platform/WebData.h"

#include "platform/SharedBuffer.h"

namespace blink {

void WebData::reset()
{
    m_data.reset();
}

#if 0 // BKTODO:
void WebData::assign(const WebData& other)
{
    m_private = other.m_private;
}
#endif

void WebData::assign(const char* data, size_t size)
{
    m_data = SharedBuffer::create(data, size);
}

size_t WebData::size() const
{
    return m_data ? m_data->size() : 0;
}

const char* WebData::data() const
{
    return m_data ? m_data->data() : nullptr;
}

#if 0 // BKTODO:
WebData::WebData(const PassRefPtr<SharedBuffer>& buffer)
    : m_private(buffer)
{
}

WebData& WebData::operator=(const PassRefPtr<SharedBuffer>& buffer)
{
    m_private = buffer;
    return *this;
}

WebData::operator PassRefPtr<SharedBuffer>() const
{
    return PassRefPtr<SharedBuffer>(m_private.get());
}
#endif

} // namespace blink
