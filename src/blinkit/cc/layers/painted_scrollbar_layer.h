#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: painted_scrollbar_layer.h
// Description: PaintedScrollbarLayer Class
//      Author: Ziming Li
//     Created: 2021-12-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_PAINTED_SCROLLBAR_LAYER_H
#define BLINKIT_PAINTED_SCROLLBAR_LAYER_H

#include "blinkit/blink/public/platform/WebScrollbarThemePainter.h"
#include "blinkit/cc/blink/layer.h"

namespace blink {
class WebScrollbarThemeGeometry;
}

namespace BlinKit {

class PaintedScrollbarLayer final : public Layer
{
public:
    PaintedScrollbarLayer(std::unique_ptr<blink::WebScrollbar> &&scrollbar, blink::WebScrollbarThemePainter painter,
        std::unique_ptr<blink::WebScrollbarThemeGeometry> &geometry);
};

} // namespace BlinKit

#endif // BLINKIT_PAINTED_SCROLLBAR_LAYER_H
