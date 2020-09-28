// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: composition_marker.cc
// Description: CompositionMarker Class
//      Author: Ziming Li
//     Created: 2020-09-28
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/editing/markers/composition_marker.h"

namespace blink {

#if 0 // BKTODO: Check if necessary.
CompositionMarker::CompositionMarker(unsigned start_offset,
                                     unsigned end_offset,
                                     Color underline_color,
                                     ws::mojom::ImeTextSpanThickness thickness,
                                     Color background_color)
    : StyleableMarker(start_offset,
                      end_offset,
                      underline_color,
                      thickness,
                      background_color) {}

DocumentMarker::MarkerType CompositionMarker::GetType() const {
  return DocumentMarker::kComposition;
}
#endif

}  // namespace blink
