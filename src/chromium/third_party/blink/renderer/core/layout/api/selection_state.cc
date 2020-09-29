// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: selection_state.cc
// Description: SelectionState
//      Author: Ziming Li
//     Created: 2020-09-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/layout/api/selection_state.h"

#include "third_party/blink/renderer/platform/wtf/assertions.h"

namespace blink {

std::ostream& operator<<(std::ostream& out, const SelectionState state) {
  ASSERT(false); // BKTODO:
#if 0
  switch (state) {
    case SelectionState::kNone:
      return out << "None";
    case SelectionState::kStart:
      return out << "Start";
    case SelectionState::kInside:
      return out << "Inside";
    case SelectionState::kEnd:
      return out << "End";
    case SelectionState::kStartAndEnd:
      return out << "StartAndEnd";
    case SelectionState::kContain:
      return out << "Contain";
  }
#endif
  NOTREACHED();
  return out;
}

}  // namespace blink
