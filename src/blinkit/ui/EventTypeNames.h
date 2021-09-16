// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EventTypeNames_h
#define EventTypeNames_h

#include "wtf/text/AtomicString.h"
#include "core/CoreExport.h"

// Generated from:
// - blink/in/EventTypeNames.in

namespace blink {
namespace EventTypeNames {

CORE_EXPORT extern const WTF::AtomicString& DOMActivate;
CORE_EXPORT extern const WTF::AtomicString& DOMContentLoaded;
CORE_EXPORT extern const WTF::AtomicString& DOMFocusIn;
CORE_EXPORT extern const WTF::AtomicString& DOMFocusOut;
CORE_EXPORT extern const WTF::AtomicString& abort;
CORE_EXPORT extern const WTF::AtomicString& animationend;
CORE_EXPORT extern const WTF::AtomicString& animationiteration;
CORE_EXPORT extern const WTF::AtomicString& animationstart;
CORE_EXPORT extern const WTF::AtomicString& beforecopy;
CORE_EXPORT extern const WTF::AtomicString& beforecut;
CORE_EXPORT extern const WTF::AtomicString& beforeunload;
CORE_EXPORT extern const WTF::AtomicString& blur;
CORE_EXPORT extern const WTF::AtomicString& change;
CORE_EXPORT extern const WTF::AtomicString& click;
CORE_EXPORT extern const WTF::AtomicString& contextmenu;
CORE_EXPORT extern const WTF::AtomicString& copy;
CORE_EXPORT extern const WTF::AtomicString& cut;
CORE_EXPORT extern const WTF::AtomicString& dblclick;
CORE_EXPORT extern const WTF::AtomicString& drag;
CORE_EXPORT extern const WTF::AtomicString& dragend;
CORE_EXPORT extern const WTF::AtomicString& dragenter;
CORE_EXPORT extern const WTF::AtomicString& dragleave;
CORE_EXPORT extern const WTF::AtomicString& dragover;
CORE_EXPORT extern const WTF::AtomicString& dragstart;
CORE_EXPORT extern const WTF::AtomicString& drop;
CORE_EXPORT extern const WTF::AtomicString& error;
CORE_EXPORT extern const WTF::AtomicString& finish;
CORE_EXPORT extern const WTF::AtomicString& focus;
CORE_EXPORT extern const WTF::AtomicString& focusin;
CORE_EXPORT extern const WTF::AtomicString& focusout;
CORE_EXPORT extern const WTF::AtomicString& invalid;
CORE_EXPORT extern const WTF::AtomicString& keydown;
CORE_EXPORT extern const WTF::AtomicString& keypress;
CORE_EXPORT extern const WTF::AtomicString& keyup;
CORE_EXPORT extern const WTF::AtomicString& load;
CORE_EXPORT extern const WTF::AtomicString& loading;
CORE_EXPORT extern const WTF::AtomicString& loadingdone;
CORE_EXPORT extern const WTF::AtomicString& loadingerror;
CORE_EXPORT extern const WTF::AtomicString& mousedown;
CORE_EXPORT extern const WTF::AtomicString& mouseenter;
CORE_EXPORT extern const WTF::AtomicString& mouseleave;
CORE_EXPORT extern const WTF::AtomicString& mousemove;
CORE_EXPORT extern const WTF::AtomicString& mouseout;
CORE_EXPORT extern const WTF::AtomicString& mouseover;
CORE_EXPORT extern const WTF::AtomicString& mouseup;
CORE_EXPORT extern const WTF::AtomicString& mousewheel;
CORE_EXPORT extern const WTF::AtomicString& pagehide;
CORE_EXPORT extern const WTF::AtomicString& pageshow;
CORE_EXPORT extern const WTF::AtomicString& paste;
CORE_EXPORT extern const WTF::AtomicString& readystatechange;
CORE_EXPORT extern const WTF::AtomicString& reset;
CORE_EXPORT extern const WTF::AtomicString& resize;
CORE_EXPORT extern const WTF::AtomicString& scroll;
CORE_EXPORT extern const WTF::AtomicString& search;
CORE_EXPORT extern const WTF::AtomicString& select;
CORE_EXPORT extern const WTF::AtomicString& selectionchange;
CORE_EXPORT extern const WTF::AtomicString& selectstart;
CORE_EXPORT extern const WTF::AtomicString& submit;
CORE_EXPORT extern const WTF::AtomicString& textInput;
CORE_EXPORT extern const WTF::AtomicString& toggle;
CORE_EXPORT extern const WTF::AtomicString& transitionend;
CORE_EXPORT extern const WTF::AtomicString& unload;
CORE_EXPORT extern const WTF::AtomicString& visibilitychange;
CORE_EXPORT extern const WTF::AtomicString& webkitBeforeTextInserted;
CORE_EXPORT extern const WTF::AtomicString& webkitEditableContentChanged;
CORE_EXPORT extern const WTF::AtomicString& webkitvisibilitychange;
CORE_EXPORT extern const WTF::AtomicString& wheel;

const unsigned EventTypeNamesCount = 67;

CORE_EXPORT void init();

} // EventTypeNames
} // namespace blink

#endif
