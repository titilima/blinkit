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

#include "blinkit/blink/renderer/platform/geometry/FloatPoint.h"
#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"
#include "third_party/skia/include/core/SkMatrix.h"

namespace BlinKit {

class Layer;

class RasterContext
{
    STACK_ALLOCATED();
public:
    RasterContext(void);
    RasterContext(const RasterContext &parent, const Layer &currentLayer);

    IntPoint CalculateOffset(const FloatPoint &position) const {
        return roundedIntPoint(m_offset + position);
    }
    IntRect CalculateLayerRect(const Layer &layer) const;
    IntRect CalculateDirtyRectForLayer(const Layer &layer, const IntSize &viewportSize) const;
    IntRect CalculateVisibleRect(const Layer &layer, const IntSize &viewportSize) const;
    SkMatrix CalculateTransform(const Layer &layer) const;
private:
    const FloatPoint m_offset;
    const SkMatrix m_transform;
};

} // namespace BlinKit

#endif // BLINKIT_RASTER_CONTEXT_H
