// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_listener_map.cpp
// Description: EventListenerMap Class
//      Author: Ziming Li
//     Created: 2019-12-21
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
 * Copyright (C) 2011 Andreas Kling (kling@webkit.org)
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

#include "event_listener_map.h"

#include <algorithm>

namespace blink {

static bool AddListenerToVector(
    EventListenerVector *vector,
    EventListener *listener,
    const AddEventListenerOptionsResolved &options,
    RegisteredEventListener *registeredListener)
{
    *registeredListener = RegisteredEventListener(listener, options);

    if (std::find(vector->begin(), vector->end(), *registeredListener) != std::end(*vector))
        return false;  // Duplicate listener.

    vector->push_back(*registeredListener);
    return true;
}

bool EventListenerMap::Add(
    const AtomicString &eventType,
    EventListener *listener,
    const AddEventListenerOptionsResolved &options,
    RegisteredEventListener* registeredListener)
{
    // BKTODO: CheckNoActiveIterators();

    for (const auto &entry : m_entries)
    {
        if (entry.first == eventType)
            return AddListenerToVector(entry.second.get(), listener, options, registeredListener);
    }

    m_entries.push_back(std::make_pair(eventType, std::make_unique<EventListenerVector>()));
    return AddListenerToVector(m_entries.back().second.get(), listener, options, registeredListener);
}

bool EventListenerMap::ContainsCapturing(const AtomicString &eventType) const
{
    for (const auto &entry : m_entries)
    {
        if (entry.first == eventType)
        {
            for (const auto &eventListener : *entry.second)
            {
                if (eventListener.Capture())
                    return true;
            }
            return false;
        }
    }
    return false;
}

EventListenerVector* EventListenerMap::Find(const AtomicString &eventType)
{
    for (const auto &entry : m_entries)
    {
        if (entry.first == eventType)
            return entry.second.get();
    }

    return nullptr;
}

static bool RemoveListenerFromVector(
    EventListenerVector *listenerVector,
    const EventListener *listener,
    const EventListenerOptions &options,
    wtf_size_t *indexOfRemovedListener,
    RegisteredEventListener* registeredListener)
{
    // Do a manual search for the matching RegisteredEventListener. It is not
    // possible to create a RegisteredEventListener on the stack because of the
    // const on |listener|.
    const auto callback = [listener, &options](const RegisteredEventListener &eventListener)
    {
        return eventListener.Matches(listener, options);
    };
    const auto it = std::find_if(listenerVector->begin(), listenerVector->end(), callback);

    if (std::end(*listenerVector) == it)
    {
        *indexOfRemovedListener = kNotFound;
        return false;
    }

    *registeredListener = *it;
    *indexOfRemovedListener = static_cast<wtf_size_t>(it - listenerVector->begin());
    listenerVector->erase(it);
    return true;
}

bool EventListenerMap::Remove(
    const AtomicString &eventType,
    const EventListener *listener,
    const EventListenerOptions &options,
    wtf_size_t *indexOfRemovedListener,
    RegisteredEventListener *registeredListener)
{
    // BKTODO: CheckNoActiveIterators();

    for (unsigned i = 0; i < m_entries.size(); ++i)
    {
        if (m_entries[i].first == eventType)
        {
            bool wasRemoved = RemoveListenerFromVector(m_entries[i].second.get(), listener, options,
                indexOfRemovedListener, registeredListener);
            if (m_entries[i].second->empty())
                m_entries.erase(m_entries.begin() + i);
            return wasRemoved;
        }
    }

    return false;
}

}  // namespace blink
