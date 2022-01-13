#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_task.h
// Description: RasterTask Class
//      Author: Ziming Li
//     Created: 2021-12-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_RASTER_TASK_H
#define BLINKIT_RASTER_TASK_H

#include "blinkit/ui/compositor/tasks/compositor_task.h"
#include "blinkit/ui/compositor/raster/raster_input.h"
#include "blinkit/ui/compositor/raster/raster_result.h"

namespace blink {
class WebTaskRunner;
}

namespace BlinKit {

class Layer;
struct PaintContext;
class PaintUITask;
class RasterContext;

class RasterTask final : public CompositorTask
{
public:
    RasterTask(const IntSize &viewportSize);
    ~RasterTask(void) override;

    const IntSize& ViewportSize(void) const { return m_viewportSize; }

    bool HasNothingToDo(void) const { return m_input.empty(); }
    const RasterResult& Result(void) const { return m_result; }

    PaintContext& RequireDirtyContext(const Layer &layer);
    void Rasterize(const Layer &layer, const IntRect &visibleRect);
    void UpdateDirtyRect(const IntRect &dirtyRect) {
        m_dirtyRect.unite(dirtyRect);
    }

    void SavePaintTask(std::unique_ptr<PaintUITask> &paintTask);
private:
    void Run(Compositor &compositor) override;

    const IntSize m_viewportSize;
    RasterInput m_input;
    RasterResult m_result;
    IntRect m_dirtyRect;

    std::shared_ptr<WebTaskRunner> m_taskRunner;
    std::unique_ptr<PaintUITask> m_paintTask;
};

} // namespace BlinKit

#endif // BLINKIT_RASTER_TASK_H
