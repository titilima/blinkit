// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositor.cpp
// Description: Compositor Class
//      Author: Ziming Li
//     Created: 2021-12-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./compositor.h"

#include "blinkit/ui/compositor/raster/raster_input.h"
#include "blinkit/ui/compositor/snapshots/layer_snapshot.h"
#include "blinkit/ui/compositor/tasks/compositor_task.h"
#ifndef NDEBUG
#   include "blinkit/ui/compositor/blink/layer.h"
#   include "third_party/skia/include/core/SkTypeface.h"
#endif

namespace BlinKit {

class Compositor::CallbackTask final : public CompositorTask
{
public:
    CallbackTask(std::function<void(Compositor &)> &&callback) : m_callback(std::move(callback)) {}
private:
    void Run(Compositor &compositor) override {
        m_callback(compositor);
    }

    const std::function<void(Compositor &)> m_callback;
};

Compositor::Compositor(void) : thread(this, &Compositor::TaskLoop)
{
}

Compositor::~Compositor(void)
{
    PostCallback([this](Compositor &) {
        m_running = false;
    });
    join();

    ASSERT(m_snapshots.empty());
    ASSERT(g_allLayers.empty());
}

void Compositor::BlendSnapshotToCanvas(
    SkCanvas &canvas,
    int layerId,
    const IntPoint &from, const IntPoint &to,
    const IntSize &size) const
{
    auto it = m_snapshots.find(layerId);
    ASSERT(m_snapshots.end() != it);
    it->second->BlendToCanvas(canvas, from, to, size);
}

void Compositor::PerformComposition(SkCanvas &canvas, const RasterResult &rasterResult, const IntRect &dirtyRect)
{
    for (const LayerData &data : rasterResult)
    {
        IntRect rect(data.rect);
        rect.intersect(dirtyRect);

        IntPoint from(rect.x() - data.rect.x(), rect.y() - data.rect.y());
        BlendSnapshotToCanvas(canvas, data.id, from, rect.location(), rect.size());
    }
}

void Compositor::PostCallback(Callback &&callback)
{
    PostTask(new CallbackTask(std::move(callback)));
}

void Compositor::PostTasks(std::vector<CompositorTask *> &tasks)
{
    m_queue.add([&tasks](auto &queue) {
        for (CompositorTask *task : tasks)
            queue.emplace(task);
    });
}

void Compositor::ReleaseSnapshot(int layerId)
{
    m_snapshots.erase(layerId);
}

LayerSnapshot& Compositor::RequireSnapshot(int layerId, const IntSize &layerBounds, const IntSize &viewportSize)
{
    const LayerSnapshot::Type type = LayerSnapshot::AssumeSnapshotType(layerBounds, viewportSize);

    auto it = m_snapshots.find(layerId);
    if (m_snapshots.end() != it)
    {
        if (!it->second->TryToReuse(type, layerBounds, viewportSize))
            it->second.reset(LayerSnapshot::Create(type, layerBounds, viewportSize));
        return *(it->second);
    }

    LayerSnapshot *ret = LayerSnapshot::Create(type, layerBounds, viewportSize);
    m_snapshots.emplace(layerId, ret);
    return *ret;
}

void Compositor::TaskLoop(void)
{
#ifndef NDEBUG
    zed::current_thread::set_name("Compositor Thread");
#endif

    zed::task_queue<CompositorTask>::queue_t tasks;
    for (;;)
    {
        m_queue.take(tasks);
        while (!tasks.empty())
        {
            std::unique_ptr<CompositorTask> t = std::move(tasks.front());
            t->Run(*this);

            if (!m_running)
                return;

            tasks.pop();
        }
    }
}

void Compositor::UpdateSnapshot(const IntSize &viewportSize, const PaintContext &input)
{
    LayerSnapshot &snapshot = RequireSnapshot(input.layerId, input.bounds, viewportSize);

    const auto callback = [&input](SkCanvas &canvas) {
#if 0 // TODO: Check the logic later.
        canvas.concat(m_transform);
#endif
        canvas.clipRect(input.dirtyRect, SkRegion::kIntersect_Op, true);
        input.displayItems->Playback(canvas);

        // DrawDebugInfo(canvas, input.layerId, input.layerBounds);
    };
    snapshot.Update(callback);
}

#ifndef NDEBUG
void Compositor::DrawDebugInfo(SkCanvas &canvas, int layerId, const IntSize &layerBounds)
{
    SkPaint paint;

    static const SkColor colors[] = {
        SK_ColorBLACK, SK_ColorRED, SK_ColorGREEN, SK_ColorBLUE, SK_ColorYELLOW, SK_ColorCYAN, SK_ColorMAGENTA
    };
    paint.setColor(colors[rand() % std::size(colors)]);

    paint.setStrokeWidth(4);
    paint.setTextSize(13);
    paint.setTextEncoding(SkPaint::kUTF8_TextEncoding);

    static SkTypeface *typeface = SkTypeface::RefDefault(SkTypeface::kNormal);
    paint.setTypeface(typeface);

    paint.setStrokeWidth(1);
    canvas.drawLine(0, 0, layerBounds.width(), layerBounds.height(), paint);
    canvas.drawLine(layerBounds.width(), 0, 0, layerBounds.height(), paint);

    char text[128];
    int len = sprintf(text, "0x%08X %dx%d", layerId, layerBounds.width(), layerBounds.height());
    canvas.drawText(text, len, 5, 15, paint);
}
#endif

} // namespace BlinKit
