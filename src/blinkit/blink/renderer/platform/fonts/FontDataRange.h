/*
 * Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef FontDataRange_h
#define FontDataRange_h

#include "platform/fonts/FontData.h"
#include "platform/fonts/SimpleFontData.h"
#include "wtf/Allocator.h"
#include "wtf/text/CharacterNames.h"

namespace blink {

class SimpleFontData;

struct FontDataRange {
    DISALLOW_NEW_EXCEPT_PLACEMENT_NEW();
    explicit FontDataRange(PassRefPtr<SimpleFontData> fontData)
        : m_from(0)
        , m_to(kMaxCodepoint)
        , m_fontData(fontData)
    {
    }

    FontDataRange()
        : m_from(0)
        , m_to(kMaxCodepoint)
        , m_fontData(nullptr)
    {
    }

    explicit FontDataRange(UChar32 from, UChar32 to, PassRefPtr<SimpleFontData> fontData)
        : m_from(from)
        , m_to(to)
        , m_fontData(fontData)
    {
    }

    UChar32 from() const { return m_from; }
    UChar32 to() const { return m_to; }
    bool contains(UChar32 testChar) { return testChar >= m_from && testChar <= m_to; }
    bool isEntireRange() const { return !m_from && m_to >= kMaxCodepoint; }
    PassRefPtr<SimpleFontData> fontData() const { return m_fontData; }

private:
    UChar32 m_from;
    UChar32 m_to;
    RefPtr<SimpleFontData> m_fontData;
};

}

#endif
