// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_target.h
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
 * Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef BLINKIT_BLINK_EVENT_TARGET_H
#define BLINKIT_BLINK_EVENT_TARGET_H

#pragma once

#include "third_party/blink/renderer/core/dom/events/add_event_listener_options_resolved.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatch_result.h"
#include "third_party/blink/renderer/core/dom/events/event_listener_map.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {

class Event;
class ExecutionContext;
class LocalDOMWindow;

struct FiringEventIterator {
    DISALLOW_NEW();
    FiringEventIterator(const AtomicString &eventType, wtf_size_t &iterator, wtf_size_t &end)
        : eventType(eventType), iterator(iterator), end(end)
    {
    }

    const AtomicString &eventType;
    wtf_size_t &iterator;
    wtf_size_t &end;
};
using FiringEventIteratorVector = std::vector<FiringEventIterator>;

class EventTargetData final : public GarbageCollectedFinalized<EventTargetData>
{
public:
    EventTargetData(void) = default;
    ~EventTargetData(void) = default;

    EventListenerMap eventListenerMap;
    std::unique_ptr<FiringEventIteratorVector> firingEventIterators;
private:
    DISALLOW_COPY_AND_ASSIGN(EventTargetData);
};

class EventTarget : public ScriptWrappable
{
public:
    virtual ExecutionContext* GetExecutionContext(void) const = 0;

    virtual const LocalDOMWindow* ToLocalDOMWindow(void) const { return nullptr; }
    virtual Node* ToNode(void) { return nullptr; }

    // Exports for JS
    bool addEventListener(const AtomicString &eventType, EventListener *listener, bool useCapture);
    bool removeEventListener(const AtomicString &eventType, const EventListener *listener, bool useCapture);

    bool HasCapturingEventListeners(const AtomicString &eventType);

    DispatchEventResult DispatchEvent(Event &event);
    DispatchEventResult FireEventListeners(Event &event);
    static DispatchEventResult GetDispatchEventResult(const Event &event);

    virtual bool KeepEventInNode(const Event &event) const { return false; } // BKTODO: This method is only for media controls, so it may be useless.
    virtual void RemoveAllEventListeners(void);
protected:
    EventTarget(void) = default;

    virtual bool AddEventListenerInternal(const AtomicString &eventType, EventListener *listener,
        const AddEventListenerOptionsResolved &options);
    virtual bool RemoveEventListenerInternal(const AtomicString &eventType, const EventListener *listener,
        const EventListenerOptions &options);

    // Called when an event listener has been successfully added.
    virtual void AddedEventListener(const AtomicString& eventType, RegisteredEventListener &registeredListener);
    // Called when an event listener is removed. The original registration
    // parameters of this event listener are available to be queried.
    virtual void RemovedEventListener(const AtomicString &eventType, const RegisteredEventListener &registeredListener);

    virtual DispatchEventResult DispatchEventInternal(Event &event);

    // Subclasses should likely not override these themselves; instead, they
    // should subclass EventTargetWithInlineData.
    virtual EventTargetData* GetEventTargetData(void) = 0;
    virtual EventTargetData& EnsureEventTargetData(void) = 0;
private:
    LocalDOMWindow* ExecutingWindow(void);
    void SetDefaultAddEventListenerOptions(const AtomicString &eventType, EventListener *eventListener,
        AddEventListenerOptionsResolved &options);

    bool FireEventListeners(Event &event, EventTargetData *d, EventListenerVector &entry);
};

class EventTargetWithInlineData : public EventTarget
{
protected:
    EventTargetData* GetEventTargetData(void) final { return &m_eventTargetData; }
    EventTargetData& EnsureEventTargetData(void) final { return m_eventTargetData; }
private:
    EventTargetData m_eventTargetData;
};

} // namespace blink

#endif // BLINKIT_BLINK_EVENT_TARGET_H
