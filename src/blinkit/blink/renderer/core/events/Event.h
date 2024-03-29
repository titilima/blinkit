#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Event.h
// Description: Event Class
//      Author: Ziming Li
//     Created: 2021-07-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2001 Tobias Anton (anton@stud.fbi.fh-darmstadt.de)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
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
 *
 */

#ifndef Event_h
#define Event_h

#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"
#include "blinkit/blink/renderer/core/dom/DOMHighResTimeStamp.h"
#include "blinkit/blink/renderer/core/dom/DOMTimeStamp.h"
#include "blinkit/blink/renderer/core/events/EventInit.h"
#include "blinkit/blink/renderer/core/events/EventPath.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"
#include "blinkit/blink/renderer/wtf/text/AtomicString.h"

namespace blink {

class DOMWrapperWorld;
class EventTarget;
class ExecutionContext;

class Event : public BlinKit::GCObject, public ScriptWrappable
{
    DEFINE_WRAPPERTYPEINFO();
public:
    enum PhaseType {
        NONE                = 0,
        CAPTURING_PHASE     = 1,
        AT_TARGET           = 2,
        BUBBLING_PHASE      = 3
    };

    enum EventType {
        MOUSEDOWN           = 1,
        MOUSEUP             = 2,
        MOUSEOVER           = 4,
        MOUSEOUT            = 8,
        MOUSEMOVE           = 16,
        MOUSEDRAG           = 32,
        CLICK               = 64,
        DBLCLICK            = 128,
        KEYDOWN             = 256,
        KEYUP               = 512,
        KEYPRESS            = 1024,
        DRAGDROP            = 2048,
        FOCUS               = 4096,
        BLUR                = 8192,
        SELECT              = 16384,
        CHANGE              = 32768
    };

    enum RailsMode {
        RailsModeFree       = 0,
        RailsModeHorizontal = 1,
        RailsModeVertical   = 2
    };

    static GCRefPtr<Event> create()
    {
        return BlinKit::GCWrapShared(new Event);
    }

    // A factory for a simple event. The event doesn't bubble, and isn't
    // cancelable.
    // http://www.whatwg.org/specs/web-apps/current-work/multipage/webappapis.html#fire-a-simple-event
    static GCRefPtr<Event> create(const AtomicString& type)
    {
        return BlinKit::GCWrapShared(new Event(type, false, false));
    }
    static GCRefPtr<Event> createCancelable(const AtomicString& type)
    {
        return BlinKit::GCWrapShared(new Event(type, false, true));
    }
    static GCRefPtr<Event> createBubble(const AtomicString& type)
    {
        return BlinKit::GCWrapShared(new Event(type, true, false));
    }
    static GCRefPtr<Event> createCancelableBubble(const AtomicString& type)
    {
        return BlinKit::GCWrapShared(new Event(type, true, true));
    }

    static GCRefPtr<Event> create(const AtomicString& type, const EventInit& initializer)
    {
        return BlinKit::GCWrapShared(new Event(type, initializer));
    }

    virtual ~Event();

    void initEvent(const AtomicString& type, bool canBubble, bool cancelable);

    const AtomicString& type() const { return m_type; }
    void setType(const AtomicString& type) { m_type = type; }

    EventTarget* target() const { return m_target.get(); }
    void setTarget(PassRefPtrWillBeRawPtr<EventTarget>);

    EventTarget* currentTarget() const;
    void setCurrentTarget(EventTarget* currentTarget) { m_currentTarget = currentTarget; }

    unsigned short eventPhase() const { return m_eventPhase; }
    void setEventPhase(unsigned short eventPhase) { m_eventPhase = eventPhase; }

    bool bubbles() const { return m_canBubble; }
    bool cancelable() const { return m_cancelable; }

#if 0 // BKTODO:
    // Event creation timestamp in milliseconds. If |HiResEventTimeStamp|
    // runtime feature is enabled it returns a DOMHighResTimeStamp using the
    // platform timestamp (see |m_platformTimeStamp|) otherwise it returns a
    // DOMTimeStamp that represents the current object's construction time (see
    // |m_createTime|). For more info see http://crbug.com/160524
    double timeStamp(ScriptState*) const;
#endif
    double platformTimeStamp() const { return m_platformTimeStamp; }
    DOMTimeStamp createTime() const { return m_createTime; }

    void stopPropagation() { m_propagationStopped = true; }
    void stopImmediatePropagation() { m_immediatePropagationStopped = true; }

    // IE Extensions
    EventTarget* srcElement() const { return target(); } // MSIE extension - "the object that fired the event"

    bool legacyReturnValue(ExecutionContext*) const;
    void setLegacyReturnValue(ExecutionContext*, bool returnValue);

    virtual const AtomicString& interfaceName() const;
    bool hasInterface(const AtomicString&) const;

    // These events are general classes of events.
    virtual bool isUIEvent() const;
    virtual bool isMouseEvent() const;
    virtual bool isFocusEvent() const;
    virtual bool isKeyboardEvent() const;
    virtual bool isTouchEvent() const;
    virtual bool isGestureEvent() const;
    virtual bool isWheelEvent() const;
    virtual bool isRelatedEvent() const;
    virtual bool isPointerEvent() const;

    // Drag events are a subset of mouse events.
    virtual bool isDragEvent() const;

    // These events lack a DOM interface.
    virtual bool isClipboardEvent() const;
    virtual bool isBeforeTextInsertedEvent() const;

    virtual bool isBeforeUnloadEvent() const;

    bool propagationStopped() const { return m_propagationStopped || m_immediatePropagationStopped; }
    bool immediatePropagationStopped() const { return m_immediatePropagationStopped; }

    bool defaultPrevented() const { return m_defaultPrevented; }
    virtual void preventDefault();
    void setDefaultPrevented(bool defaultPrevented) { m_defaultPrevented = defaultPrevented; }

    bool defaultHandled() const { return m_defaultHandled; }
    void setDefaultHandled() { m_defaultHandled = true; }

    bool cancelBubble() const { return m_cancelBubble; }
    void setCancelBubble(bool cancel) { m_cancelBubble = cancel; }

    Event* underlyingEvent() const { return m_underlyingEvent.get(); }
    void setUnderlyingEvent(PassRefPtrWillBeRawPtr<Event>);

    EventPath& eventPath() { ASSERT(m_eventPath); return *m_eventPath; }
    void initEventPath(Node&);

#if 0 // BKTODO:
    WillBeHeapVector<RefPtrWillBeMember<EventTarget>> path(ScriptState*) const;
#endif

    bool isBeingDispatched() const { return eventPhase(); }

    // Events that must not leak across isolated world, similar to how
    // ErrorEvent behaves, can override this method.
    virtual bool canBeDispatchedInWorld(const DOMWrapperWorld&) const { return true; }

    virtual PassRefPtrWillBeRawPtr<EventDispatchMediator> createMediator();

    bool isTrusted() const { return m_isTrusted; }
    void setTrusted(bool value) { m_isTrusted = value; }

    void setHandlingPassive(bool value) { m_handlingPassive = value; }

protected:
    Event();
    Event(const AtomicString& type, bool canBubble, bool cancelable);
    Event(const AtomicString& type, bool canBubble, bool cancelable, double platformTimeStamp);
    Event(const AtomicString& type, const EventInit&);

    virtual void receivedTarget();
    bool dispatched() const { return m_target; }

    void setCanBubble(bool bubble) { m_canBubble = bubble; }

private:
    AtomicString m_type;
    unsigned m_canBubble:1;
    unsigned m_cancelable:1;

    unsigned m_propagationStopped:1;
    unsigned m_immediatePropagationStopped:1;
    unsigned m_defaultPrevented:1;
    unsigned m_defaultHandled:1;
    unsigned m_cancelBubble:1;
    unsigned m_isTrusted : 1;
    unsigned m_handlingPassive : 1;

    unsigned short m_eventPhase;
    RefPtrWillBeMember<EventTarget> m_currentTarget;
    RefPtrWillBeMember<EventTarget> m_target;
    DOMTimeStamp m_createTime;
    GCRefPtr<Event> m_underlyingEvent;
    std::unique_ptr<EventPath> m_eventPath;
    // The monotonic platform time in seconds, for input events it is the
    // event timestamp provided by the host OS and reported in the original
    // WebInputEvent instance.
    double m_platformTimeStamp;
};

#define DEFINE_EVENT_TYPE_CASTS(typeName) \
    DEFINE_TYPE_CASTS(typeName, Event, event, event->is##typeName(), event.is##typeName())

} // namespace blink

#endif // Event_h
