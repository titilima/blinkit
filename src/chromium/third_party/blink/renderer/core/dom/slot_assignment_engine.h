// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: slot_assignment_engine.h
// Description: SlotAssignmentEngine Class
//      Author: Ziming Li
//     Created: 2020-12-18
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_DOM_SLOT_ASSIGNMENT_ENGINE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_DOM_SLOT_ASSIGNMENT_ENGINE_H_

#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class ShadowRoot;

class SlotAssignmentEngine final {
 public:
  static std::unique_ptr<SlotAssignmentEngine> Create() { return base::WrapUnique(new SlotAssignmentEngine); }

  void AddShadowRootNeedingRecalc(ShadowRoot&);
  void RemoveShadowRootNeedingRecalc(ShadowRoot&);

  void Connected(ShadowRoot&);
  void Disconnected(ShadowRoot&);

  bool HasPendingSlotAssignmentRecalc() const {
    return !shadow_roots_needing_recalc_.IsEmpty();
  }

  void RecalcSlotAssignments();

  void Trace(blink::Visitor*);

 private:
  explicit SlotAssignmentEngine();

  HeapHashSet<WeakMember<ShadowRoot>> shadow_roots_needing_recalc_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_DOM_SLOT_ASSIGNMENT_ENGINE_H_
