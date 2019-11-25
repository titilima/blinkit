// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_element_lookup_trie.cpp
// Description: lookupHTMLTag
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "html_element_lookup_trie.h"

namespace blink {

const AtomicString& lookupHTMLTag(const UChar *data, unsigned length)
{
    ASSERT(nullptr != data);
    ASSERT(0 != length);
    ASSERT(false); // BKTODO:
    return g_null_atom;
}

} // namespace blink
