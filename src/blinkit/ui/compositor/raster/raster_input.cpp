#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_input.cpp
// Description: RasterInput Definitions
//      Author: Ziming Li
//     Created: 2022-01-11
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./raster_input.h"

#include "blinkit/ui/compositor/blink/layer.h"
#include "blinkit/ui/compositor/layers/layer_client.h"

namespace BlinKit {

LayerContext::LayerContext(const Layer &layer) : layerId(layer.id()), displayItems(std::make_unique<DisplayItemList>())
{
    layerRect.setSize(layer.Bounds());
    layer.Client()->PaintContents(*displayItems);
}

} // namespace BlinKit
