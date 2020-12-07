// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: FontSelector.cc
// Description: FontCacheClient Class
//      Author: Ziming Li
//     Created: 2020-10-22
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/fonts/font_selector.h"

#include "build/build_config.h"
#include "third_party/blink/renderer/core/frame/settings.h"
#include "third_party/blink/renderer/platform/fonts/font_cache.h"
#include "third_party/blink/renderer/platform/fonts/font_description.h"

namespace blink {

AtomicString FontSelector::FamilyNameFromSettings(
    const FontDescription& font_description,
    const AtomicString& generic_family_name) {
#if defined(OS_ANDROID)
  if (font_description.GenericFamily() == FontDescription::kStandardFamily) {
    return FontCache::GetGenericFamilyNameForScript(
        FontFamilyNames::webkit_standard, font_description);
  }

  if (generic_family_name.StartsWith("-webkit-")) {
    return FontCache::GetGenericFamilyNameForScript(generic_family_name,
                                                    font_description);
  }
#else
  if (font_description.GenericFamily() == FontDescription::kStandardFamily)
    return Settings::StandardFontFamilyName();
  if (generic_family_name == FontFamilyNames::webkit_serif)
    ASSERT(false); // BKTODO: return settings.Serif(script);
  if (generic_family_name == FontFamilyNames::webkit_sans_serif)
    ASSERT(false); // BKTODO: return settings.SansSerif(script);
  if (generic_family_name == FontFamilyNames::webkit_cursive)
    ASSERT(false); // BKTODO: return settings.Cursive(script);
  if (generic_family_name == FontFamilyNames::webkit_fantasy)
    ASSERT(false); // BKTODO: return settings.Fantasy(script);
  if (generic_family_name == FontFamilyNames::webkit_monospace)
    ASSERT(false); // BKTODO: return settings.Fixed(script);
  if (generic_family_name == FontFamilyNames::webkit_pictograph)
    ASSERT(false); // BKTODO: return settings.Pictograph(script);
  if (generic_family_name == FontFamilyNames::webkit_standard)
    ASSERT(false); // BKTODO: return settings.Standard(script);
#endif
  return g_empty_atom;
}

}  // namespace blink
