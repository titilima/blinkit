// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "EventNames.h"

#include "wtf/StdLibExtras.h"

// Generated from:
// - blink/in/EventInterfaces.in

namespace blink {
namespace EventNames {

using namespace WTF;

void* NamesStorage[EventNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString& FontFaceSetLoadEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[0];
const AtomicString& MediaQueryListEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[1];
const AtomicString& AnimationEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[2];
const AtomicString& AnimationPlayerEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[3];
const AtomicString& BeforeUnloadEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[4];
const AtomicString& ClipboardEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[5];
const AtomicString& Event = reinterpret_cast<AtomicString*>(&NamesStorage)[6];
const AtomicString& FocusEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[7];
const AtomicString& KeyboardEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[8];
const AtomicString& MouseEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[9];
const AtomicString& PageTransitionEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[10];
const AtomicString& TextEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[11];
const AtomicString& TransitionEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[12];
const AtomicString& UIEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[13];
const AtomicString& WheelEvent = reinterpret_cast<AtomicString*>(&NamesStorage)[14];

void init()
{
    struct NameEntry {
        const char* name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
        { "FontFaceSetLoadEvent", 3580378, 20 },
        { "MediaQueryListEvent", 2479019, 19 },
        { "AnimationEvent", 6834880, 14 },
        { "AnimationPlayerEvent", 11993449, 20 },
        { "BeforeUnloadEvent", 6032602, 17 },
        { "ClipboardEvent", 10110691, 14 },
        { "Event", 14343017, 5 },
        { "FocusEvent", 8217765, 10 },
        { "KeyboardEvent", 6959608, 13 },
        { "MouseEvent", 12822602, 10 },
        { "PageTransitionEvent", 14165289, 19 },
        { "TextEvent", 9996370, 9 },
        { "TransitionEvent", 4011983, 15 },
        { "UIEvent", 9523113, 7 },
        { "WheelEvent", 580237, 10 },
    };

    for (size_t i = 0; i < WTF_ARRAY_LENGTH(kNames); i++) {
        StringImpl* stringImpl = StringImpl::createStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void* address = reinterpret_cast<AtomicString*>(&NamesStorage) + i;
        new (address) AtomicString(stringImpl);
    }
}

} // EventNames
} // namespace blink
