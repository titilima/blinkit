// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_tokenizer_names.cpp
// Description: HTML Tokenizer Names
//      Author: Ziming Li
//     Created: 2019-10-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "html_tokenizer_names.h"

#include <iterator>

namespace blink {
namespace html_tokenizer_names {

void* names_storage[kNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

const AtomicString &kDashDash = reinterpret_cast<AtomicString*>(&names_storage)[0];
const AtomicString &kCdata = reinterpret_cast<AtomicString*>(&names_storage)[1];
const AtomicString &kDoctype = reinterpret_cast<AtomicString*>(&names_storage)[2];
const AtomicString &kPublic = reinterpret_cast<AtomicString*>(&names_storage)[3];
const AtomicString &kSystem = reinterpret_cast<AtomicString*>(&names_storage)[4];

void Init(void)
{
    static bool is_loaded = false;
    if (is_loaded)
        return;

    is_loaded = true;

    struct NameEntry {
        const char *name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
      { "--",       7376722, 2 },
      { "[CDATA[", 11402496, 7 },
      { "doctype", 10540185, 7 },
      { "public",  14399889, 6 },
      { "system",   7974370, 6 },
    };

    for (size_t i = 0; i < std::size(kNames); ++i)
    {
        StringImpl *impl = StringImpl::CreateStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void *address = reinterpret_cast<AtomicString *>(&names_storage) + i;
        new (address) AtomicString(impl);
    }
}

} // namespace html_tokenizer_names
} // namespace blink
