// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_scrollbar_behavior.cpp
// Description: WinScrollbarBehavior Class
//      Author: Ziming Li
//     Created: 2021-10-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./win_scrollbar_behavior.h"

using namespace blink;

namespace BlinKit {

bool WinScrollbarBehavior::shouldCenterOnThumb(Button button, bool shiftKeyPressed, bool altKeyPressed)
{
    return false;
}

bool WinScrollbarBehavior::shouldSnapBackToDragOrigin(
    const IntPoint &eventPoint,
    const IntRect &scrollbarRect,
    bool isHorizontal)
{
    return false;
}

} // namespace BlinKit
