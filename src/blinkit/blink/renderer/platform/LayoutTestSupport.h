#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LayoutTestSupport.h
// Description: LayoutTestSupport Class
//      Author: Ziming Li
//     Created: 2021-07-28
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LAYOUT_TEST_SUPPORT_H
#define BLINKIT_BLINKIT_LAYOUT_TEST_SUPPORT_H

#include "blinkit/blink/renderer/wtf/Allocator.h"

namespace blink {

class LayoutTestSupport
{
    STATIC_ONLY(LayoutTestSupport);
public:
    static constexpr bool alwaysUseComplexTextForTest(void) { return false; }
};

} // namespace blink

#endif // BLINKIT_BLINKIT_LAYOUT_TEST_SUPPORT_H
