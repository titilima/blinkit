// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_target.cpp
// Description: EventTarget Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 *           (C) 2007, 2008 Nikolas Zimmermann <zimmermann@kde.org>
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "event_target.h"

#include "third_party/blink/renderer/core/dom/events/event.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatch_forbidden_scope.h"

namespace blink {

DispatchEventResult EventTarget::DispatchEvent(Event &event)
{
    event.SetTrusted(true);
    return DispatchEventInternal(event);
}

DispatchEventResult EventTarget::DispatchEventInternal(Event &event)
{
    ASSERT(false); // BKTODO:
    return DispatchEventResult::kCanceledBeforeDispatch;
}

DispatchEventResult EventTarget::FireEventListeners(Event &event)
{
#if DCHECK_IS_ON()
    ASSERT(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif
    ASSERT(event.WasInitialized());

    EventTargetData *d = GetEventTargetData();
    if (nullptr == d)
        return DispatchEventResult::kNotCanceled;

    EventListenerVector *legacyListenersVector = nullptr;
#ifndef BLINKIT_CRAWLER_ONLY
    AtomicString legacyTypeName = LegacyType(event);
    if (!legacy_type_name.IsEmpty())
        legacy_listeners_vector = d->event_listener_map.Find(legacy_type_name);
#endif

    EventListenerVector *listenersVector = d->eventListenerMap.Find(event.type());

    bool firedEventListeners = false;
    if (nullptr != listenersVector)
    {
        ASSERT(false); // BKTODO:
#if 0
        firedEventListeners = FireEventListeners(event, d, *listenersVector);
#endif
    }
    else if (event.isTrusted() && legacyListenersVector)
    {
        ASSERT(false); // BKTODO:
#if 0
        AtomicString unprefixed_type_name = event.type();
        event.SetType(legacy_type_name);
        fired_event_listeners =
            FireEventListeners(event, d, *legacy_listeners_vector);
        event.SetType(unprefixed_type_name);
#endif
    }

    // Only invoke the callback if event listeners were fired for this phase.
    if (firedEventListeners)
    {
        event.DoneDispatchingEventAtCurrentTarget();
        event.SetExecutedListenerOrDefaultAction();
    }
    return GetDispatchEventResult(event);
}

DispatchEventResult EventTarget::GetDispatchEventResult(const Event &event)
{
    if (event.defaultPrevented())
        return DispatchEventResult::kCanceledByEventHandler;
    if (event.DefaultHandled())
        return DispatchEventResult::kCanceledByDefaultEventHandler;
    return DispatchEventResult::kNotCanceled;
}

}  // namespace blink
