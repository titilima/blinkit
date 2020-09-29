// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_segmentation_machine_state.cc
// Description: TextSegmentationMachineState
//      Author: Ziming Li
//     Created: 2020-09-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/editing/state_machines/text_segmentation_machine_state.h"

#include <ostream>  // NOLINT
#include "third_party/blink/renderer/platform/wtf/assertions.h"

namespace blink {

std::ostream& operator<<(std::ostream& os, TextSegmentationMachineState state) {
  static const char* const kTexts[] = {
      "Invalid", "NeedMoreCodeUnit", "NeedFollowingCodeUnit", "Finished",
  };

  auto* const* const it = std::begin(kTexts) + static_cast<size_t>(state);
  DCHECK_GE(it, std::begin(kTexts)); // Unknown state value
  DCHECK_LT(it, std::end(kTexts)); // Unknown state value
  return os << *it;
}

}  // namespace blink
