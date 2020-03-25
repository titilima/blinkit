// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_type_names.cpp
// Description: Event Type Names
//      Author: Ziming Li
//     Created: 2020-03-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "event_type_names.h"

#include <iterator>

namespace blink {
namespace event_type_names {

static void* names_storage[kNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString &kDOMCharacterDataModified = reinterpret_cast<AtomicString *>(&names_storage)[0];
const AtomicString &kDOMContentLoaded = reinterpret_cast<AtomicString *>(&names_storage)[1];
const AtomicString &kDOMNodeInserted = reinterpret_cast<AtomicString *>(&names_storage)[2];
const AtomicString &kDOMNodeInsertedIntoDocument = reinterpret_cast<AtomicString *>(&names_storage)[3];
const AtomicString &kDOMNodeRemoved = reinterpret_cast<AtomicString *>(&names_storage)[4];
const AtomicString &kDOMNodeRemovedFromDocument = reinterpret_cast<AtomicString *>(&names_storage)[5];
const AtomicString &kDOMSubtreeModified = reinterpret_cast<AtomicString *>(&names_storage)[6];
const AtomicString &kBeforeunload = reinterpret_cast<AtomicString *>(&names_storage)[7];
const AtomicString &kError = reinterpret_cast<AtomicString *>(&names_storage)[8];
const AtomicString &kLoad = reinterpret_cast<AtomicString *>(&names_storage)[9];
const AtomicString &kMousewheel = reinterpret_cast<AtomicString *>(&names_storage)[10];
const AtomicString &kReadystatechange = reinterpret_cast<AtomicString *>(&names_storage)[11];
const AtomicString &kScroll = reinterpret_cast<AtomicString *>(&names_storage)[12];
const AtomicString &kUnload = reinterpret_cast<AtomicString *>(&names_storage)[13];
const AtomicString &kWheel = reinterpret_cast<AtomicString *>(&names_storage)[14];

void Init(void)
{
    static bool s_isLoaded = false;
    if (s_isLoaded)
        return;
    s_isLoaded = true;

    struct NameEntry {
        const char *name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
        { "DOMCharacterDataModified", 1747391, 24 },
        { "DOMContentLoaded", 10217403, 16 },
        { "DOMNodeInserted", 3305523, 15 },
        { "DOMNodeInsertedIntoDocument", 9765719, 27 },
        { "DOMNodeRemoved", 15338285, 14 },
        { "DOMNodeRemovedFromDocument", 16284219, 26 },
        { "DOMSubtreeModified", 6084203, 18 },
        { "beforeunload", 16009443, 12 },
        { "error", 6654137, 5 },
        { "load", 8207817, 4 },
        { "mousewheel", 15891108, 10 },
        { "readystatechange", 11011948, 16 },
        { "scroll", 7626286, 6 },
        { "unload", 4411490, 6 },
        { "wheel", 5389519, 5 },
    };

    for (size_t i = 0; i < std::size(kNames); ++i)
    {
        StringImpl *impl = StringImpl::CreateStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void *address = reinterpret_cast<AtomicString *>(&names_storage) + i;
        new (address) AtomicString(impl);
    }
}

} // namespace event_type_names
} // namespace blink
