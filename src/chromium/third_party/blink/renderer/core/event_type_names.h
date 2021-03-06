// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_type_names.h
// Description: Event Type Names
//      Author: Ziming Li
//     Created: 2021-02-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EVENT_TYPE_NAMES_H
#define BLINKIT_BLINK_EVENT_TYPE_NAMES_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {
namespace event_type_names {

extern const WTF::AtomicString &kDOMCharacterDataModified;
extern const WTF::AtomicString &kDOMContentLoaded;
extern const WTF::AtomicString &kDOMNodeInserted;
extern const WTF::AtomicString &kDOMNodeInsertedIntoDocument;
extern const WTF::AtomicString &kDOMNodeRemoved;
extern const WTF::AtomicString &kDOMNodeRemovedFromDocument;
extern const WTF::AtomicString &kDOMSubtreeModified;
extern const WTF::AtomicString &kBeforeunload;
extern const WTF::AtomicString &kBlur;
extern const WTF::AtomicString &kError;
extern const WTF::AtomicString &kFocus;
extern const WTF::AtomicString &kFocusin;
extern const WTF::AtomicString &kFocusout;
extern const WTF::AtomicString &kLoad;
extern const WTF::AtomicString &kMousewheel;
extern const WTF::AtomicString &kPagehide;
extern const WTF::AtomicString &kPageshow;
extern const WTF::AtomicString &kReadystatechange;
extern const WTF::AtomicString &kResize;
extern const WTF::AtomicString &kScroll;
extern const WTF::AtomicString &kUnload;
extern const WTF::AtomicString &kVisibilitychange;
extern const WTF::AtomicString &kWheel;

constexpr unsigned kNamesCount = 23;

void Init(void);

} // namespace event_type_names
} // namespace blink

#endif // BLINKIT_BLINK_EVENT_TYPE_NAMES_H
