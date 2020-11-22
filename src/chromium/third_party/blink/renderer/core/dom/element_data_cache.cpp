// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: element_data_cache.cpp
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

#include "element_data_cache.h"

#include "third_party/blink/renderer/core/dom/element_data.h"
#include "third_party/blink/renderer/platform/wtf/string_hasher.h"

namespace blink {

ElementDataCache::~ElementDataCache(void) = default;

inline unsigned AttributeHash(const Vector<Attribute> &attributes)
{
    return StringHasher::HashMemory(attributes.data(), attributes.size() * sizeof(Attribute));
}

inline bool HasSameAttributes(const Vector<Attribute> &attributes, ShareableElementData &elementData)
{
    if (attributes.size() != elementData.Attributes().size())
        return false;
    return 0 == memcmp(attributes.data(), elementData.attribute_array_, attributes.size() * sizeof(Attribute));
}

ShareableElementData* ElementDataCache::CachedShareableElementDataWithAttributes(const Vector<Attribute> &attributes)
{
    ASSERT(!attributes.IsEmpty());

    unsigned hash = AttributeHash(attributes);

    auto it = m_shareableElementDataCache.find(hash);
    if (std::end(m_shareableElementDataCache) != it)
    {
        if (HasSameAttributes(attributes, *it->second))
            return it->second;
        else
            return ShareableElementData::CreateWithAttributes(attributes);
    }

    ShareableElementData *ret = ShareableElementData::CreateWithAttributes(attributes);
    m_shareableElementDataCache[hash] = ret;
    return ret;
}

void ElementDataCache::Trace(Visitor *visitor)
{
    visitor->Trace(m_shareableElementDataCache);
}

}  // namespace blink
