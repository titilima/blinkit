// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: visual_viewport_resize_event.cc
// Description: VisualViewportResizeEvent Class
//      Author: Ziming Li
//     Created: 2020-12-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/events/visual_viewport_resize_event.h"

#include "third_party/blink/renderer/core/dom/events/event_target.h"
#include "third_party/blink/renderer/core/event_type_names.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"

namespace blink {

VisualViewportResizeEvent::~VisualViewportResizeEvent() = default;

VisualViewportResizeEvent::VisualViewportResizeEvent()
    : Event(event_type_names::kResize, Bubbles::kNo, Cancelable::kNo) {}

void VisualViewportResizeEvent::DoneDispatchingEventAtCurrentTarget() {
  UseCounter::Count(currentTarget()->GetExecutionContext(),
                    WebFeature::kVisualViewportResizeFired);
}

}  // namespace blink
