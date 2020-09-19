// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: media_type_names.h
// Description: Media Type Names
//      Author: Ziming Li
//     Created: 2020-09-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_MEDIA_TYPE_NAMES_H
#define BLINKIT_BLINK_MEDIA_TYPE_NAMES_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {
namespace media_type_names {

extern const WTF::AtomicString &kAll;

constexpr unsigned kNamesCount = 1;

void Init(void);

} // namespace media_type_names
} // namespace blink

#endif // BLINKIT_BLINK_MEDIA_TYPE_NAMES_H
