#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: solid_color_scrollbar_layer.h
// Description: SolidColorScrollbarLayer Class
//      Author: Ziming Li
//     Created: 2021-12-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SOLID_COLOR_SCROLLBAR_LAYER_H
#define BLINKIT_SOLID_COLOR_SCROLLBAR_LAYER_H

#include "blinkit/blink/public/platform/web_scrollbar.h"
#include "blinkit/ui/compositor/blink/layer.h"

namespace BlinKit {

class SolidColorScrollbarLayer final : public Layer
{
public:
    SolidColorScrollbarLayer(LayerClient *client, WebScrollbar::Orientation orientation, int thumbThickness,
        int trackStart, bool isLeftSideVerticalScrollbar);
};

} // namespace BlinKit

#endif // BLINKIT_SOLID_COLOR_SCROLLBAR_LAYER_H
