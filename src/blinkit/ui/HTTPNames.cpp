// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "HTTPNames.h"

#include "wtf/StdLibExtras.h"

// Generated from:
// - blink/in/HTTPNames.in

namespace blink {
namespace HTTPNames {

using namespace WTF;

void* NamesStorage[HTTPNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString& Accept = reinterpret_cast<AtomicString*>(&NamesStorage)[0];
const AtomicString& Content_Language = reinterpret_cast<AtomicString*>(&NamesStorage)[1];
const AtomicString& Content_Type = reinterpret_cast<AtomicString*>(&NamesStorage)[2];
const AtomicString& GET = reinterpret_cast<AtomicString*>(&NamesStorage)[3];
const AtomicString& POST = reinterpret_cast<AtomicString*>(&NamesStorage)[4];
const AtomicString& Referer = reinterpret_cast<AtomicString*>(&NamesStorage)[5];
const AtomicString& User_Agent = reinterpret_cast<AtomicString*>(&NamesStorage)[6];

void init()
{
    struct NameEntry {
        const char* name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
        { "Accept", 16044384, 6 },
        { "Content-Language", 3105184, 16 },
        { "Content-Type", 7448957, 12 },
        { "GET", 1490282, 3 },
        { "POST", 13393727, 4 },
        { "Referer", 15797945, 7 },
        { "User-Agent", 13018365, 10 },
    };

    for (size_t i = 0; i < WTF_ARRAY_LENGTH(kNames); i++) {
        StringImpl* stringImpl = StringImpl::createStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void* address = reinterpret_cast<AtomicString*>(&NamesStorage) + i;
        new (address) AtomicString(stringImpl);
    }
}

} // HTTPNames
} // namespace blink
