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

#include <unordered_set>
#include "blinkit/ui/compositor/raster/raster_result.h"
#include "third_party/zed/include/zed/threading/task_queue.hpp"
#ifndef NDEBUG
#   include "third_party/zed/include/zed/file/path.hpp"
#endif

class SkCanvas;

namespace BlinKit {

class AnimationFrame;
class AnimationProxy;
class CompositorTask;
class LayerSnapshot;
struct LayerContext;

class Compositor final : private zed::thread<Compositor>
{
public:
    Compositor(void);
    ~Compositor(void);

    void Attach(AnimationProxy *proxy);
    void Detach(AnimationProxy *proxy);
#ifndef NDEBUG
    bool IsProxyAttached(AnimationProxy *proxy);
#endif

    void PostTask(CompositorTask *task) { m_queue.add(task); }
    void PostTasks(std::vector<CompositorTask *> &tasks);

    using Callback = std::function<void(Compositor &)>;
    void PostCallback(Callback &&callback);

    void ReleaseSnapshot(int layerId);
    void UpdateSnapshot(const LayerContext &input, const IntSize &viewportSize);
    void PerformComposition(AnimationProxy *proxy, const IntSize &viewportSize, const RasterResult &rasterResult,
        const IntRect &dirtyRect);

#ifndef NDEBUG
    void DumpSnapshots(zed::path::psz_t path);
    void DumpBackingStoreFrame(zed::path::psz_t path) {
        m_backingStoreFrameFolder.assign(path);
    }
#endif
private:
    SkCanvas BeginPaint(const IntSize &viewportSize);
    LayerSnapshot& RequireSnapshot(int layerId, const IntSize &layerBounds, const IntSize &viewportSize);

    void TaskLoop(void);

    class CallbackTask;
    bool m_running = true;
    zed::task_queue<CompositorTask> m_queue;

    std::unique_ptr<AnimationFrame> m_backingStoreFrame;
    std::unordered_map<int, std::unique_ptr<LayerSnapshot>> m_snapshots;

    zed::mutex m_lockForProxies;
    std::unordered_set<AnimationProxy *> m_livingProxies;

#ifndef NDEBUG
    zed::path::string_t GenerateBackingStoreFrameFileName(void) const;
    zed::path::string_t m_backingStoreFrameFolder;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_COMPOSITOR_H
