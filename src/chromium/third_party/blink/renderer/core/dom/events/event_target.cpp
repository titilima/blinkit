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
#include "third_party/blink/renderer/core/dom/events/event_listener.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"

namespace blink {

static Event::PassiveMode EventPassiveMode(const RegisteredEventListener &eventListener)
{
    if (!eventListener.Passive())
    {
        if (eventListener.PassiveSpecified())
            return Event::PassiveMode::kNotPassive;
        return Event::PassiveMode::kNotPassiveDefault;
    }
    if (eventListener.PassiveForcedForDocumentTarget())
        return Event::PassiveMode::kPassiveForcedDocumentLevel;
    if (eventListener.PassiveSpecified())
        return Event::PassiveMode::kPassive;
    return Event::PassiveMode::kPassiveDefault;
}

static bool IsTouchScrollBlockingEvent(const AtomicString &eventType)
{
    return false;
#if 0 // BKTODO:
    return eventType == EventTypeNames::touchstart || eventType == EventTypeNames::touchmove;
#endif
}

static bool IsWheelScrollBlockingEvent(const AtomicString &eventType)
{
    return eventType == event_type_names::kMousewheel || eventType == event_type_names::kWheel;
}

static bool IsScrollBlockingEvent(const AtomicString &eventType)
{
    return IsTouchScrollBlockingEvent(eventType) || IsWheelScrollBlockingEvent(eventType);
}

bool EventTarget::AddEventListenerInternal(
    const AtomicString &eventType,
    EventListener *listener,
    const AddEventListenerOptionsResolved &options)
{
    if (nullptr == listener)
        return false;

    RegisteredEventListener registeredListener;
    bool added = EnsureEventTargetData().eventListenerMap.Add(eventType, listener, options, &registeredListener);
    if (added)
        AddedEventListener(eventType, registeredListener);
    return added;
}

bool EventTarget::addEventListener(const AtomicString &eventType, EventListener *listener, bool useCapture)
{
    AddEventListenerOptionsResolved options;
    options.setCapture(useCapture);
    SetDefaultAddEventListenerOptions(eventType, listener, options);
    return AddEventListenerInternal(eventType, listener, options);
}

void EventTarget::AddedEventListener(const AtomicString& eventType, RegisteredEventListener &registeredListener)
{
    // Currently nothing to do.
}

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

inline LocalDOMWindow* EventTarget::ExecutingWindow(void)
{
    if (ExecutionContext *context = GetExecutionContext())
        return context->ExecutingWindow();
    return nullptr;
}

#ifndef BLINKIT_CRAWLER_ONLY
static const AtomicString& LegacyType(const Event &event)
{
    BKLOG("// BKTODO: Translate event type names.");
#if 0
    if (event.type() == EventTypeNames::transitionend)
        return EventTypeNames::webkitTransitionEnd;
    if (event.type() == EventTypeNames::animationstart)
        return EventTypeNames::webkitAnimationStart;
    if (event.type() == EventTypeNames::animationend)
        return EventTypeNames::webkitAnimationEnd;
    if (event.type() == EventTypeNames::animationiteration)
        return EventTypeNames::webkitAnimationIteration;
    if (event.type() == EventTypeNames::wheel)
        return EventTypeNames::mousewheel;
#endif
    return g_empty_atom;
}
#endif

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
    if (!legacyTypeName.IsEmpty())
        legacyListenersVector = d->eventListenerMap.Find(legacyTypeName);
#endif

    EventListenerVector *listenersVector = d->eventListenerMap.Find(event.type());

    bool firedEventListeners = false;
    if (nullptr != listenersVector)
    {
        firedEventListeners = FireEventListeners(event, d, *listenersVector);
    }
#ifndef BLINKIT_CRAWLER_ONLY
    else if (event.isTrusted() && legacyListenersVector)
    {
        AtomicString unprefixedTypeName = event.type();
        ASSERT(false); // BKTODO: event.SetType(legacyTypeName);
        firedEventListeners = FireEventListeners(event, d, *legacyListenersVector);
        event.SetType(unprefixedTypeName);
    }
#endif

    // Only invoke the callback if event listeners were fired for this phase.
    if (firedEventListeners)
    {
        event.DoneDispatchingEventAtCurrentTarget();
        event.SetExecutedListenerOrDefaultAction();
    }
    return GetDispatchEventResult(event);
}

bool EventTarget::FireEventListeners(Event &event, EventTargetData *d, EventListenerVector &entry)
{
    // Fire all listeners registered for this event. Don't fire listeners removed
    // during event dispatch. Also, don't fire event listeners added during event
    // dispatch. Conveniently, all new event listeners will be added after or at
    // index |size|, so iterating up to (but not including) |size| naturally
    // excludes new event listeners.
    ExecutionContext *context = GetExecutionContext();
    if (nullptr == context)
        return false;

    wtf_size_t i = 0;
    wtf_size_t size = entry.size();
    if (!d->firingEventIterators)
        d->firingEventIterators = std::make_unique<FiringEventIteratorVector>();
    d->firingEventIterators->push_back(FiringEventIterator(event.type(), i, size));

    bool firedListener = false;
    while (i < size)
    {
        RegisteredEventListener registeredListener = entry[i];

        // Move the iterator past this event listener. This must match
        // the handling of the FiringEventIterator::iterator in
        // EventTarget::removeEventListener.
        ++i;

        if (!registeredListener.ShouldFire(event))
            continue;

        EventListener *listener = registeredListener.Callback();
        // The listener will be retained by Member<EventListener> in the
        // registeredListener, i and size are updated with the firing event iterator
        // in case the listener is removed from the listener vector below.
        if (registeredListener.Once())
            removeEventListener(event.type(), listener, registeredListener.Capture());

        // If stopImmediatePropagation has been called, we just break out
        // immediately, without handling any more events on this target.
        if (event.ImmediatePropagationStopped())
            break;

        event.SetHandlingPassive(EventPassiveMode(registeredListener));
        bool passiveForced = registeredListener.PassiveForcedForDocumentTarget();

        // To match Mozilla, the AT_TARGET phase fires both capturing and bubbling
        // event listeners, even though that violates some versions of the DOM spec.
        listener->handleEvent(context, &event);
        firedListener = true;

        event.SetHandlingPassive(Event::PassiveMode::kNotPassive);

        ASSERT(i <= size);
    }
    d->firingEventIterators->pop_back();
    return firedListener;
}

RegisteredEventListener* EventTarget::GetAttributeRegisteredEventListener(const AtomicString &eventType)
{
    EventListenerVector *listenerVector = GetEventListeners(eventType);
    if (nullptr == listenerVector)
        return nullptr;

    for (auto &eventListener : *listenerVector)
    {
        EventListener *listener = eventListener.Callback();
        if (listener->IsEventHandler() && listener->BelongsToTheCurrentWorld(GetExecutionContext()))
            return &eventListener;
    }
    return nullptr;
}

DispatchEventResult EventTarget::GetDispatchEventResult(const Event &event)
{
    if (event.defaultPrevented())
        return DispatchEventResult::kCanceledByEventHandler;
    if (event.DefaultHandled())
        return DispatchEventResult::kCanceledByDefaultEventHandler;
    return DispatchEventResult::kNotCanceled;
}

EventListenerVector* EventTarget::GetEventListeners(const AtomicString &eventType)
{
    EventTargetData *data = GetEventTargetData();
    if (nullptr == data)
        return nullptr;
    return data->eventListenerMap.Find(eventType);
}

bool EventTarget::HasCapturingEventListeners(const AtomicString &eventType)
{
    EventTargetData *d = GetEventTargetData();
    if (nullptr == d)
        return false;
    return d->eventListenerMap.ContainsCapturing(eventType);
}

void EventTarget::RemoveAllEventListeners(void)
{
    EventTargetData *d = GetEventTargetData();
    if (nullptr == d)
        return;
    d->eventListenerMap.Clear();

    // Notify firing events planning to invoke the listener at 'index' that
    // they have one less listener to invoke.
    if (d->firingEventIterators)
    {
        for (const auto &iterator : *d->firingEventIterators)
        {
            iterator.iterator = 0;
            iterator.end = 0;
        }
    }
}

bool EventTarget::removeEventListener(const AtomicString &eventType, const EventListener *listener, bool useCapture)
{
    EventListenerOptions options;
    options.setCapture(useCapture);
    return RemoveEventListenerInternal(eventType, listener, options);
}

void EventTarget::RemovedEventListener(const AtomicString &eventType, const RegisteredEventListener &registeredListener)
{
    // Currently nothing to do.
}

bool EventTarget::RemoveEventListenerInternal(
    const AtomicString &eventType,
    const EventListener *listener,
    const EventListenerOptions &options)
{
    if (nullptr == listener)
        return false;

    EventTargetData *d = GetEventTargetData();
    if (nullptr == d)
        return false;

    wtf_size_t indexOfRemovedListener;
    RegisteredEventListener registeredListener;

    if (!d->eventListenerMap.Remove(eventType, listener, options, &indexOfRemovedListener, &registeredListener))
        return false;

    // Notify firing events planning to invoke the listener at 'index' that
    // they have one less listener to invoke.
    if (d->firingEventIterators)
    {
        for (const auto &firingIterator : *d->firingEventIterators)
        {
            if (eventType != firingIterator.eventType)
                continue;

            if (indexOfRemovedListener >= firingIterator.end)
                continue;

            --firingIterator.end;
            // Note that when firing an event listener,
            // firingIterator.iterator indicates the next event listener
            // that would fire, not the currently firing event
            // listener. See EventTarget::fireEventListeners.
            if (indexOfRemovedListener < firingIterator.iterator)
                --firingIterator.iterator;
        }
    }
    RemovedEventListener(eventType, registeredListener);
    return true;
}

bool EventTarget::SetAttributeEventListener(const AtomicString &eventType, EventListener *listener)
{
    RegisteredEventListener *registeredListener = GetAttributeRegisteredEventListener(eventType);
    if (nullptr == listener)
    {
        if (nullptr != registeredListener)
            removeEventListener(eventType, registeredListener->Callback(), false);
        return false;
    }
    if (nullptr != registeredListener)
    {
        registeredListener->SetCallback(listener);
        return true;
    }
    return addEventListener(eventType, listener, false);
}

void EventTarget::SetDefaultAddEventListenerOptions(
    const AtomicString &eventType,
    EventListener *eventListener,
    AddEventListenerOptionsResolved &options)
{
    options.SetPassiveSpecified(options.hasPassive());

    if (!IsScrollBlockingEvent(eventType))
    {
        if (!options.hasPassive())
            options.setPassive(false);
        return;
    }

#ifdef BLINKIT_CRAWLER_ONLY
    NOTREACHED();
#else
    LocalDOMWindow *executingWindow = ExecutingWindow();

    if (RuntimeEnabledFeatures::PassiveDocumentEventListenersEnabled() && IsTouchScrollBlockingEvent(eventType))
    {
        ASSERT(false); // BKTODO:
#if 0
        if (!options.hasPassive() && IsTopLevelNode())
        {
            options.setPassive(true);
            options.SetPassiveForcedForDocumentTarget(true);
            return;
        }
#endif
    }

    ASSERT(false); // BKTODO:
#if 0
    if (IsWheelScrollBlockingEvent(eventType) && IsTopLevelNode())
    {
        if (options.hasPassive()) {
            if (executing_window) {
                UseCounter::Count(
                    executing_window->document(),
                    options.passive()
                    ? WebFeature::kAddDocumentLevelPassiveTrueWheelEventListener
                    : WebFeature::kAddDocumentLevelPassiveFalseWheelEventListener);
            }
        }
        else {  // !options.hasPassive()
            if (executing_window) {
                UseCounter::Count(
                    executing_window->document(),
                    WebFeature::kAddDocumentLevelPassiveDefaultWheelEventListener);
            }
            if (RuntimeEnabledFeatures::PassiveDocumentWheelEventListenersEnabled()) {
                options.setPassive(true);
                options.SetPassiveForcedForDocumentTarget(true);
                return;
            }
        }
    }

    // For mousewheel event listeners that have the target as the window and
    // a bound function name of "ssc_wheel" treat and no passive value default
    // passive to true. See crbug.com/501568.
    if (RuntimeEnabledFeatures::SmoothScrollJSInterventionEnabled() &&
        event_type == EventTypeNames::mousewheel && ToLocalDOMWindow() &&
        event_listener && !options.hasPassive()) {
        JSBasedEventListener* v8_listener =
            JSBasedEventListener::Cast(event_listener);
        if (!v8_listener)
            return;
        v8::Local<v8::Value> callback_object =
            v8_listener->GetListenerObject(*this);
        if (!callback_object.IsEmpty() && callback_object->IsFunction() &&
            strcmp(
                "ssc_wheel",
                *v8::String::Utf8Value(
                    v8::Isolate::GetCurrent(),
                    v8::Local<v8::Function>::Cast(callback_object)->GetName())) ==
            0) {
            options.setPassive(true);
            if (executing_window) {
                UseCounter::Count(executing_window->document(),
                    WebFeature::kSmoothScrollJSInterventionActivated);

                executing_window->GetFrame()->Console().AddMessage(
                    ConsoleMessage::Create(
                        kInterventionMessageSource, kWarningMessageLevel,
                        "Registering mousewheel event as passive due to "
                        "smoothscroll.js usage. The smoothscroll.js library is "
                        "buggy, no longer necessary and degrades performance. See "
                        "https://www.chromestatus.com/feature/5749447073988608"));
            }
            return;
        }
    }

    if (Settings* settings = WindowSettings(ExecutingWindow())) {
        switch (settings->GetPassiveListenerDefault()) {
        case PassiveListenerDefault::kFalse:
            if (!options.hasPassive())
                options.setPassive(false);
            break;
        case PassiveListenerDefault::kTrue:
            if (!options.hasPassive())
                options.setPassive(true);
            break;
        case PassiveListenerDefault::kForceAllTrue:
            options.setPassive(true);
            break;
        }
    }
    else {
        if (!options.hasPassive())
            options.setPassive(false);
    }

    if (!options.passive() && !options.PassiveSpecified()) {
        String message_text = String::Format(
            "Added non-passive event listener to a scroll-blocking '%s' event. "
            "Consider marking event handler as 'passive' to make the page more "
            "responsive. See "
            "https://www.chromestatus.com/feature/5745543795965952",
            event_type.GetString().Utf8().data());

        PerformanceMonitor::ReportGenericViolation(
            GetExecutionContext(), PerformanceMonitor::kDiscouragedAPIUse,
            message_text, base::TimeDelta(), nullptr);
    }
#endif
#endif
}

}  // namespace blink
