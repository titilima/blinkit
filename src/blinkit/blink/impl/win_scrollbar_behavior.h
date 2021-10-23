#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_scrollbar_behavior.h
// Description: WinScrollbarBehavior Class
//      Author: Ziming Li
//     Created: 2021-10-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_SCROLLBAR_BEHAVIOR_H
#define BLINKIT_BLINKIT_WIN_SCROLLBAR_BEHAVIOR_H

#include "blinkit/blink/public/platform/WebScrollbarBehavior.h"

namespace BlinKit {

class WinScrollbarBehavior final : public blink::WebScrollbarBehavior
{
public:
private:
    bool shouldCenterOnThumb(Button button, bool shiftKeyPressed, bool altKeyPressed) override;
    bool shouldSnapBackToDragOrigin(const blink::IntPoint &eventPoint, const blink::IntRect &scrollbarRect,
        bool isHorizontal) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_SCROLLBAR_BEHAVIOR_H
