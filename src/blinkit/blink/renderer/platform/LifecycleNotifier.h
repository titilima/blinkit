// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LifecycleNotifier.h
// Description: LifecycleNotifier Class
//      Author: Ziming Li
//     Created: 2021-08-31
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 * Copyright (C) 2013 Google Inc. All Rights Reserved.
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
#ifndef LifecycleNotifier_h
#define LifecycleNotifier_h

#include "blinkit/gc/gc_weak_object_set.h"
#include "platform/heap/Handle.h"
#include "wtf/HashSet.h"
#include "wtf/TemporaryChange.h"

namespace blink {

template<typename T, typename Observer>
class LifecycleNotifier { // BKTODO: : public virtual WillBeGarbageCollectedMixin {
public:
    virtual ~LifecycleNotifier();

    void addObserver(Observer*);
    void removeObserver(Observer*);

    // notifyContextDestroyed() should be explicitly dispatched from an
    // observed context to notify observers that contextDestroyed().
    //
    // When contextDestroyed() is called, the observer's lifecycleContext()
    // is still valid and safe to use during the notification.
    virtual void notifyContextDestroyed();

#if 0 // BKTODO:
    DEFINE_INLINE_VIRTUAL_TRACE()
    {
#if ENABLE(OILPAN)
        visitor->trace(m_observers);
#endif
    }
#endif

    bool isIteratingOverObservers() const { return m_iterating != IteratingNone; }

protected:
    LifecycleNotifier()
        : m_iterating(IteratingNone)
        , m_didCallContextDestroyed(false)
    {
    }

    enum IterationType {
        IteratingNone,
        IteratingOverAll,
    };

    IterationType m_iterating;

protected:
    using ObserverSet = BlinKit::GCWeakObjectSet<Observer>;

    ObserverSet m_observers;

#if ENABLE(ASSERT)
    T* context() { return static_cast<T*>(this); }
#endif

private:
    bool m_didCallContextDestroyed;
};

template<typename T, typename Observer>
inline LifecycleNotifier<T, Observer>::~LifecycleNotifier()
{
    // FIXME: Enable the following ASSERT. Also see a FIXME in Document::detach().
    // ASSERT(!m_observers.size() || m_didCallContextDestroyed);

#if !ENABLE(OILPAN)
    TemporaryChange<IterationType> scope(m_iterating, IteratingOverAll);
    for (Observer* observer : m_observers) {
        observer->clearLifecycleContext();
    }
#endif
}

template<typename T, typename Observer>
inline void LifecycleNotifier<T, Observer>::notifyContextDestroyed()
{
    // Don't notify contextDestroyed() twice.
    if (m_didCallContextDestroyed)
        return;

    TemporaryChange<IterationType> scope(m_iterating, IteratingOverAll);
    std::vector<Observer *> snapshotOfObservers = m_observers.GetSnapshot();
    for (Observer* observer : snapshotOfObservers) {
        // FIXME: Oilpan: At the moment, it's possible that the Observer is
        // destructed during the iteration.
        // Once we enable Oilpan by default for Observers *and*
        // Observer::contextDestroyed() does not call removeObserver(),
        // we can remove the hack by making m_observers
        // a HeapHashSet<WeakMember<Observers>>. (i.e., we can just iterate
        // m_observers without taking a snapshot).
        if (m_observers.contains(observer)) {
            ASSERT(observer->lifecycleContext() == context());
            observer->contextDestroyed();
        }
    }

    m_didCallContextDestroyed = true;
}

template<typename T, typename Observer>
inline void LifecycleNotifier<T, Observer>::addObserver(Observer* observer)
{
    RELEASE_ASSERT(m_iterating != IteratingOverAll);
    m_observers.emplace(observer);
}

template<typename T, typename Observer>
inline void LifecycleNotifier<T, Observer>::removeObserver(Observer* observer)
{
    m_observers.erase(observer);
}

} // namespace blink

#endif // LifecycleNotifier_h
