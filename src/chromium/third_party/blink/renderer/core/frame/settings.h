// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: settings.h
// Description: Settings
//      Author: Ziming Li
//     Created: 2020-10-24
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SETTINGS_H
#define BLINKIT_BLINK_SETTINGS_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {

class Settings
{
public:
    static constexpr int DefaultFontSize = 16;
    static constexpr int DefaultFixedFontSize = 13;
    static constexpr int MinimumFontSize = 0;
    static constexpr int MinimumLogicalFontSize = 6;

    static const AtomicString& StandardFontFamilyName(void);
};

} // namespace blink

#endif // BLINKIT_BLINK_SETTINGS_H
