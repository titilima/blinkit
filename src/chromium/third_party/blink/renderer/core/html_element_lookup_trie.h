// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_element_lookup_trie.h
// Description: lookupHTMLTag
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HTML_ELEMENT_LOOKUP_TRIE_H
#define BLINKIT_BLINK_HTML_ELEMENT_LOOKUP_TRIE_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {

const AtomicString& lookupHTMLTag(const UChar *data, unsigned length);

} // namespace blink

#endif // BLINKIT_BLINK_HTML_ELEMENT_LOOKUP_TRIE_H
