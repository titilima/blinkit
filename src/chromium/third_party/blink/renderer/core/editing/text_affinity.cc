// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text_affinity.cc
// Description: TextAffinity
//      Author: Ziming Li
//     Created: 2020-09-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/editing/text_affinity.h"

#include <ostream>  // NOLINT
// BKTODO: #include "third_party/blink/public/web/web_ax_enums.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"

namespace blink {

std::ostream& operator<<(std::ostream& ostream, TextAffinity affinity) {
  switch (affinity) {
    case TextAffinity::kDownstream:
      return ostream << "TextAffinity::Downstream";
    case TextAffinity::kUpstream:
      return ostream << "TextAffinity::Upstream";
  }
  return ostream << "TextAffinity(" << static_cast<int>(affinity) << ')';
}

}  // namespace blink
