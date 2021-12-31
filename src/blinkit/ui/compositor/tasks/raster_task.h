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

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/ui/compositor/compositor_task.h"

namespace blink {
class WebTaskRunner;
}

namespace BlinKit {

class Layer;
class PaintUITask;

class RasterTask final : public CompositorTask
{
public:
    RasterTask(const IntSize &viewportSize);
    ~RasterTask(void) override;

    bool HasNothingToDo(void) const { return m_entries.empty(); }

    void AddDirtyLayer(Layer *layer);

    void SavePaintTask(std::unique_ptr<PaintUITask> &paintTask);
private:
    void RecordPainting(Layer *layer);
    bool Invalidate(Layer *layer);

    void Run(Compositor &compositor) override;

    const IntSize m_viewportSize;

    class Entry;
    std::vector<std::unique_ptr<Entry>> m_entries;
    IntRect m_dirtyRect;

    std::shared_ptr<WebTaskRunner> m_taskRunner;
    std::unique_ptr<PaintUITask> m_paintTask;
};

} // namespace BlinKit

#endif // BLINKIT_RASTER_TASK_H
