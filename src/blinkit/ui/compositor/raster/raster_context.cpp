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
    : m_offset(parent.m_offset + currentLayer.Position())
    , m_transform(SkMatrix::Concat(parent.m_transform, currentLayer.Transform()))
{
    ASSERT(isMainThread());
}

IntRect RasterContext::CalculateDirtyRectForLayer(const Layer &layer, const IntSize &viewportSize) const
{
    ASSERT(m_transform.isIdentity()); // BKTODO:

    IntRect layerRect(roundedIntPoint(m_offset + layer.Position()), layer.Bounds());
    layerRect.intersect(IntRect(IntPoint(), viewportSize));
    return layerRect;
}

IntRect RasterContext::CalculateLayerRect(const Layer &layer) const
{
    return IntRect(roundedIntPoint(m_offset + layer.Position()), layer.Bounds());
}

IntRect RasterContext::CalculateVisibleRect(const Layer &layer, const IntSize &viewportSize) const
{
    ASSERT(m_transform.isIdentity()); // BKTODO:

    IntRect layerRect(roundedIntPoint(m_offset + layer.Position()), layer.Bounds());
    layerRect.intersect(IntRect(IntPoint(), viewportSize));
    return layerRect;
}

} // namespace BlinKit
