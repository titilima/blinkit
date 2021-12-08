// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositor_support.cpp
// Description: CompositorSupport Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./compositor_support.h"

#include "blinkit/cc/blink/content_layer.h"
#include "blinkit/cc/blink/scrollbar_layer.h"

using namespace blink;

namespace BlinKit {

WebContentLayer* CompositorSupport::createContentLayer(WebContentLayerClient *client)
{
    return new ContentLayer(client);
}

WebScrollbarLayer* CompositorSupport::createScrollbarLayer(
    std::unique_ptr<WebScrollbar> &&scrollbar,
    WebScrollbarThemePainter painter,
    std::unique_ptr<WebScrollbarThemeGeometry> &geometry)
{
    return new ScrollbarLayer(std::move(scrollbar), painter, geometry);
}

WebScrollbarLayer* CompositorSupport::createSolidColorScrollbarLayer(
    WebScrollbar::Orientation orientation,
    int thumbThickness,
    int trackStart,
    bool isLeftSideVerticalScrollbar)
{
    return new ScrollbarLayer(orientation, thumbThickness, trackStart, isLeftSideVerticalScrollbar);
}

} // namespace BlinKit
