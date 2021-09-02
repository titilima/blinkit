// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: IntersectionObserverController.h
// Description: IntersectionObserverController Class
//      Author: Ziming Li
//     Created: 2021-07-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IntersectionObserverController_h
#define IntersectionObserverController_h

#include "blinkit/gc/gc_object_set.h"
#include "blinkit/gc/gc_weak_object_set.h"
#include "core/dom/Element.h"
#include "core/dom/IntersectionObserver.h"
#include "platform/Timer.h"
#include "platform/heap/Handle.h"

// Design doc for IntersectionObserver implementation:
//   https://docs.google.com/a/google.com/document/d/1hLK0eyT5_BzyNS4OkjsnoqqFQDYCbKfyBinj94OnLiQ

namespace blink {

class IntersectionObserverController : public ActiveDOMObject {
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(IntersectionObserverController);
public:
    static GCPassPtr<IntersectionObserverController> create(Document*);
    ~IntersectionObserverController();

    void resume() override;

    void scheduleIntersectionObserverForDelivery(IntersectionObserver&);
    void deliverIntersectionObservations(Timer<IntersectionObserverController>*);
    void computeTrackedIntersectionObservations();
    void addTrackedObserver(IntersectionObserver&);
    void removeTrackedObserversForRoot(const Node&);

    DECLARE_TRACE();

private:
    explicit IntersectionObserverController(Document*);

private:
    Timer<IntersectionObserverController> m_timer;
    // IntersectionObservers for which this is the tracking document.
    BlinKit::GCWeakObjectSet<IntersectionObserver> m_trackedIntersectionObservers;
    // IntersectionObservers for which this is the execution context of the callback.
    BlinKit::GCObjectSet<IntersectionObserver> m_pendingIntersectionObservers;

    bool m_timerFiredWhileSuspended;
};

} // namespace blink

#endif // IntersectionObserverController_h
