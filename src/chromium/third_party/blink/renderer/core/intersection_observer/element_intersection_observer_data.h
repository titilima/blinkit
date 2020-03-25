// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: element_intersection_observer_data.h
// Description: ElementIntersectionObserverData Class
//      Author: Ziming Li
//     Created: 2020-03-22
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_INTERSECTION_OBSERVER_ELEMENT_INTERSECTION_OBSERVER_DATA_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_INTERSECTION_OBSERVER_ELEMENT_INTERSECTION_OBSERVER_DATA_H_

// BKTODO: #include "third_party/blink/renderer/core/dom/dom_high_res_time_stamp.h"
#include "third_party/blink/renderer/platform/bindings/name_client.h"
// BKTODO: #include "third_party/blink/renderer/platform/bindings/trace_wrapper_member.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class IntersectionObservation;
class IntersectionObserver;

class ElementIntersectionObserverData
    : public GarbageCollected<ElementIntersectionObserverData>,
      public NameClient {
 public:
  bool HasObservations(void) const
  {
    ASSERT(false); // BKTODO:
    return false;
  }
#if 0 // BKTODO:
  ElementIntersectionObserverData();

  IntersectionObservation* GetObservationFor(IntersectionObserver&);
  void AddObservation(IntersectionObservation&);
  void RemoveObservation(IntersectionObserver&);
  bool HasObservations() const { return !intersection_observations_.IsEmpty(); }
  void ComputeObservations(unsigned flags);

  void Trace(blink::Visitor*);
  const char* NameInHeapSnapshot() const override {
    return "ElementIntersectionObserverData";
  }

 private:
  // IntersectionObservations for which the Node owning this data is target.
  HeapHashMap<TraceWrapperMember<IntersectionObserver>,
              Member<IntersectionObservation>>
      intersection_observations_;
#endif
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_INTERSECTION_OBSERVER_ELEMENT_INTERSECTION_OBSERVER_DATA_H_
