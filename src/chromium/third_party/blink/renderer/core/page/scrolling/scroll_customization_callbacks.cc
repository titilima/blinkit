// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: scroll_customization_callbacks.cc
// Description: ScrollCustomizationCallbacks Class
//      Author: Ziming Li
//     Created: 2020-12-18
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/page/scrolling/scroll_customization_callbacks.h"

#include "third_party/blink/renderer/core/page/scrolling/scroll_state_callback.h"

namespace blink {

void ScrollCustomizationCallbacks::SetDistributeScroll(
    Element* element,
    ScrollStateCallback* scroll_state_callback) {
  distribute_scroll_callbacks_[element] = scroll_state_callback;
}

ScrollStateCallback* ScrollCustomizationCallbacks::GetDistributeScroll(
    Element* element) {
  auto it = distribute_scroll_callbacks_.find(element);
  if (it == distribute_scroll_callbacks_.end())
    return nullptr;
  return it->second.Get();
}

void ScrollCustomizationCallbacks::SetApplyScroll(
    Element* element,
    ScrollStateCallback* scroll_state_callback) {
  apply_scroll_callbacks_[element] = scroll_state_callback;
}

void ScrollCustomizationCallbacks::RemoveApplyScroll(Element* element) {
  apply_scroll_callbacks_.erase(element);
}

ScrollStateCallback* ScrollCustomizationCallbacks::GetApplyScroll(
    Element* element) {
  auto it = apply_scroll_callbacks_.find(element);
  if (it == apply_scroll_callbacks_.end())
    return nullptr;
  return it->second.Get();
}

bool ScrollCustomizationCallbacks::InScrollPhase(Element* element) const {
  return in_scrolling_phase_.Contains(element) &&
         in_scrolling_phase_.at(element);
}

void ScrollCustomizationCallbacks::SetInScrollPhase(Element* element,
                                                    bool value) {
  DCHECK(element);
  in_scrolling_phase_[element] = value;
}

}  // namespace blink
