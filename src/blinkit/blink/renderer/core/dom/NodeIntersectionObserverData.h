// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: NodeIntersectionObserverData.h
// Description: NodeIntersectionObserverData Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NodeIntersectionObserverData_h
#define NodeIntersectionObserverData_h

#include "blinkit/gc/gc_weak_object_set.h"
#include "platform/heap/Handle.h"

namespace blink {

class Node;
class IntersectionObservation;
class IntersectionObserver;

class NodeIntersectionObserverData : public GarbageCollectedFinalized<NodeIntersectionObserverData> {
public:
    DECLARE_TRACE();
    NodeIntersectionObserverData();
    ~NodeIntersectionObserverData();

    bool hasIntersectionObserver() const;
    bool hasIntersectionObservation() const;
    IntersectionObservation* getObservationFor(IntersectionObserver&);
    void addObservation(IntersectionObservation&);
    void removeObservation(IntersectionObserver&);
    void activateValidIntersectionObservers(Node&);
    void deactivateAllIntersectionObservers(Node&);

#if !ENABLE(OILPAN)
    void dispose();
#endif

    WeakPtrWillBeRawPtr<Node> createWeakPtr(Node*);

private:
    // IntersectionObservers for which the Node owning this data is root.
    BlinKit::GCWeakObjectSet<IntersectionObserver> m_intersectionObservers;
    // IntersectionObservations for which the Node owning this data is target.
    std::unordered_map<Member<IntersectionObserver>, Member<IntersectionObservation>> m_intersectionObservations;

#if !ENABLE(OILPAN)
    OwnPtr<WeakPtrFactory<Node>> m_weakPointerFactory;
#endif
};

} // namespace blink

#endif // NodeIntersectionObserverData_h
