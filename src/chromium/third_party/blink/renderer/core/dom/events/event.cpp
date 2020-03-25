// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event.cpp
// Description: Event Class
//      Author: Ziming Li
//     Created: 2019-10-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2001 Tobias Anton (anton@stud.fbi.fh-darmstadt.de)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2003, 2005, 2006, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "event.h"

#include "third_party/blink/renderer/core/dom/events/event_dispatcher.h"
#include "third_party/blink/renderer/core/dom/events/event_path.h"
#include "third_party/blink/renderer/core/dom/events/window_event_context.h"

namespace blink {

Event::Event(const AtomicString &eventType, Bubbles bubbles, Cancelable cancelable, ComposedMode composedMode, TimeTicks platformTimeStamp)
    : m_type(eventType)
    , m_bubbles(Bubbles::kYes == bubbles)
    , m_cancelable(Cancelable::kYes == cancelable)
    , m_composed(ComposedMode::kComposed == composedMode)
    , m_propagationStopped(false)
    , m_immediatePropagationStopped(false)
    , m_defaultPrevented(false)
    , m_defaultHandled(false)
    , m_wasInitialized(true)
    , m_isTrusted(false)
    , m_executedListenerOrDefaultAction(false)
    , m_preventDefaultCalledOnUncancelableEvent(false)
    , m_fireOnlyCaptureListenersAtTarget(false)
    , m_fireOnlyNonCaptureListenersAtTarget(false)
    , m_platformTimeStamp(platformTimeStamp)
{
}

Event::Event(const AtomicString &eventType, Bubbles bubbles, Cancelable cancelable, ComposedMode composedMode)
    : Event(eventType, bubbles, cancelable, composedMode, CurrentTimeTicks())
{
}

Event::~Event(void) = default;

DispatchEventResult Event::DispatchEvent(EventDispatcher &dispatcher)
{
    return dispatcher.Dispatch();
}

void Event::DoneDispatchingEventAtCurrentTarget(void)
{
    SetExecutedListenerOrDefaultAction();
}

void Event::InitEventPath(Node &node)
{
    if (!m_eventPath)
        m_eventPath = std::make_unique<EventPath>(node, this);
    else
        m_eventPath->InitializeWith(node, this);
}

void Event::preventDefault(void)
{
    if (PassiveMode::kNotPassive != m_handlingPassive && PassiveMode::kNotPassiveDefault != m_handlingPassive)
    {
        m_preventDefaultCalledDuringPassive = true;

        const LocalDOMWindow *window = m_eventPath ? m_eventPath->GetWindowEventContext().Window() : nullptr;
        if (nullptr != window && PassiveMode::kPassive == m_handlingPassive)
        {
            ASSERT(false); // BKTODO:
#if 0
            window->PrintErrorMessage(
                "Unable to preventDefault inside passive event listener invocation.");
#endif
        }
        return;
    }

    if (m_cancelable)
        m_defaultPrevented = true;
    else
        m_preventDefaultCalledOnUncancelableEvent = true;
}

void Event::SetHandlingPassive(PassiveMode mode)
{
    m_handlingPassive = mode;
    m_preventDefaultCalledDuringPassive = false;
}

void Event::SetRelatedTargetIfExists(EventTarget *relatedTarget)
{
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
#endif
}

void Event::SetTarget(EventTarget *target)
{
    if (m_target == target)
        return;

    m_target = target;
    if (m_target)
        ReceivedTarget();
}

} // namespace blink
