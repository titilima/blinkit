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

class SkBitmap;
class SkCanvas;

namespace blink {
class IntRect;
}

namespace BlinKit {

class CompositorTask;
struct PaintContext;

class Compositor final : private zed::thread<Compositor>
{
public:
    Compositor(void);
    ~Compositor(void);

    void PostTask(CompositorTask *task) { m_queue.add(task); }
    void PostTasks(std::vector<CompositorTask *> &tasks);

    using Callback = std::function<void(Compositor &)>;
    void PostCallback(Callback &&callback);

    void ReleaseBitmap(int layerId);
    void PaintLayer(const PaintContext &input);
    void PerformComposition(SkCanvas &canvas, const RasterResult &rasterResult, const IntRect &dirtyRect);
private:
    const SkBitmap& RequireBitmap(int layerId, const IntSize &size);
    void BlendBitmapToCanvas(SkCanvas &canvas, int layerId, const IntPoint &from, const IntPoint &to,
        const IntSize &size) const;

    void TaskLoop(void);

#ifndef NDEBUG
    static void DrawDebugInfo(SkCanvas &canvas, int layerId, const IntSize &layerBounds);
#endif

    class CallbackTask;
    bool m_running = true;
    zed::task_queue<CompositorTask> m_queue;

    std::unordered_map<int, std::unique_ptr<SkBitmap>> m_layerBitmaps;
};

} // namespace BlinKit

#endif // BLINKIT_COMPOSITOR_H
