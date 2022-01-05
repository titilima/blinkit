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
#include "blinkit/blink/renderer/wtf/Allocator.h"

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
private:
    const FloatPoint m_offset;
};

} // namespace BlinKit

#endif // BLINKIT_RASTER_CONTEXT_H
