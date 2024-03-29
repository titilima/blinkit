#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: int_size_hash.h
// Description: Hashers for IntSize
//      Author: Ziming Li
//     Created: 2021-07-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */
#ifndef IntSizeHash_h
#define IntSizeHash_h

#include "blinkit/blink/renderer/platform/geometry/int_size.h"
#include "blinkit/blink/renderer/wtf/HashMap.h"
#include "blinkit/blink/renderer/wtf/HashSet.h"

namespace WTF {

template<> struct IntHash<blink::IntSize> {
    STATIC_ONLY(IntHash);
    static unsigned hash(const blink::IntSize& key) { return pairIntHash(key.width(), key.height()); }
    static bool equal(const blink::IntSize& a, const blink::IntSize& b) { return a == b; }
    static const bool safeToCompareToEmptyOrDeleted = true;
};

template<> struct DefaultHash<blink::IntSize> {
    STATIC_ONLY(DefaultHash);
    typedef IntHash<blink::IntSize> Hash;
};

template<> struct HashTraits<blink::IntSize> : GenericHashTraits<blink::IntSize> {
    STATIC_ONLY(HashTraits);
    static const bool emptyValueIsZero = true;
    static void constructDeletedValue(blink::IntSize& slot, bool) { new (NotNull, &slot) blink::IntSize(-1, -1); }
    static bool isDeletedValue(const blink::IntSize& value) { return value.width() == -1 && value.height() == -1; }
};

} // namespace WTF

namespace std {
template<> struct hash<blink::IntSize>
{
    std::size_t operator()(const blink::IntSize &s) const noexcept
    {
        return WTF::IntHash<blink::IntSize>::hash(s);
    }
};
}

#endif
