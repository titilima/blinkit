// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "MediaTypeNames.h"

#include "wtf/StdLibExtras.h"

// Generated from:
// - blink/in/MediaTypeNames.in

namespace blink {
namespace MediaTypeNames {

using namespace WTF;

void* NamesStorage[MediaTypeNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString& all = reinterpret_cast<AtomicString*>(&NamesStorage)[0];
const AtomicString& speech = reinterpret_cast<AtomicString*>(&NamesStorage)[1];

void init()
{
    struct NameEntry {
        const char* name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
        { "all", 10505778, 3 },
        { "speech", 4509099, 6 },
    };

    for (size_t i = 0; i < WTF_ARRAY_LENGTH(kNames); i++) {
        StringImpl* stringImpl = StringImpl::createStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void* address = reinterpret_cast<AtomicString*>(&NamesStorage) + i;
        new (address) AtomicString(stringImpl);
    }
}

} // MediaTypeNames
} // namespace blink
