// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: language.cpp
// Description: Language Helpers
//      Author: Ziming Li
//     Created: 2019-10-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010, 2013 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "language.h"

#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace blink {

// Main thread static AtomicString. This can be safely shared across threads.
const AtomicString *g_platformLanguage = nullptr;

static AtomicString CanonicalizePlatformLanguage(void)
{
    String s = Platform::Current()->DefaultLocale();
    if (!s.IsEmpty())
    {
        s.Replace('_', '-');

        const LChar *chars = s.Characters8();
        const unsigned length = s.length();
        const unsigned hash = StringHasher::ComputeHashAndMaskTop8Bits(chars, length);
        StringImpl *impl = StringImpl::CreateStatic(reinterpret_cast<const char *>(chars), length, hash);
        return AtomicString(impl);
    }
    return AtomicString();
}

void InitializePlatformLanguage(void)
{
    assert(IsMainThread());
    static const AtomicString s_platformLanguage = CanonicalizePlatformLanguage();
    g_platformLanguage = &s_platformLanguage;
}

AtomicString DefaultLanguage(void)
{
    return *g_platformLanguage;
}

Vector<AtomicString> UserPreferredLanguages(void)
{
    Vector<AtomicString> languages;
    languages.ReserveInitialCapacity(1);
    languages.push_back(*g_platformLanguage);
    return languages;
}

}  // namespace blink
