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

#include "blinkit/app/app_impl.h"
#include "blinkit/ui/compositor/blink/layer.h"
#include "blinkit/ui/compositor/compositor.h"
#include "blinkit/ui/compositor/raster/raster_context.h"
#include "blinkit/ui/compositor/tasks/paint_ui_task.h"

namespace BlinKit {

RasterTask::RasterTask(const IntSize &viewportSize) : m_viewportSize(viewportSize)
{
}

RasterTask::~RasterTask(void) = default;

void RasterTask::Rasterize(const Layer &layer, const IntRect &visibleRect)
{
    m_result.emplace_back(layer.id(), visibleRect);
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

    ASSERT(m_paintTask);
    m_paintTask->PerformComposition(compositor, m_result, m_dirtyRect);

    ASSERT(m_taskRunner);
    m_taskRunner->postTask(BLINK_FROM_HERE, m_paintTask.release());
}

void RasterTask::SavePaintTask(std::unique_ptr<PaintUITask> &paintTask)
{
    ASSERT(!m_paintTask);
    ASSERT(paintTask);
    m_paintTask.swap(paintTask);

    m_taskRunner = AppImpl::Get().taskRunner();
}

} // namespace BlinKit
