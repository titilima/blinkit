// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_context.cpp
// Description: RasterContext Class
//      Author: Ziming Li
//     Created: 2022-01-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./raster_context.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/ui/compositor/blink/layer.h"

namespace BlinKit {

RasterContext::RasterContext(void) : m_transform(SkMatrix::I())
{
    ASSERT(isMainThread());
}

RasterContext::RasterContext(const RasterContext &parent, const Layer &currentLayer)
    : m_offset(parent.m_offset + toFloatSize(currentLayer.Position()))
    , m_scrollOffset(parent.m_scrollOffset + toDoubleSize(currentLayer.ScrollPosition()))
    , m_transform(SkMatrix::Concat(parent.m_transform, currentLayer.Transform()))
{
    ASSERT(isMainThread());
}

IntPoint RasterContext::CalculatePosition(const FloatPoint &position) const
{
    FloatPoint ret(position + m_offset);
    if (!m_scrollOffset.isZero())
        ret -= toFloatSize(m_scrollOffset);
    return roundedIntPoint(ret);
}

IntRect RasterContext::CalculateVisibleRect(
    const FloatPoint &layerPosition, const IntSize &layerBounds,
    const IntRect &viewportRect) const
{
    ASSERT(m_transform.isIdentity()); // BKTODO:

    IntRect layerRect(roundedIntPoint(layerPosition + m_offset), layerBounds);
    if (!m_scrollOffset.isZero())
        layerRect.contract(roundedIntSize(m_scrollOffset));
    return intersection(layerRect, viewportRect);
}

} // namespace BlinKit
