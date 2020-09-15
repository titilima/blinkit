// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: fetch_initiator_type_names.cpp
// Description: Fetch Initiator Type Names
//      Author: Ziming Li
//     Created: 2020-09-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "fetch_initiator_type_names.h"

#include <iterator>

namespace blink {
namespace fetch_initiator_type_names {

void* namesStorage[kNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString &kCSS = reinterpret_cast<AtomicString *>(&names_storage)[0];

void Init(void)
{
    static bool isLoaded = false;
    if (isLoaded)
        return;

    isLoaded = true;

    struct NameEntry {
        const char *name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
        { "css", 12506144, 3 },
    };

    for (size_t i = 0; i < std::size(kNames); ++i)
    {
        StringImpl *impl = StringImpl::CreateStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void *address = reinterpret_cast<AtomicString *>(&namesStorage) + i;
        new (address) AtomicString(impl);
    }
}

} // namespace fetch_initiator_type_names
} // namespace blink
