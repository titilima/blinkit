#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: IntersectionObserver.h
// Description: IntersectionObserver Class
//      Author: Ziming Li
//     Created: 2021-08-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IntersectionObserver_h
#define IntersectionObserver_h

#include "blinkit/blink/renderer/core/dom/element.h"
#include "blinkit/blink/renderer/core/dom/IntersectionObservation.h"
#include "blinkit/blink/renderer/core/dom/IntersectionObserverEntry.h"
#include "blinkit/blink/renderer/wtf/HashSet.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"
#include "blinkit/blink/renderer/wtf/Vector.h"

namespace blink {

class ExceptionState;
class IntersectionObserverCallback;
class IntersectionObserverInit;

// TODO(oilpan): Switch to GarbageCollected<> after oilpan ships
class IntersectionObserver final : public BlinKit::GCObject, public ScriptWrappable
{
    DEFINE_WRAPPERTYPEINFO();

public:
    static IntersectionObserver* create(const IntersectionObserverInit&, IntersectionObserverCallback&, ExceptionState&);
    static void resumeSuspendedObservers();

    BLINKIT_DEFINE_GC_CASTER(IntersectionObserver)

    // API methods
    void observe(Element*, ExceptionState&);
    void unobserve(Element*, ExceptionState&);
    HeapVector<Member<IntersectionObserverEntry>> takeRecords();

    Node* root() const { return m_root.get(); }
    LayoutObject* rootLayoutObject() const;
    bool hasPercentMargin() const;
    const Length& topMargin() const { return m_topMargin; }
    const Length& rightMargin() const { return m_rightMargin; }
    const Length& bottomMargin() const { return m_bottomMargin; }
    const Length& leftMargin() const { return m_leftMargin; }
    bool isDescendantOfRoot(const Element*) const;
    void computeIntersectionObservations(double timestamp);
    void enqueueIntersectionObserverEntry(IntersectionObserverEntry&);
    void applyRootMargin(LayoutRect&) const;
    unsigned firstThresholdGreaterThan(float ratio) const;
    void deliver();
    void setActive(bool);
    void disconnect();
    void removeObservation(IntersectionObservation&);
    bool hasEntries() const { return m_entries.size(); }

    DECLARE_TRACE();

private:
    explicit IntersectionObserver(IntersectionObserverCallback&, Node&, const Vector<Length>& rootMargin, const Vector<float>& thresholds);

    void checkRootAndDetachIfNeeded();

    Member<IntersectionObserverCallback> m_callback;
    WeakPtrWillBeWeakMember<Node> m_root;
    HeapHashSet<WeakMember<IntersectionObservation>> m_observations;
    HeapVector<Member<IntersectionObserverEntry>> m_entries;
    Vector<float> m_thresholds;
    Length m_topMargin;
    Length m_rightMargin;
    Length m_bottomMargin;
    Length m_leftMargin;
};

} // namespace blink

#endif // IntersectionObserver_h
