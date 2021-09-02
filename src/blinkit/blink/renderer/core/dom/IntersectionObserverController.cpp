// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: IntersectionObserverController.cpp
// Description: IntersectionObserverController Class
//      Author: Ziming Li
//     Created: 2021-07-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/dom/IntersectionObserverController.h"

#include "core/dom/Document.h"

namespace blink {

typedef std::vector<IntersectionObserver *> IntersectionObserverVector;

GCPassPtr<IntersectionObserverController> IntersectionObserverController::create(Document* document)
{
    IntersectionObserverController* result = new IntersectionObserverController(document);
    result->suspendIfNeeded();
    return WrapLeaked(result);
}

IntersectionObserverController::IntersectionObserverController(Document* document)
    : ActiveDOMObject(document)
    , m_timer(this, &IntersectionObserverController::deliverIntersectionObservations)
    , m_timerFiredWhileSuspended(false)
{
}

IntersectionObserverController::~IntersectionObserverController() { }

void IntersectionObserverController::scheduleIntersectionObserverForDelivery(IntersectionObserver& observer)
{
    // TODO(szager): use idle callback with a timeout.  Until we do that, there is no
    // reliable way to write a test for takeRecords, because it's impossible to guarantee
    // that javascript will get a chance to run before the timer fires.
    if (!m_timer.isActive())
        m_timer.startOneShot(0, BLINK_FROM_HERE);
    m_pendingIntersectionObservers.emplace(&observer);
}

void IntersectionObserverController::resume()
{
    // If the timer fired while DOM objects were suspended, notifications might be late, so deliver
    // them right away (rather than waiting for m_timer to fire again).
    if (m_timerFiredWhileSuspended) {
        m_timerFiredWhileSuspended = false;
        deliverIntersectionObservations(nullptr);
    }
}

void IntersectionObserverController::deliverIntersectionObservations(Timer<IntersectionObserverController>*)
{
    if (executionContext()->activeDOMObjectsAreSuspended()) {
        m_timerFiredWhileSuspended = true;
        return;
    }
    IntersectionObserverVector observers = m_pendingIntersectionObservers.GetSnapshot();
    m_pendingIntersectionObservers.clear();
    for (auto& observer : observers)
        observer->deliver();
}

void IntersectionObserverController::computeTrackedIntersectionObservations()
{
    // TODO(szager): Need to define timestamp.
    double timestamp = currentTime();
    for (auto& observer : m_trackedIntersectionObservers) {
        observer.computeIntersectionObservations(timestamp);
        if (observer.hasEntries())
            scheduleIntersectionObserverForDelivery(observer);
    }
}

void IntersectionObserverController::addTrackedObserver(IntersectionObserver& observer)
{
    m_trackedIntersectionObservers.emplace(&observer);
}

void IntersectionObserverController::removeTrackedObserversForRoot(const Node& root)
{
    for (auto it = m_trackedIntersectionObservers.begin(); m_trackedIntersectionObservers.end() != it; ++it)
    {
        if (it->root() == &root)
            it = m_trackedIntersectionObservers.erase(it);
    }
}

DEFINE_TRACE(IntersectionObserverController)
{
    // BKTODO: visitor->trace(m_trackedIntersectionObservers);
    visitor->trace(m_pendingIntersectionObservers);
    ActiveDOMObject::trace(visitor);
}

} // namespace blink
