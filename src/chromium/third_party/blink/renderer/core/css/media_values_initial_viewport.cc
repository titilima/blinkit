// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: media_values_initial_viewport.cc
// Description: MediaValuesInitialViewport Class
//      Author: Ziming Li
//     Created: 2020-09-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/media_values_initial_viewport.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"

namespace blink {

MediaValuesInitialViewport* MediaValuesInitialViewport::Create(
    LocalFrame& frame) {
  return new MediaValuesInitialViewport(frame);
}

MediaValuesInitialViewport::MediaValuesInitialViewport(LocalFrame& frame)
    : MediaValuesDynamic(&frame) {}

double MediaValuesInitialViewport::ViewportWidth() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  DCHECK(frame_->View());
  return frame_->View()->InitialViewportWidth();
#endif
}

double MediaValuesInitialViewport::ViewportHeight() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  DCHECK(frame_->View());
  return frame_->View()->InitialViewportHeight();
#endif
}

}  // namespace blink
