// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_font_family_value.cc
// Description: CSSFontFamilyValue Class
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/css_font_family_value.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/css/css_markup.h"
#include "third_party/blink/renderer/core/css/css_value_pool.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

std::shared_ptr<CSSFontFamilyValue> CSSFontFamilyValue::Create(const String& family_name) {
  if (family_name.IsNull())
    return base::WrapShared(new CSSFontFamilyValue(family_name));
  std::shared_ptr<CSSFontFamilyValue> &entry =
      CssValuePool().GetFontFamilyCacheEntry(family_name);
  if (!entry)
    entry.reset(new CSSFontFamilyValue(family_name));
  return entry;
}

CSSFontFamilyValue::CSSFontFamilyValue(const String& str)
    : CSSValue(kFontFamilyClass), string_(str) {}

String CSSFontFamilyValue::CustomCSSText() const {
  return SerializeFontFamily(string_);
}

}  // namespace blink
