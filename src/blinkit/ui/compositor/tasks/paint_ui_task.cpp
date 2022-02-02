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
#include "third_party/skia/include/core/SkCanvas.h"

namespace BlinKit {

PaintUITask::PaintUITask(zed::mutex &lock, std::unique_ptr<SkCanvas> &canvas) : m_lock(lock), m_canvas(canvas)
{
}

void PaintUITask::EnsureCanvas(void)
{
    if (m_canvas)
    {
        SkImageInfo imageInfo = m_canvas->imageInfo();
        if (m_viewportSize.width() <= imageInfo.width() && m_viewportSize.height() <= imageInfo.height())
            return;
    }

    m_canvas = CreateCanvas(m_viewportSize);
    m_canvas->drawColor(m_backgroundColor);
}

void PaintUITask::PerformComposition(Compositor &compositor, const RasterResult &rasterResult, const IntRect &dirtyRect)
{
    if (m_fullPaint)
        m_dirtyRect = IntRect(IntPoint(), m_viewportSize);
    else
        m_dirtyRect = dirtyRect;

    auto _ = m_lock.guard();
    EnsureCanvas();
    compositor.PerformComposition(*m_canvas, rasterResult, m_dirtyRect);
}

void PaintUITask::PerformPaint(const std::function<void(const IntRect &)> &callback)
{
    ASSERT(isMainThread());
    auto _ = m_lock.guard();
    callback(m_dirtyRect);
}

} // namespace BlinKit
