// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: fetch_initiator_type_names.h
// Description: Fetch Initiator Type Names
//      Author: Ziming Li
//     Created: 2020-09-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_FETCH_INITIATOR_TYPE_NAMES_H
#define BLINKIT_BLINK_FETCH_INITIATOR_TYPE_NAMES_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {
namespace fetch_initiator_type_names {

extern const WTF::AtomicString &kCSS;

constexpr unsigned kNamesCount = 1;

void Init();

} // namespace fetch_initiator_type_names
} // namespace blink

#endif // BLINKIT_BLINK_FETCH_INITIATOR_TYPE_NAMES_H
