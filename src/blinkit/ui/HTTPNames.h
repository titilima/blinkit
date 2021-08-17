// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HTTPNames_h
#define HTTPNames_h

#include "wtf/text/AtomicString.h"
#include "platform/PlatformExport.h"

// Generated from:
// - blink/in/HTTPNames.in

namespace blink {
namespace HTTPNames {

PLATFORM_EXPORT extern const WTF::AtomicString& Accept;
PLATFORM_EXPORT extern const WTF::AtomicString& Content_Language;
PLATFORM_EXPORT extern const WTF::AtomicString& Content_Type;
PLATFORM_EXPORT extern const WTF::AtomicString& GET;
PLATFORM_EXPORT extern const WTF::AtomicString& POST;
PLATFORM_EXPORT extern const WTF::AtomicString& Referer;
PLATFORM_EXPORT extern const WTF::AtomicString& User_Agent;

const unsigned HTTPNamesCount = 7;

PLATFORM_EXPORT void init();

} // HTTPNames
} // namespace blink

#endif
