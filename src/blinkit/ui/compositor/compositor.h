#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositor.h
// Description: Compositor Class
//      Author: Ziming Li
//     Created: 2021-12-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_COMPOSITOR_H
#define BLINKIT_COMPOSITOR_H

#include "blinkit/ui/compositor/raster/raster_result.h"
#include "third_party/zed/include/zed/threading/task_queue.hpp"

class SkCanvas;

namespace BlinKit {

class CompositorTask;
class LayerSnapshot;
struct LayerContext;

class Compositor final : private zed::thread<Compositor>
{
public:
    Compositor(void);
    ~Compositor(void);

    void PostTask(CompositorTask *task) { m_queue.add(task); }
    void PostTasks(std::vector<CompositorTask *> &tasks);

    using Callback = std::function<void(Compositor &)>;
    void PostCallback(Callback &&callback);

    void ReleaseSnapshot(int layerId);
    void UpdateSnapshot(const LayerContext &input, const IntSize &viewportSize);
    void PerformComposition(SkCanvas &canvas, const RasterResult &rasterResult, const IntRect &dirtyRect);
private:
    LayerSnapshot& RequireSnapshot(int layerId, const IntSize &layerBounds, const IntSize &viewportSize);

    void TaskLoop(void);

    class CallbackTask;
    bool m_running = true;
    zed::task_queue<CompositorTask> m_queue;

    std::unordered_map<int, std::unique_ptr<LayerSnapshot>> m_snapshots;
};

} // namespace BlinKit

#endif // BLINKIT_COMPOSITOR_H
