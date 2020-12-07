// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: layout_test_support.h
// Description: LayoutTestSupport Class
//      Author: Ziming Li
//     Created: 2020-12-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_LAYOUT_TEST_SUPPORT_H
#define BLINKIT_BLINK_LAYOUT_TEST_SUPPORT_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

class LayoutTestSupport final
{
    STATIC_ONLY(LayoutTestSupport);
public:
    static constexpr bool IsRunningLayoutTest(void) { return false; }

    static constexpr bool IsFontAntialiasingEnabledForTest(void) { return false; }
    static constexpr bool IsMockThemeEnabledForTest(void) { return false; }
    static constexpr bool IsTextSubpixelPositioningAllowedForTest(void) { return true; }
};

} // namespace blink

#endif // BLINKIT_BLINK_LAYOUT_TEST_SUPPORT_H
