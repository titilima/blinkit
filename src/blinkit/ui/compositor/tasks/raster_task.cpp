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
#include "blinkit/ui/compositor/layers/layer_client.h"
#include "blinkit/ui/compositor/raster/raster_context.h"
#include "blinkit/ui/compositor/tasks/paint_ui_task.h"

namespace BlinKit {

RasterTask::RasterTask(const IntSize &viewportSize) : m_viewportSize(viewportSize)
{
}

RasterTask::~RasterTask(void) = default;

void RasterTask::AddDirtyLayer(Layer &layer, const IntRect &layerRect)
{
    ASSERT(layer.drawsContent());

    const IntRect dirtyRectInLayer = layer.TakeDirtyRect();

    IntRect dirtyRect(dirtyRectInLayer);
    dirtyRect.moveBy(layerRect.location());
    m_dirtyRect.unite(dirtyRect);

    PaintContext &paintContext = m_input.emplace_back(layer.id(), layer.Bounds(), dirtyRectInLayer);
    layer.Client()->PaintContents(*paintContext.displayItems);
}

void RasterTask::Rasterize(const Layer &layer, const IntRect &layerRect)
{
    m_result.emplace_back(layer.id(), layerRect);
}

void RasterTask::Run(Compositor &compositor)
{
    ASSERT(!HasNothingToDo());

    for (const PaintContext &context : m_input)
        compositor.PaintLayer(context);

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
