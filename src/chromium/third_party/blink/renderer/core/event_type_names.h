// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_type_names.h
// Description: Event Type Names
//      Author: Ziming Li
//     Created: 2019-11-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EVENT_TYPE_NAMES_H
#define BLINKIT_BLINK_EVENT_TYPE_NAMES_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {
namespace event_type_names {

extern const WTF::AtomicString &kDOMContentLoaded;
extern const WTF::AtomicString &kLoad;

constexpr unsigned kNamesCount = 2;

void Init(void);

} // namespace event_type_names
} // namespace blink

#endif // BLINKIT_BLINK_EVENT_TYPE_NAMES_H
