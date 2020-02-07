// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_type_names.cpp
// Description: Event Type Names
//      Author: Ziming Li
//     Created: 2020-02-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "event_type_names.h"

#include <iterator>

namespace blink {
namespace event_type_names {

static void* names_storage[kNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString &kDOMContentLoaded = reinterpret_cast<AtomicString *>(&names_storage)[0];
const AtomicString &kError = reinterpret_cast<AtomicString *>(&names_storage)[1];
const AtomicString &kLoad = reinterpret_cast<AtomicString *>(&names_storage)[2];
const AtomicString &kReadystatechange = reinterpret_cast<AtomicString *>(&names_storage)[3];
const AtomicString &kUnload = reinterpret_cast<AtomicString *>(&names_storage)[4];

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
        { "DOMContentLoaded", 10217403, 16 },
        { "error", 6654137, 5 },
        { "load", 8207817, 4 },
        { "readystatechange", 11011948, 16 },
        { "unload", 4411490, 6 },
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
