// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EventNames_h
#define EventNames_h

#include "wtf/text/AtomicString.h"
#include "core/CoreExport.h"

// Generated from:
// - blink/in/EventInterfaces.in

namespace blink {
namespace EventNames {

CORE_EXPORT extern const WTF::AtomicString& FontFaceSetLoadEvent;
CORE_EXPORT extern const WTF::AtomicString& MediaQueryListEvent;
CORE_EXPORT extern const WTF::AtomicString& AnimationEvent;
CORE_EXPORT extern const WTF::AtomicString& AnimationPlayerEvent;
CORE_EXPORT extern const WTF::AtomicString& BeforeUnloadEvent;
CORE_EXPORT extern const WTF::AtomicString& ClipboardEvent;
CORE_EXPORT extern const WTF::AtomicString& Event;
CORE_EXPORT extern const WTF::AtomicString& FocusEvent;
CORE_EXPORT extern const WTF::AtomicString& KeyboardEvent;
CORE_EXPORT extern const WTF::AtomicString& MouseEvent;
CORE_EXPORT extern const WTF::AtomicString& PageTransitionEvent;
CORE_EXPORT extern const WTF::AtomicString& TextEvent;
CORE_EXPORT extern const WTF::AtomicString& TransitionEvent;
CORE_EXPORT extern const WTF::AtomicString& UIEvent;
CORE_EXPORT extern const WTF::AtomicString& WheelEvent;

const unsigned EventNamesCount = 15;

CORE_EXPORT void init();

} // EventNames
} // namespace blink

#endif
