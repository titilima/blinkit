// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: solid_color_scrollbar_layer.cpp
// Description: SolidColorScrollbarLayer Class
//      Author: Ziming Li
//     Created: 2021-12-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./solid_color_scrollbar_layer.h"

namespace BlinKit {

SolidColorScrollbarLayer::SolidColorScrollbarLayer(
    LayerClient *client,
    WebScrollbar::Orientation orientation,
    int thumbThickness,
    int trackStart,
    bool isLeftSideVerticalScrollbar)
    : Layer(client)
{
}

} // namespace BlinKit
