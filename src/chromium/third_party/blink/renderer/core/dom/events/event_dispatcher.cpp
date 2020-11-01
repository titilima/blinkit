// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_dispatcher.cpp
// Description: EventDispatcher Class
//      Author: Ziming Li
//     Created: 2019-11-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) 2011 Google Inc. All rights reserved.
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

#include "event_dispatcher.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/events/event.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatch_forbidden_scope.h"
#include "third_party/blink/renderer/core/dom/events/event_path.h"
#include "third_party/blink/renderer/core/dom/events/node_event_context.h"
#include "third_party/blink/renderer/core/dom/events/window_event_context.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"

namespace blink {

EventDispatcher::EventDispatcher(Node &node, Event &event) : m_node(node), m_event(event)
{
#ifndef BLINKIT_CRAWLER_ONLY
    m_view = node.GetDocument().View();
#endif
    m_event->InitEventPath(*m_node);
}

DispatchEventResult EventDispatcher::Dispatch(void)
{
#if DCHECK_IS_ON()
    ASSERT(!m_eventDispatched);
    m_eventDispatched = true;
#endif

    EventPath &eventPath = m_event->GetEventPath();
    if (eventPath.IsEmpty())
    {
        // GetEventPath() can be empty if event path is shrinked by relataedTarget
        // retargeting.
        return DispatchEventResult::kNotCanceled;
    }
    eventPath.EnsureWindowEventContext();

#ifdef BLINKIT_CRAWLER_ONLY
    Node *activationTarget = nullptr;
#else
    ASSERT(false); // BKTODO:
#if 0
    // 6. Let isActivationEvent be true, if event is a MouseEvent object and
    // event's type attribute is "click", and false otherwise.
    //
    // We need to include non-standard textInput event for HTMLInputElement.
    const bool is_activation_event =
        (event_->IsMouseEvent() && event_->type() == EventTypeNames::click) ||
        event_->type() == EventTypeNames::textInput;

    // 7. Let activationTarget be target, if isActivationEvent is true and target
    // has activation behavior, and null otherwise.
    Node* activation_target =
        is_activation_event && node_->HasActivationBehavior() ? node_ : nullptr;

    // A part of step 9 loop.
    if (is_activation_event && !activation_target && event_->bubbles()) {
        wtf_size_t size = event_->GetEventPath().size();
        for (wtf_size_t i = 1; i < size; ++i) {
            Node* target = event_->GetEventPath()[i].GetNode();
            if (target->HasActivationBehavior()) {
                activation_target = target;
                break;
            }
        }
    }
#else
    Node *activationTarget = nullptr;
#endif
#endif

    m_event->SetTarget(EventPath::EventTargetRespectingTargetRules(*m_node));
#if DCHECK_IS_ON()
    ASSERT(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif
    ASSERT(nullptr != m_event->target());
    EventDispatchHandlingState *preDispatchEventHandlerResult = nullptr;
    if (DispatchEventPreProcess(activationTarget, preDispatchEventHandlerResult) == kContinueDispatching)
    {
        if (DispatchEventAtCapturing() == kContinueDispatching)
        {
            // TODO(crbug/882574): Remove these.
            ASSERT(m_event->HasEventPath());
            ASSERT(!eventPath.IsEmpty());
            if (DispatchEventAtTarget() == kContinueDispatching)
                DispatchEventAtBubbling();
        }
    }
    DispatchEventPostProcess(activationTarget, preDispatchEventHandlerResult);

    return EventTarget::GetDispatchEventResult(*m_event);
}

DispatchEventResult EventDispatcher::DispatchEvent(Node &node, Event &event)
{
#if DCHECK_IS_ON()
    ASSERT(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif
    EventDispatcher dispatcher(node, event);
    return event.DispatchEvent(dispatcher);
}

void EventDispatcher::DispatchEventAtBubbling(void)
{
    EventPath &eventPath = m_event->GetEventPath();

    // Trigger bubbling event handlers, starting at the bottom and working our way
    // up.
    wtf_size_t size = eventPath.size();
    for (wtf_size_t i = 1; i < size; ++i)
    {
        const NodeEventContext &eventContext = eventPath.at(i);
        if (eventContext.CurrentTargetSameAsTarget())
        {
            // TODO(hayato): Need to check cancelBubble() also here?
            m_event->SetEventPhase(Event::kAtTarget);
            ASSERT(false); // BKTODO:
#if 0
            if (RuntimeEnabledFeatures::
                CallCaptureListenersAtCapturePhaseAtShadowHostsEnabled()) {
                event_->SetFireOnlyNonCaptureListenersAtTarget(true);
                event_context.HandleLocalEvents(*event_);
                event_->SetFireOnlyNonCaptureListenersAtTarget(false);
            }
            else {
                event_context.HandleLocalEvents(*event_);
            }
#endif
        }
        else if (m_event->bubbles() && !m_event->cancelBubble())
        {
            m_event->SetEventPhase(Event::kBubblingPhase);
            eventContext.HandleLocalEvents(*m_event);
        }
        else
        {
            continue;
        }
        if (m_event->PropagationStopped())
            return;
    }
    if (m_event->bubbles() && !m_event->PropagationStopped())
    {
        m_event->SetEventPhase(Event::kBubblingPhase);
        eventPath.GetWindowEventContext().HandleLocalEvents(*m_event);
    }
}

EventDispatchContinuation EventDispatcher::DispatchEventAtCapturing(void)
{
    // Trigger capturing event handlers, starting at the top and working our way
    // down.
    m_event->SetEventPhase(Event::kCapturingPhase);

    EventPath &eventPath = m_event->GetEventPath();
    if (eventPath.GetWindowEventContext().HandleLocalEvents(*m_event) && m_event->PropagationStopped())
        return kDoneDispatching;

    for (wtf_size_t i = eventPath.size() - 1; i > 0; --i)
    {
        const NodeEventContext &eventContext = eventPath.at(i);
        if (eventContext.CurrentTargetSameAsTarget())
        {
            ASSERT(false); // BKTODO:
#if 0
            if (!RuntimeEnabledFeatures::
                CallCaptureListenersAtCapturePhaseAtShadowHostsEnabled())
                continue;
            event_->SetEventPhase(Event::kAtTarget);
            event_->SetFireOnlyCaptureListenersAtTarget(true);
            event_context.HandleLocalEvents(*event_);
            event_->SetFireOnlyCaptureListenersAtTarget(false);
#endif
        }
        else
        {
            m_event->SetEventPhase(Event::kCapturingPhase);
            eventContext.HandleLocalEvents(*m_event);
        }
        if (m_event->PropagationStopped())
            return kDoneDispatching;
    }

    return kContinueDispatching;
 }

EventDispatchContinuation EventDispatcher::DispatchEventAtTarget(void)
{
    m_event->SetEventPhase(Event::kAtTarget);
    m_event->GetEventPath().at(0).HandleLocalEvents(*m_event);
    return m_event->PropagationStopped() ? kDoneDispatching : kContinueDispatching;
}

EventDispatchContinuation EventDispatcher::DispatchEventPreProcess(Node *activationTarget, EventDispatchHandlingState *&preDispatchEventHandlerResult)
{
#ifndef BLINKIT_CRAWLER_ONLY
    // 11. If activationTarget is non-null and activationTarget has
    // legacy-pre-activation behavior, then run activationTarget's
    // legacy-pre-activation behavior.
    if (nullptr != activationTarget)
    {
        ASSERT(false); // BKTODO: preDispatchEventHandlerResult = activationTarget->PreDispatchEventHandler(*event_);
    }
#endif
    return (m_event->GetEventPath().IsEmpty() || m_event->PropagationStopped())
        ? kDoneDispatching
        : kContinueDispatching;
}

void EventDispatcher::DispatchEventPostProcess(Node *activationTarget, EventDispatchHandlingState *preDispatchEventHandlerResult)
{
    m_event->SetTarget(EventPath::EventTargetRespectingTargetRules(*m_node));
    // https://dom.spec.whatwg.org/#concept-event-dispatch
    // 14. Unset event¡¯s dispatch flag, stop propagation flag, and stop immediate
    // propagation flag.
    m_event->SetStopPropagation(false);
    m_event->SetStopImmediatePropagation(false);
    // 15. Set event¡¯s eventPhase attribute to NONE.
    m_event->SetEventPhase(0);
    // TODO(rakina): investigate this and move it to the bottom of step 16
    // 17. Set event¡¯s currentTarget attribute to null.
    m_event->SetCurrentTarget(nullptr);

#ifdef BLINKIT_CRAWLER_ONLY
    bool isClick = false;
#else
    ASSERT(false); // BKTODO:
#if 0
    bool is_click = event_->IsMouseEvent() &&
        ToMouseEvent(*event_).type() == EventTypeNames::click;
    if (is_click)
    {
        // Fire an accessibility event indicating a node was clicked on.  This is
        // safe if event_->target()->ToNode() returns null.
        if (AXObjectCache* cache = node_->GetDocument().ExistingAXObjectCache())
            cache->HandleClicked(event_->target()->ToNode());

        // Pass the data from the PreDispatchEventHandler to the
        // PostDispatchEventHandler.
        // This may dispatch an event, and node_ and event_ might be altered.
        if (activation_target) {
            activation_target->PostDispatchEventHandler(
                *event_, pre_dispatch_event_handler_result);
        }
        // TODO(tkent): Is it safe to kick DefaultEventHandler() with such altered
        // event_?
    }
#else
    bool isClick = false;
#endif
#endif

    // The DOM Events spec says that events dispatched by JS (other than "click")
    // should not have their default handlers invoked.
    bool isTrustedOrClick = !RuntimeEnabledFeatures::TrustedEventsDefaultActionEnabled()
        || m_event->isTrusted() || isClick;

#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(false); // BKTODO:
#if 0
    // For Android WebView (distinguished by wideViewportQuirkEnabled)
    // enable untrusted events for mouse down on select elements because
    // fastclick.js seems to generate these. crbug.com/642698
    // TODO(dtapuska): Change this to a target SDK quirk crbug.com/643705
    if (!isTrustedOrClick && event_->IsMouseEvent() &&
        event_->type() == EventTypeNames::mousedown &&
        IsHTMLSelectElement(*node_)) {
        if (Settings* settings = node_->GetDocument().GetSettings()) {
            is_trusted_or_click = settings->GetWideViewportQuirkEnabled();
        }
    }
#endif
#endif

    EventPath &eventPath = m_event->GetEventPath();

    // Call default event handlers. While the DOM does have a concept of
    // preventing default handling, the detail of which handlers are called is an
    // internal implementation detail and not part of the DOM.
    if (!m_event->defaultPrevented() && !m_event->DefaultHandled() && isTrustedOrClick)
    {
        // Non-bubbling events call only one default event handler, the one for the
        // target.
        m_node->WillCallDefaultEventHandler(*m_event);
        m_node->DefaultEventHandler(*m_event);
        ASSERT(!m_event->defaultPrevented());
        // For bubbling events, call default event handlers on the same targets in
        // the same order as the bubbling phase.
        if (!m_event->DefaultHandled() && m_event->bubbles())
        {
            wtf_size_t size = eventPath.size();
            for (wtf_size_t i = 1; i < size; ++i)
            {
                Node *node = eventPath.at(i).GetNode();
                node->WillCallDefaultEventHandler(*m_event);
                node->DefaultEventHandler(*m_event);
                ASSERT(!m_event->defaultPrevented());
                if (m_event->DefaultHandled())
                    break;
            }
        }
    }

    // 16. If target's root is a shadow root, then set event's target attribute
    // and event's relatedTarget to null.
    m_event->SetTarget(eventPath.GetWindowEventContext().Target());
    if (nullptr == m_event->target())
        m_event->SetRelatedTargetIfExists(nullptr);
}

} // namespace blink
