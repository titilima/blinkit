// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: painted_scrollbar_layer.cpp
// Description: PaintedScrollbarLayer Class
//      Author: Ziming Li
//     Created: 2021-12-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./painted_scrollbar_layer.h"

namespace BlinKit {

PaintedScrollbarLayer::PaintedScrollbarLayer(
    LayerClient *client,
    std::unique_ptr<WebScrollbar> &&scrollbar,
    WebScrollbarThemePainter painter,
    std::unique_ptr<WebScrollbarThemeGeometry> &geometry)
    : Layer(client)
{
}

} // namespace BlinKit
