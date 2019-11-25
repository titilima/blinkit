// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_dispatcher.h
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
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights
 * reserved.
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

#ifndef BLINKIT_BLINK_EVENT_DISPATCHER_H
#define BLINKIT_BLINK_EVENT_DISPATCHER_H

#pragma once

#include "third_party/blink/renderer/core/dom/events/event_dispatch_result.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class Event;
class Node;

class EventDispatchHandlingState : public GarbageCollected<EventDispatchHandlingState>
{
};

enum EventDispatchContinuation { kContinueDispatching, kDoneDispatching };

class EventDispatcher
{
    STACK_ALLOCATED();
public:
    static DispatchEventResult DispatchEvent(Node &node, Event &event);

    DispatchEventResult Dispatch(void);

    Event& GetEvent(void) const { return *m_event; }
private:
    EventDispatcher(Node &node, Event &event);

    EventDispatchContinuation DispatchEventPreProcess(Node *activationTarget, EventDispatchHandlingState *&preDispatchEventHandlerResult);
    EventDispatchContinuation DispatchEventAtCapturing(void);
    EventDispatchContinuation DispatchEventAtTarget(void);
    void DispatchEventAtBubbling(void);
    void DispatchEventPostProcess(Node *activationTarget, EventDispatchHandlingState *preDispatchEventHandlerResult);

    Member<Node> m_node;
    Member<Event> m_event;
#ifndef BLINKIT_CRAWLER_ONLY
    Member<LocalFrameView> m_view;
#endif
#if DCHECK_IS_ON()
    bool m_eventDispatched = false;
#endif
};

} // namespace blink

#endif // BLINKIT_BLINK_EVENT_DISPATCHER_H
