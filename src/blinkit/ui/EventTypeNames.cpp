// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "EventTypeNames.h"

#include "wtf/StdLibExtras.h"

// Generated from:
// - blink/in/EventTypeNames.in

namespace blink {
namespace EventTypeNames {

using namespace WTF;

void* NamesStorage[EventTypeNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString& DOMActivate = reinterpret_cast<AtomicString*>(&NamesStorage)[0];
const AtomicString& DOMContentLoaded = reinterpret_cast<AtomicString*>(&NamesStorage)[1];
const AtomicString& DOMFocusIn = reinterpret_cast<AtomicString*>(&NamesStorage)[2];
const AtomicString& DOMFocusOut = reinterpret_cast<AtomicString*>(&NamesStorage)[3];
const AtomicString& abort = reinterpret_cast<AtomicString*>(&NamesStorage)[4];
const AtomicString& animationend = reinterpret_cast<AtomicString*>(&NamesStorage)[5];
const AtomicString& animationiteration = reinterpret_cast<AtomicString*>(&NamesStorage)[6];
const AtomicString& animationstart = reinterpret_cast<AtomicString*>(&NamesStorage)[7];
const AtomicString& beforecopy = reinterpret_cast<AtomicString*>(&NamesStorage)[8];
const AtomicString& beforecut = reinterpret_cast<AtomicString*>(&NamesStorage)[9];
const AtomicString& beforeunload = reinterpret_cast<AtomicString*>(&NamesStorage)[10];
const AtomicString& blur = reinterpret_cast<AtomicString*>(&NamesStorage)[11];
const AtomicString& change = reinterpret_cast<AtomicString*>(&NamesStorage)[12];
const AtomicString& click = reinterpret_cast<AtomicString*>(&NamesStorage)[13];
const AtomicString& contextmenu = reinterpret_cast<AtomicString*>(&NamesStorage)[14];
const AtomicString& copy = reinterpret_cast<AtomicString*>(&NamesStorage)[15];
const AtomicString& cut = reinterpret_cast<AtomicString*>(&NamesStorage)[16];
const AtomicString& dblclick = reinterpret_cast<AtomicString*>(&NamesStorage)[17];
const AtomicString& drag = reinterpret_cast<AtomicString*>(&NamesStorage)[18];
const AtomicString& dragend = reinterpret_cast<AtomicString*>(&NamesStorage)[19];
const AtomicString& dragenter = reinterpret_cast<AtomicString*>(&NamesStorage)[20];
const AtomicString& dragleave = reinterpret_cast<AtomicString*>(&NamesStorage)[21];
const AtomicString& dragover = reinterpret_cast<AtomicString*>(&NamesStorage)[22];
const AtomicString& dragstart = reinterpret_cast<AtomicString*>(&NamesStorage)[23];
const AtomicString& drop = reinterpret_cast<AtomicString*>(&NamesStorage)[24];
const AtomicString& error = reinterpret_cast<AtomicString*>(&NamesStorage)[25];
const AtomicString& finish = reinterpret_cast<AtomicString*>(&NamesStorage)[26];
const AtomicString& focus = reinterpret_cast<AtomicString*>(&NamesStorage)[27];
const AtomicString& focusin = reinterpret_cast<AtomicString*>(&NamesStorage)[28];
const AtomicString& focusout = reinterpret_cast<AtomicString*>(&NamesStorage)[29];
const AtomicString& invalid = reinterpret_cast<AtomicString*>(&NamesStorage)[30];
const AtomicString& keydown = reinterpret_cast<AtomicString*>(&NamesStorage)[31];
const AtomicString& keypress = reinterpret_cast<AtomicString*>(&NamesStorage)[32];
const AtomicString& keyup = reinterpret_cast<AtomicString*>(&NamesStorage)[33];
const AtomicString& load = reinterpret_cast<AtomicString*>(&NamesStorage)[34];
const AtomicString& loading = reinterpret_cast<AtomicString*>(&NamesStorage)[35];
const AtomicString& loadingdone = reinterpret_cast<AtomicString*>(&NamesStorage)[36];
const AtomicString& loadingerror = reinterpret_cast<AtomicString*>(&NamesStorage)[37];
const AtomicString& mousedown = reinterpret_cast<AtomicString*>(&NamesStorage)[38];
const AtomicString& mouseenter = reinterpret_cast<AtomicString*>(&NamesStorage)[39];
const AtomicString& mouseleave = reinterpret_cast<AtomicString*>(&NamesStorage)[40];
const AtomicString& mousemove = reinterpret_cast<AtomicString*>(&NamesStorage)[41];
const AtomicString& mouseout = reinterpret_cast<AtomicString*>(&NamesStorage)[42];
const AtomicString& mouseover = reinterpret_cast<AtomicString*>(&NamesStorage)[43];
const AtomicString& mouseup = reinterpret_cast<AtomicString*>(&NamesStorage)[44];
const AtomicString& mousewheel = reinterpret_cast<AtomicString*>(&NamesStorage)[45];
const AtomicString& pagehide = reinterpret_cast<AtomicString*>(&NamesStorage)[46];
const AtomicString& pageshow = reinterpret_cast<AtomicString*>(&NamesStorage)[47];
const AtomicString& paste = reinterpret_cast<AtomicString*>(&NamesStorage)[48];
const AtomicString& readystatechange = reinterpret_cast<AtomicString*>(&NamesStorage)[49];
const AtomicString& reset = reinterpret_cast<AtomicString*>(&NamesStorage)[50];
const AtomicString& resize = reinterpret_cast<AtomicString*>(&NamesStorage)[51];
const AtomicString& scroll = reinterpret_cast<AtomicString*>(&NamesStorage)[52];
const AtomicString& search = reinterpret_cast<AtomicString*>(&NamesStorage)[53];
const AtomicString& select = reinterpret_cast<AtomicString*>(&NamesStorage)[54];
const AtomicString& selectionchange = reinterpret_cast<AtomicString*>(&NamesStorage)[55];
const AtomicString& selectstart = reinterpret_cast<AtomicString*>(&NamesStorage)[56];
const AtomicString& submit = reinterpret_cast<AtomicString*>(&NamesStorage)[57];
const AtomicString& textInput = reinterpret_cast<AtomicString*>(&NamesStorage)[58];
const AtomicString& toggle = reinterpret_cast<AtomicString*>(&NamesStorage)[59];
const AtomicString& transitionend = reinterpret_cast<AtomicString*>(&NamesStorage)[60];
const AtomicString& unload = reinterpret_cast<AtomicString*>(&NamesStorage)[61];
const AtomicString& visibilitychange = reinterpret_cast<AtomicString*>(&NamesStorage)[62];
const AtomicString& webkitBeforeTextInserted = reinterpret_cast<AtomicString*>(&NamesStorage)[63];
const AtomicString& webkitEditableContentChanged = reinterpret_cast<AtomicString*>(&NamesStorage)[64];
const AtomicString& webkitvisibilitychange = reinterpret_cast<AtomicString*>(&NamesStorage)[65];
const AtomicString& wheel = reinterpret_cast<AtomicString*>(&NamesStorage)[66];

void init()
{
    struct NameEntry {
        const char* name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
        { "DOMActivate", 1580932, 11 },
        { "DOMContentLoaded", 10217403, 16 },
        { "DOMFocusIn", 13717889, 10 },
        { "DOMFocusOut", 14725020, 11 },
        { "abort", 15390287, 5 },
        { "animationend", 2356336, 12 },
        { "animationiteration", 4399371, 18 },
        { "animationstart", 1171773, 14 },
        { "beforecopy", 15623218, 10 },
        { "beforecut", 6886552, 9 },
        { "beforeunload", 16009443, 12 },
        { "blur", 3880931, 4 },
        { "change", 3980998, 6 },
        { "click", 10125525, 5 },
        { "contextmenu", 14578063, 11 },
        { "copy", 16438426, 4 },
        { "cut", 6810636, 3 },
        { "dblclick", 9054393, 8 },
        { "drag", 1532976, 4 },
        { "dragend", 11982793, 7 },
        { "dragenter", 10351130, 9 },
        { "dragleave", 11297414, 9 },
        { "dragover", 2973271, 8 },
        { "dragstart", 15174021, 9 },
        { "drop", 13413407, 4 },
        { "error", 6654137, 5 },
        { "finish", 15149124, 6 },
        { "focus", 1849182, 5 },
        { "focusin", 13489365, 7 },
        { "focusout", 13124807, 8 },
        { "invalid", 2059736, 7 },
        { "keydown", 5301006, 7 },
        { "keypress", 13845508, 8 },
        { "keyup", 1054547, 5 },
        { "load", 8207817, 4 },
        { "loading", 13228638, 7 },
        { "loadingdone", 6190750, 11 },
        { "loadingerror", 5585167, 12 },
        { "mousedown", 5740218, 9 },
        { "mouseenter", 632853, 10 },
        { "mouseleave", 1144826, 10 },
        { "mousemove", 7356044, 9 },
        { "mouseout", 685204, 8 },
        { "mouseover", 12064397, 9 },
        { "mouseup", 8467273, 7 },
        { "mousewheel", 15891108, 10 },
        { "pagehide", 16640617, 8 },
        { "pageshow", 12320166, 8 },
        { "paste", 369957, 5 },
        { "readystatechange", 11011948, 16 },
        { "reset", 13674204, 5 },
        { "resize", 11716975, 6 },
        { "scroll", 7626286, 6 },
        { "search", 6906057, 6 },
        { "select", 210571, 6 },
        { "selectionchange", 8861459, 15 },
        { "selectstart", 5847864, 11 },
        { "submit", 12328646, 6 },
        { "textInput", 12906447, 9 },
        { "toggle", 8404466, 6 },
        { "transitionend", 9031405, 13 },
        { "unload", 4411490, 6 },
        { "visibilitychange", 11206348, 16 },
        { "webkitBeforeTextInserted", 5393700, 24 },
        { "webkitEditableContentChanged", 1212374, 28 },
        { "webkitvisibilitychange", 440449, 22 },
        { "wheel", 5389519, 5 },
    };

    for (size_t i = 0; i < WTF_ARRAY_LENGTH(kNames); i++) {
        StringImpl* stringImpl = StringImpl::createStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void* address = reinterpret_cast<AtomicString*>(&NamesStorage) + i;
        new (address) AtomicString(stringImpl);
    }
}

} // EventTypeNames
} // namespace blink
