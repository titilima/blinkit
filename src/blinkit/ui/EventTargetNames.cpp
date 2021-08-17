// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "EventTargetNames.h"

#include "wtf/StdLibExtras.h"

// Generated from:
// - blink/in/EventTargetFactory.in

namespace blink {
namespace EventTargetNames {

using namespace WTF;

void* NamesStorage[EventTargetNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString& AnimationPlayer = reinterpret_cast<AtomicString*>(&NamesStorage)[0];
const AtomicString& FontFaceSet = reinterpret_cast<AtomicString*>(&NamesStorage)[1];
const AtomicString& MediaQueryList = reinterpret_cast<AtomicString*>(&NamesStorage)[2];
const AtomicString& Node = reinterpret_cast<AtomicString*>(&NamesStorage)[3];
const AtomicString& DOMWindow = reinterpret_cast<AtomicString*>(&NamesStorage)[4];

void init()
{
    struct NameEntry {
        const char* name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
        { "AnimationPlayer", 8316901, 15 },
        { "FontFaceSet", 15992392, 11 },
        { "MediaQueryList", 4446762, 14 },
        { "Node", 7481749, 4 },
        { "DOMWindow", 5154002, 9 },
    };

    for (size_t i = 0; i < WTF_ARRAY_LENGTH(kNames); i++) {
        StringImpl* stringImpl = StringImpl::createStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void* address = reinterpret_cast<AtomicString*>(&NamesStorage) + i;
        new (address) AtomicString(stringImpl);
    }
}

} // EventTargetNames
} // namespace blink
