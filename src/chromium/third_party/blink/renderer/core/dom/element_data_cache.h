// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: element_data_cache.h
// Description: ElementDataCache Class
//      Author: Ziming Li
//     Created: 2019-10-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012, 2013 Apple Inc. All Rights Reserved.
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
 *
 */

#ifndef BLINKIT_BLINK_ELEMENT_DATA_CACHE_H
#define BLINKIT_BLINK_ELEMENT_DATA_CACHE_H

#pragma once

#include <unordered_map>
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class Attribute;
class ShareableElementData;

class ElementDataCache final : public GarbageCollectedFinalized<ElementDataCache>
{
public:
    BK_DECLARE_GC_NAME(ElementDataCache)

    static ElementDataCache* Create(void)
    {
        return new ElementDataCache;
    }
    ~ElementDataCache(void);
    void Trace(Visitor *visitor);

    ShareableElementData* CachedShareableElementDataWithAttributes(const Vector<Attribute> &attributes);
private:
    ElementDataCache(void) = default;

    std::unordered_map<unsigned, Member<ShareableElementData>> m_shareableElementDataCache;
};

}  // namespace blink

#endif // BLINKIT_BLINK_ELEMENT_DATA_CACHE_H
