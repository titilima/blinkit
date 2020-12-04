// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: page_transition_event.cc
// Description: PageTransitionEvent Class
//      Author: Ziming Li
//     Created: 2020-12-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/events/page_transition_event.h"

 // BKTODO: #include "third_party/blink/renderer/core/event_names.h"

namespace blink {

PageTransitionEvent::PageTransitionEvent() : persisted_(false) {}

PageTransitionEvent::PageTransitionEvent(const AtomicString& type,
                                         bool persisted)
    : Event(type, Bubbles::kYes, Cancelable::kYes), persisted_(persisted) {}

#if 0 // BKTODO: Check if necessary.
PageTransitionEvent::PageTransitionEvent(
    const AtomicString& type,
    const PageTransitionEventInit& initializer)
    : Event(type, initializer), persisted_(false) {
  if (initializer.hasPersisted())
    persisted_ = initializer.persisted();
}
#endif

PageTransitionEvent::~PageTransitionEvent() = default;

const AtomicString& PageTransitionEvent::InterfaceName() const {
  ASSERT(false); // BKTODO:
  return g_null_atom;
#if 0
  return EventNames::PageTransitionEvent;
#endif
}

void PageTransitionEvent::Trace(blink::Visitor* visitor) {
  Event::Trace(visitor);
}

}  // namespace blink
