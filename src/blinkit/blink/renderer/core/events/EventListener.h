// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: EventListener.h
// Description: EventListener Class
//      Author: Ziming Li
//     Created: 2021-11-08
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
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

#ifndef EventListener_h
#define EventListener_h

#include "platform/heap/Handle.h"

namespace blink {

class Event;
class ExecutionContext;

class EventListener : public BlinKit::GCObject {
public:
    enum Type {
        JSEventListenerType,
        ImageEventListenerType,
        CPPEventListenerType,
        ConditionEventListenerType,
        NativeEventListenerType,
    };

    virtual ~EventListener() { }
    virtual bool operator==(const EventListener&) const = 0;
    virtual void handleEvent(ExecutionContext*, Event*) = 0;
    virtual bool wasCreatedFromMarkup() const { return false; }
    virtual bool belongsToTheCurrentWorld() const { return false; }

    bool isAttribute() const { return virtualisAttribute(); }
    Type type() const { return m_type; }

    DEFINE_INLINE_VIRTUAL_TRACE() { }

protected:
    explicit EventListener(Type type)
            : m_type(type)
    {
    }

private:
    virtual bool virtualisAttribute() const { return false; }

    Type m_type;
};

}

#endif
