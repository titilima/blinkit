// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: media_type_names.cpp
// Description: Media Type Names
//      Author: Ziming Li
//     Created: 2020-09-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "media_type_names.h"

#include <utility>

namespace blink {
namespace media_type_names {

void *namesStorage[kNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString &kAll = reinterpret_cast<AtomicString*>(&namesStorage)[0];

void Init(void)
{
    static bool isLoaded = false;
    if (isLoaded)
        return;

    isLoaded = true;

    struct NameEntry {
        const char* name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
      { "all", 10505778, 3 },
    };

    for (size_t i = 0; i < std::size(kNames); ++i)
    {
        StringImpl *impl = StringImpl::CreateStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void *address = reinterpret_cast<AtomicString*>(&namesStorage) + i;
        new (address) AtomicString(impl);
    }
}

} // namespace media_type_names
} // namespace blink
