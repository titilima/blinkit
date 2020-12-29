// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: icu_error.cc
// Description: ICUError Class
//      Author: Ziming Li
//     Created: 2020-12-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/text/icu_error.h"

namespace blink {

// Distinguish memory allocation failures from other errors.
// https://groups.google.com/a/chromium.org/d/msg/platform-architecture-dev/MP0k9WGnCjA/zIBiJtilBwAJ
static NOINLINE void ICUOutOfMemory() {
  ASSERT(false);
  exit(EXIT_FAILURE);
}

void ICUError::HandleFailure() {
  switch (error_) {
    case U_MEMORY_ALLOCATION_ERROR:
      ICUOutOfMemory();
      break;
    case U_ILLEGAL_ARGUMENT_ERROR:
      CHECK(false) << error_;
      break;
    default:
      break;
  }
}

}  // namespace blink
