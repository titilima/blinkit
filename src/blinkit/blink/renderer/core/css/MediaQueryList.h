#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MediaQueryList.h
// Description: MediaQueryList Class
//      Author: Ziming Li
//     Created: 2021-07-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#ifndef MediaQueryList_h
#define MediaQueryList_h

#include "blinkit/blink/renderer/core/dom/ActiveDOMObject.h"
#include "blinkit/blink/renderer/core/events/event_target.h"
#include "blinkit/blink/renderer/wtf/Forward.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"

namespace blink {

class ExecutionContext;
class MediaQueryListListener;
class MediaQueryMatcher;
class MediaQuerySet;

// MediaQueryList interface is specified at http://dev.w3.org/csswg/cssom-view/#the-mediaquerylist-interface
// The objects of this class are returned by window.matchMedia. They may be used to
// retrieve the current value of the given media query and to add/remove listeners that
// will be called whenever the value of the query changes.

class CORE_EXPORT MediaQueryList final : public EventTargetWithInlineData, public ActiveDOMObject {
    REFCOUNTED_EVENT_TARGET(MediaQueryList);
    DEFINE_WRAPPERTYPEINFO();
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(MediaQueryList);
public:
    static PassRefPtrWillBeRawPtr<MediaQueryList> create(ExecutionContext*, PassRefPtrWillBeRawPtr<MediaQueryMatcher>, PassRefPtrWillBeRawPtr<MediaQuerySet>);
    ~MediaQueryList() override;

    String media() const;
    bool matches();

    // BKTODO: DEFINE_ATTRIBUTE_EVENT_LISTENER(change);

    // These two functions are provided for compatibility with JS code
    // written before the change listener became a DOM event.
    void addDeprecatedListener(PassRefPtrWillBeRawPtr<EventListener>);
    void removeDeprecatedListener(PassRefPtrWillBeRawPtr<EventListener>);

    // C++ code can use these functions to listen to changes instead of having to use DOM event listeners.
    void addListener(PassRefPtrWillBeRawPtr<MediaQueryListListener>);
    void removeListener(PassRefPtrWillBeRawPtr<MediaQueryListListener>);

    // Will return true if a DOM event should be scheduled.
    bool mediaFeaturesChanged(WillBeHeapVector<RefPtrWillBeMember<MediaQueryListListener>>* listenersToNotify);

    DECLARE_VIRTUAL_TRACE();

    // From ActiveDOMObject
    bool hasPendingActivity() const override;
    void stop() override;

    const AtomicString& interfaceName() const override;
    ExecutionContext* executionContext() const override;

private:
    MediaQueryList(ExecutionContext*, PassRefPtrWillBeRawPtr<MediaQueryMatcher>, PassRefPtrWillBeRawPtr<MediaQuerySet>);

    bool updateMatches();

    RefPtrWillBeMember<MediaQueryMatcher> m_matcher;
    RefPtrWillBeMember<MediaQuerySet> m_media;
    using ListenerList = WillBeHeapListHashSet<RefPtrWillBeMember<MediaQueryListListener>>;
    ListenerList m_listeners;
    bool m_matchesDirty;
    bool m_matches;
};

} // namespace blink

#endif // MediaQueryList_h
