// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: paint_ui_task.cpp
// Description: PaintUITask Class
//      Author: Ziming Li
//     Created: 2021-12-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./paint_ui_task.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/ui/compositor/compositor.h"
#include "third_party/zed/include/zed/mutex.hpp"

namespace BlinKit {

PaintUITask::PaintUITask(zed::mutex &lock, SkCanvas *canvas) : m_lock(lock), m_canvas(canvas)
{
}

void PaintUITask::PerformComposition(Compositor &compositor, const RasterResult &rasterResult, const IntRect &dirtyRect)
{
    m_dirtyRect = dirtyRect;

    auto _ = m_lock.guard();
    compositor.PerformComposition(*m_canvas, rasterResult, m_dirtyRect);
}

void PaintUITask::PerformPaint(const std::function<void(IntRect &)> &callback)
{
    ASSERT(isMainThread());
    auto _ = m_lock.guard();
    callback(m_dirtyRect);
}

} // namespace BlinKit