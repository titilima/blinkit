#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scrollbar_layer.h
// Description: ScrollbarLayer Class
//      Author: Ziming Li
//     Created: 2021-12-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCROLLBAR_LAYER_H
#define BLINKIT_SCROLLBAR_LAYER_H

#include "blinkit/blink/public/platform/WebScrollbar.h"
#include "blinkit/blink/public/platform/web_scrollbar_layer.h"
#include "blinkit/ui/compositor/layers/layer_client.h"

namespace BlinKit {

class Layer;

class ScrollbarLayer final : public WebScrollbarLayer, public LayerClient
{
public:
    ScrollbarLayer(std::unique_ptr<WebScrollbar> &&scrollbar, WebScrollbarThemePainter painter,
        std::unique_ptr<WebScrollbarThemeGeometry> &geometry);
    ScrollbarLayer(WebScrollbar::Orientation orientation, int thumbThickness, int trackStart,
        bool isLeftSideVerticalScrollbar);
private:
    WebLayer* layer(void) override;
    void setScrollLayer(WebLayer *layer) override;
    void PaintContents(WebDisplayItemList *displayItemList) override;

    int m_scrollLayerId;
    std::unique_ptr<Layer> m_layer;
};

} // namespace BlinKit

#endif // BLINKIT_SCROLLBAR_LAYER_H
