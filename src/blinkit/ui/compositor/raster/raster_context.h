#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_context.h
// Description: RasterContext Class
//      Author: Ziming Li
//     Created: 2022-01-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_RASTER_CONTEXT_H
#define BLINKIT_RASTER_CONTEXT_H

#include "blinkit/blink/renderer/platform/geometry/DoubleSize.h"
#include "blinkit/blink/renderer/platform/geometry/FloatPoint.h"
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/skia/include/core/SkMatrix.h"

namespace BlinKit {

class Layer;

class RasterContext
{
    STACK_ALLOCATED();
public:
    RasterContext(void);
    RasterContext(const RasterContext &parent, const Layer &currentLayer);

    IntPoint CalculatePosition(const FloatPoint &position) const;
    IntRect CalculateVisibleRect(const FloatPoint &layerPosition, const IntSize &layerBounds,
        const IntRect &viewportRect) const;
private:
    const FloatSize m_offset;
    const DoubleSize m_scrollOffset;
    const SkMatrix m_transform;
};

} // namespace BlinKit

#endif // BLINKIT_RASTER_CONTEXT_H
