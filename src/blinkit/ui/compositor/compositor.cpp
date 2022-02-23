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

#include "blinkit/ui/animation/animation_frame.h"
#include "blinkit/ui/animation/animation_proxy.h"
#include "blinkit/ui/compositor/snapshots/layer_snapshot.h"
#include "blinkit/ui/compositor/tasks/compositor_task.h"
#ifndef NDEBUG
#   include "blinkit/blink/renderer/platform/graphics/skia/skia_utils.h"
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

void Compositor::Attach(AnimationProxy *proxy)
{
    auto _ = m_lockForProxies.guard();
    m_livingProxies.emplace(proxy);
}

SkCanvas Compositor::BeginPaint(const IntSize &viewportSize)
{
    bool createNewFrame = true;
    if (m_backingStoreFrame)
    {
        IntSize size = m_backingStoreFrame->GetSize();
        if (size.width() >= viewportSize.width() && size.height() >= viewportSize.height())
            createNewFrame = false;
    }

    if (createNewFrame)
        m_backingStoreFrame = std::make_unique<AnimationFrame>(viewportSize);
    return m_backingStoreFrame->BeginPaint();
}

void Compositor::Detach(AnimationProxy *proxy)
{
    auto _ = m_lockForProxies.guard();
    m_livingProxies.erase(proxy);
}

void Compositor::PerformComposition(
    AnimationProxy *proxy,
    const IntSize &viewportSize,
    const RasterResult &rasterResult,
    const IntRect &dirtyRect)
{
    const IntPoint offset = dirtyRect.location();
    const IntSize size = dirtyRect.size();

    SkCanvas canvas = BeginPaint(size);

    if (IntPoint::zero() != offset)
        canvas.setMatrix(SkMatrix::MakeTrans(-offset.x(), -offset.y()));

    SkPaint paint;
    paint.setAntiAlias(false);
    paint.setFilterQuality(kLow_SkFilterQuality);

    for (const LayerData &data : rasterResult)
    {
        auto it = m_snapshots.find(data.id);
        ASSERT(m_snapshots.end() != it);

        paint.setXfermodeMode(data.opaque ? SkXfermode::kSrc_Mode : SkXfermode::kSrcATop_Mode);

        it->second->BlendToCanvas(canvas, dirtyRect, &paint);
    }

    auto _ = m_lockForProxies.guard();
    if (!zed::key_exists(m_livingProxies, proxy))
        return;

#ifndef NDEBUG
    if (!m_backingStoreFrameFolder.empty())
    {
        zed::path::string_t fileName = GenerateBackingStoreFrameFileName();
        SaveBitmapAsPNG(m_backingStoreFrame->GetBitmap(), fileName.c_str());
    }
#endif
    if (size == viewportSize)
    {
        ASSERT(IntPoint::zero() == offset); // SHOULD BE full invalidation!
        proxy->SwapFrame(m_backingStoreFrame, size);
    }
    else
    {
        paint.setXfermodeMode(SkXfermode::kSrc_Mode);
        proxy->FlushFrame(m_backingStoreFrame->GetBitmap(), offset, size, paint);
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

void Compositor::UpdateSnapshot(const LayerContext &input, const IntSize &viewportSize)
{
    LayerSnapshot &snapshot = RequireSnapshot(input.id, input.bounds, viewportSize);

    const auto callback = [&input](SkCanvas &canvas) {
#if 0 // TODO: Check the logic later.
        canvas.concat(m_transform);
#endif
        input.displayItems->Playback(canvas);
    };
    snapshot.Update(viewportSize, input, callback);
}

#ifndef NDEBUG
void Compositor::DumpSnapshots(zed::path::psz_t path)
{
    for (const auto &[_, snapshot] : m_snapshots)
        snapshot->DumpTo(path);
}

zed::path::string_t Compositor::GenerateBackingStoreFrameFileName(void) const
{
    zed::path::string_t ret(m_backingStoreFrameFolder);

#ifdef _Z_OS_WINDOWS
    WCHAR buf[MAX_PATH];
    wsprintfW(buf, L"%08X.png", m_backingStoreFrame.get());
    ret.append(buf);
#else
    ASSERT(false); // BKTODO:
#endif

    return ret;
}

bool Compositor::IsProxyAttached(AnimationProxy *proxy)
{
    auto _ = m_lockForProxies.guard();
    return zed::key_exists(m_livingProxies, proxy);
}
#endif

} // namespace BlinKit
