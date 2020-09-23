// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: style_color.cc
// Description: StyleColor Class
//      Author: Ziming Li
//     Created: 2020-09-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/style_color.h"

#include "third_party/blink/renderer/core/layout/layout_theme.h"

namespace blink {

Color StyleColor::ColorFromKeyword(CSSValueID keyword) {
  if (const char* value_name = getValueName(keyword)) {
    if (const NamedColor* named_color =
            FindColor(value_name, static_cast<wtf_size_t>(strlen(value_name))))
      return Color(named_color->argb_value);
  }
  return LayoutTheme::GetTheme().SystemColor(keyword);
}

bool StyleColor::IsColorKeyword(CSSValueID id) {
  // Named colors and color keywords:
  //
  // <named-color>
  //   'aqua', 'black', 'blue', ..., 'yellow' (CSS3: "basic color keywords")
  //   'aliceblue', ..., 'yellowgreen'        (CSS3: "extended color keywords")
  //   'transparent'
  //
  // 'currentcolor'
  //
  // <deprecated-system-color>
  //   'ActiveBorder', ..., 'WindowText'
  //
  // WebKit proprietary/internal:
  //   '-webkit-link'
  //   '-webkit-activelink'
  //   '-internal-active-list-box-selection'
  //   '-internal-active-list-box-selection-text'
  //   '-internal-inactive-list-box-selection'
  //   '-internal-inactive-list-box-selection-text'
  //   '-webkit-focus-ring-color'
  //   '-internal-quirk-inherit'
  //
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return (id >= CSSValueAqua && id <= CSSValueInternalQuirkInherit) ||
         (id >= CSSValueAliceblue && id <= CSSValueYellowgreen) ||
         id == CSSValueMenu;
#endif
}

bool StyleColor::IsSystemColor(CSSValueID id) {
  return (id >= CSSValueActiveborder && id <= CSSValueWindowtext) ||
         id == CSSValueMenu;
}

}  // namespace blink
