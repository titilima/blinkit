// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_listener.h
// Description: EventListener Class
//      Author: Ziming Li
//     Created: 2019-12-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef BLINKIT_BLINK_EVENT_LISTENER_H
#define BLINKIT_BLINK_EVENT_LISTENER_H

#pragma once

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

class Event;
class ExecutionContext;

class EventListener : public std::enable_shared_from_this<EventListener>
{
public:
    enum ListenerType {
        // |kJSEventListenerType| corresponds to EventListener defined in standard:
        // https://dom.spec.whatwg.org/#callbackdef-eventlistener
        kJSEventListenerType,
        // |kJSEventHandlerType| corresponds to EventHandler defined in standard:
        // https://html.spec.whatwg.org/multipage/webappapis.html#event-handler-attributes
        kJSEventHandlerType,
        // These are for C++ native callbacks.
        kImageEventListenerType,
        kCPPEventListenerType,
        kConditionEventListenerType,
    };

    virtual ~EventListener(void) = default;
    virtual bool operator==(const EventListener &other) const = 0;
    virtual void handleEvent(ExecutionContext*, Event*) = 0;
#if 0 // BKTODO:
    virtual const String& Code() const { return g_empty_string; }
    virtual bool IsEventHandlerForContentAttribute() const { return false; }
    virtual bool BelongsToTheCurrentWorld(ExecutionContext*) const {
        return false;
    }
    virtual bool IsEventHandler() const { return false; }

    ListenerType GetType() const { return type_; }

    // Returns true if this EventListener is implemented based on JS object.
    bool IsJSBased() const {
        return type_ == kJSEventListenerType || type_ == kJSEventHandlerType;
    }

    // Returns true if this EventListener is C++ native callback.
    bool IsNativeBased() const { return !IsJSBased(); }

    const char* NameInHeapSnapshot() const override { return "EventListener"; }

protected:
    explicit EventListener(ListenerType type) : type_(type) {}

private:
    ListenerType type_;
#endif
};

} // BLINKIT_BLINK_EVENT_LISTENER_H blink

#endif // BLINKIT_BLINK_EVENT_LISTENER_H
