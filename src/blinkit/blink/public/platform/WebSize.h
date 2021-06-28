// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebSize.h
// Description: WebSize Struct
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

#ifndef WebSize_h
#define WebSize_h

#include "WebCommon.h"
#include "platform/geometry/IntSize.h"

namespace blink {

struct WebSize {
    int width;
    int height;

    bool isEmpty() const { return width <= 0 || height <= 0; }

    WebSize()
        : width(0)
        , height(0)
    {
    }

    WebSize(int width, int height)
        : width(width)
        , height(height)
    {
    }

    WebSize(const IntSize& s)
        : width(s.width())
        , height(s.height())
    {
    }

    WebSize& operator=(const IntSize& s)
    {
        width = s.width();
        height = s.height();
        return *this;
    }

    operator IntSize() const
    {
        return IntSize(width, height);
    }
};

inline bool operator==(const WebSize& a, const WebSize& b)
{
    return a.width == b.width && a.height == b.height;
}

inline bool operator!=(const WebSize& a, const WebSize& b)
{
    return !(a == b);
}

} // namespace blink

#endif
