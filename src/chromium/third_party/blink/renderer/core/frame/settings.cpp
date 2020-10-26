// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: settings.cpp
// Description: Settings
//      Author: Ziming Li
//     Created: 2020-10-24
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "settings.h"

#include "third_party/blink/renderer/platform/font_family_names.h"

namespace blink {

const AtomicString& Settings::StandardFontFamilyName(void)
{
    return FontFamilyNames::Times_New_Roman; // See web_preferences.cc for the default value.
}

} // namespace blink
