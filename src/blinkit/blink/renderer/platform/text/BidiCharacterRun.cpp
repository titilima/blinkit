// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: BidiCharacterRun.cpp
// Description: BidiCharacterRun Class
//      Author: Ziming Li
//     Created: 2021-07-31
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/**
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Andrew Wellington (proton@wiretapped.net)
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

#include "platform/text/BidiCharacterRun.h"

#include "wtf/Partitions.h"
// BKTODO: #include "wtf/RefCountedLeakCounter.h"

using namespace WTF;

namespace blink {
namespace {
#if 0 // BKTODO: ndef NDEBUG
static RefCountedLeakCounter& bidiRunCounter()
{
    DEFINE_STATIC_LOCAL(RefCountedLeakCounter, staticBidiRunCounter, ("BidiCharacterRun"));
    return staticBidiRunCounter;
}
#endif
} // namespace

void* BidiCharacterRun::operator new(size_t sz)
{
#if 0 // BKTODO: ndef NDEBUG
    bidiRunCounter().increment();
#endif
    return partitionAlloc(Partitions::layoutPartition(), sz, WTF_HEAP_PROFILER_TYPE_NAME(BidiCharacterRun));
}

void BidiCharacterRun::operator delete(void* ptr)
{
#if 0 // BKTODO: ndef NDEBUG
    bidiRunCounter().decrement();
#endif
    partitionFree(ptr);
}

}
