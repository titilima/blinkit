// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EventTargetNames_h
#define EventTargetNames_h

#include "wtf/text/AtomicString.h"
#include "core/CoreExport.h"

// Generated from:
// - blink/in/EventTargetFactory.in

namespace blink {
namespace EventTargetNames {

CORE_EXPORT extern const WTF::AtomicString& AnimationPlayer;
CORE_EXPORT extern const WTF::AtomicString& FontFaceSet;
CORE_EXPORT extern const WTF::AtomicString& MediaQueryList;
CORE_EXPORT extern const WTF::AtomicString& Node;
CORE_EXPORT extern const WTF::AtomicString& DOMWindow;

const unsigned EventTargetNamesCount = 5;

CORE_EXPORT void init();

} // EventTargetNames
} // namespace blink

#endif
