#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_input.h
// Description: RasterInput Definitions
//      Author: Ziming Li
//     Created: 2022-01-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_RASTER_INPUT_H
#define BLINKIT_RASTER_INPUT_H

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/ui/compositor/blink/display_item_list.h"

namespace BlinKit {

struct PaintContext {
    const int layerId;
    const IntSize layerBounds;
    const IntRect dirtyRect;
    std::unique_ptr<DisplayItemList> displayItems;

    PaintContext(int layerId, const IntSize &layerBounds, const IntRect &dirtyRect)
        : layerId(layerId), layerBounds(layerBounds), dirtyRect(dirtyRect)
        , displayItems(std::make_unique<DisplayItemList>())
    {
    }
};

using RasterInput = std::vector<PaintContext>;

} // namespace BlinKit

#endif // BLINKIT_RASTER_INPUT_H
