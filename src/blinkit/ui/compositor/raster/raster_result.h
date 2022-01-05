#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_result.h
// Description: RasterResult Definitions
//      Author: Ziming Li
//     Created: 2022-01-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_RASTER_RESULT_H
#define BLINKIT_RASTER_RESULT_H

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"

namespace BlinKit {

struct LayerData {
    const int id;
    const IntRect rect;

    LayerData(int id, const IntRect &rect) : id(id), rect(rect) {}
};

using RasterResult = std::vector<LayerData>;

} // namespace BlinKit

#endif // BLINKIT_RASTER_RESULT_H
