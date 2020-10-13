// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: color_interpolation_custom.cc
// Description: ColorInterpolation Class
//      Author: Ziming Li
//     Created: 2020-10-13
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/properties/longhands/color_interpolation.h"

namespace blink {
namespace CSSLonghand {

const CSSValue* ColorInterpolation::CSSValueFromComputedStyleInternal(
    const ComputedStyle& style,
    const SVGComputedStyle& svg_style,
    const LayoutObject*,
    Node*,
    bool allow_visited_style) const {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return CSSIdentifierValue::Create(svg_style.ColorInterpolation());
#endif
}

}  // namespace CSSLonghand
}  // namespace blink
