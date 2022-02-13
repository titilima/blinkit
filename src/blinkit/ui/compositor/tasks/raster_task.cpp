// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_task.cpp
// Description: RasterTask Class
//      Author: Ziming Li
//     Created: 2021-12-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./raster_task.h"

#include "blinkit/ui/compositor/blink/layer.h"
#include "blinkit/ui/compositor/compositor.h"
#include "blinkit/ui/compositor/raster/raster_context.h"

namespace BlinKit {

RasterTask::RasterTask(const IntSize &viewportSize, AnimationProxy &proxy)
    : m_viewportSize(viewportSize), m_proxy(proxy)
{
}

void RasterTask::Rasterize(const Layer &layer, const IntRect &visibleRect)
{
    LayerData &layerData = m_result.emplace_back(layer.id(), visibleRect);
    layerData.opaque = layer.Opaque();
}

LayerContext& RasterTask::RequireLayerContext(const Layer &layer, const IntSize &layerBounds)
{
    ASSERT(layer.drawsContent());
    return m_input.emplace_back(layer, layerBounds);
}

void RasterTask::Run(Compositor &compositor)
{
    for (const LayerContext &context : m_input)
        compositor.UpdateSnapshot(context, m_viewportSize);
    compositor.PerformComposition(m_proxy, m_viewportSize, m_result, m_dirtyRect);
}

} // namespace BlinKit
