// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: animation_playback_event.cc
// Description: AnimationPlaybackEvent Class
//      Author: Ziming Li
//     Created: 2020-09-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/events/animation_playback_event.h"

// BKTODO: #include "third_party/blink/renderer/core/event_names.h"

namespace blink {

AnimationPlaybackEvent::AnimationPlaybackEvent(const AtomicString& type,
                                               double current_time,
                                               double timeline_time)
    : Event(type, Bubbles::kNo, Cancelable::kNo),
      current_time_(current_time),
      timeline_time_(timeline_time) {}

AnimationPlaybackEvent::AnimationPlaybackEvent(
    const AtomicString& type,
    const AnimationPlaybackEventInit& initializer)
#if 0 // BKTODO:
    : Event(type, initializer) {
  if (initializer.hasCurrentTime())
    current_time_ = initializer.currentTime();
  if (initializer.hasTimelineTime())
    timeline_time_ = initializer.timelineTime();
}
#else
{
  ASSERT(false); // BKTODO:
}
#endif

AnimationPlaybackEvent::~AnimationPlaybackEvent() = default;

double AnimationPlaybackEvent::currentTime(bool& is_null) const {
  is_null = !current_time_.has_value();
  return current_time_.value_or(0);
}

double AnimationPlaybackEvent::timelineTime(bool& is_null) const {
  is_null = !timeline_time_.has_value();
  return timeline_time_.value_or(0);
}

const AtomicString& AnimationPlaybackEvent::InterfaceName() const {
  ASSERT(false); // BKTODO:
  return g_null_atom;
#if 0
  return EventNames::AnimationPlaybackEvent;
#endif
}

}  // namespace blink
