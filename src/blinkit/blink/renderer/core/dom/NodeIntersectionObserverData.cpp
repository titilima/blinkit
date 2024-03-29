// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: NodeIntersectionObserverData.cpp
// Description: NodeIntersectionObserverData Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/dom/NodeIntersectionObserverData.h"

#include "core/dom/Document.h"
#include "core/dom/IntersectionObservation.h"
#include "core/dom/IntersectionObserver.h"
#include "core/dom/IntersectionObserverController.h"

namespace blink {

NodeIntersectionObserverData::NodeIntersectionObserverData() { }

NodeIntersectionObserverData::~NodeIntersectionObserverData() { }

bool NodeIntersectionObserverData::hasIntersectionObserver() const
{
    return !m_intersectionObservers.empty();
}

bool NodeIntersectionObserverData::hasIntersectionObservation() const
{
    return !m_intersectionObservations.empty();
}

IntersectionObservation* NodeIntersectionObserverData::getObservationFor(IntersectionObserver& observer)
{
    auto i = m_intersectionObservations.find(&observer);
    if (i == m_intersectionObservations.end())
        return nullptr;
    return i->second;
}

void NodeIntersectionObserverData::addObservation(IntersectionObservation& observation)
{
    m_intersectionObservations.emplace(&observation.observer(), &observation);
}

void NodeIntersectionObserverData::removeObservation(IntersectionObserver& observer)
{
    m_intersectionObservations.erase(&observer);
}

void NodeIntersectionObserverData::activateValidIntersectionObservers(Node& node)
{
    IntersectionObserverController& controller = node.document().ensureIntersectionObserverController();
    // Activate observers for which node is root.
    for (auto& observer : m_intersectionObservers) {
        controller.addTrackedObserver(observer);
        observer.setActive(true);
    }
    // A document can be root, but not target.
    if (node.isDocumentNode())
        return;
    // Active observers for which node is target.
    for (auto& observation : m_intersectionObservations)
        observation.second->setActive(observation.first->isDescendantOfRoot(&toElement(node)));
}

void NodeIntersectionObserverData::deactivateAllIntersectionObservers(Node& node)
{
    node.document().ensureIntersectionObserverController().removeTrackedObserversForRoot(node);
    for (auto& observer : m_intersectionObservers)
        observer.setActive(false);
    for (auto& observation : m_intersectionObservations)
        observation.second->setActive(false);
}

#if !ENABLE(OILPAN)
void NodeIntersectionObserverData::dispose()
{
    HeapVector<Member<IntersectionObserver>> observersToDisconnect;
    copyToVector(m_intersectionObservers, observersToDisconnect);
    for (auto& observer : observersToDisconnect)
        observer->disconnect();
    ASSERT(m_intersectionObservers.isEmpty());
}
#endif

WeakPtrWillBeRawPtr<Node> NodeIntersectionObserverData::createWeakPtr(Node* node)
{
#if ENABLE(OILPAN)
    return node;
#else
    if (!m_weakPointerFactory)
        m_weakPointerFactory = adoptPtrWillBeNoop(new WeakPtrFactory<Node>(node));
    WeakPtr<Node> result = m_weakPointerFactory->createWeakPtr();
    ASSERT(result.get() == node);
    return result;
#endif
}

DEFINE_TRACE(NodeIntersectionObserverData)
{
    // BKTODO: visitor->trace(m_intersectionObservers);
    visitor->trace(m_intersectionObservations);
}

} // namespace blink
