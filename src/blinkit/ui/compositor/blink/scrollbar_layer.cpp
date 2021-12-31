// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scrollbar_layer.cpp
// Description: ScrollbarLayer Class
//      Author: Ziming Li
//     Created: 2021-12-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./scrollbar_layer.h"

#include "blinkit/ui/compositor/layers/painted_scrollbar_layer.h"
#include "blinkit/ui/compositor/layers/solid_color_scrollbar_layer.h"

using namespace blink;

namespace BlinKit {

ScrollbarLayer::ScrollbarLayer(
    std::unique_ptr<WebScrollbar> &&scrollbar,
    WebScrollbarThemePainter painter,
    std::unique_ptr<WebScrollbarThemeGeometry> &geometry)
    : m_scrollLayerId(Layer::INVALID_ID)
    , m_layer(std::make_unique<PaintedScrollbarLayer>(this, std::move(scrollbar), painter, geometry))
{
}

ScrollbarLayer::ScrollbarLayer(
    WebScrollbar::Orientation orientation,
    int thumbThickness,
    int trackStart,
    bool isLeftSideVerticalScrollbar)
    : m_scrollLayerId(Layer::INVALID_ID)
    , m_layer(std::make_unique<SolidColorScrollbarLayer>(this, orientation, thumbThickness, trackStart, isLeftSideVerticalScrollbar))
{
}

WebLayer* ScrollbarLayer::layer(void)
{
    return m_layer.get();
}

void ScrollbarLayer::PaintContents(WebDisplayItemList *displayItemList)
{
    // ASSERT(false); // BKTODO:
}

void ScrollbarLayer::setScrollLayer(WebLayer *layer)
{
    int layerId = nullptr != layer
        ? layer->id()
        : Layer::INVALID_ID;

    if (m_scrollLayerId == layerId)
        return;

    m_scrollLayerId = layerId;
    m_layer->SetNeedsFullTreeSync();
}

} // namespace BlinKit
