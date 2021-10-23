// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MediaTypeNames_h
#define MediaTypeNames_h

#include "wtf/text/AtomicString.h"
#include "core/CoreExport.h"

// Generated from:
// - blink/in/MediaTypeNames.in

namespace blink {
namespace MediaTypeNames {

CORE_EXPORT extern const WTF::AtomicString& all;
CORE_EXPORT extern const WTF::AtomicString& screen;

const unsigned MediaTypeNamesCount = 2;

CORE_EXPORT void init();

} // MediaTypeNames
} // namespace blink

#endif
