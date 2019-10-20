// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_tokenizer_names.h
// Description: HTML Tokenizer Names
//      Author: Ziming Li
//     Created: 2019-10-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HTML_TOKENIZER_NAMES_H
#define BLINKIT_BLINK_HTML_TOKENIZER_NAMES_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {
namespace html_tokenizer_names {

extern const WTF::AtomicString &kDoctype;
extern const WTF::AtomicString &kPublic;
extern const WTF::AtomicString &kSystem;
extern const WTF::AtomicString &kCdata;
extern const WTF::AtomicString &kDashDash;

constexpr unsigned kNamesCount = 5;

void Init(void);

} // namespace html_tokenizer_names
} // namespace blink

#endif // BLINKIT_BLINK_HTML_TOKENIZER_NAMES_H

