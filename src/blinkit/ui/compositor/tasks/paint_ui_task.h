#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: paint_ui_task.h
// Description: PaintUITask Class
//      Author: Ziming Li
//     Created: 2021-12-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_PAINT_UI_TASK_H
#define BLINKIT_PAINT_UI_TASK_H

#include "blinkit/blink/public/platform/WebTaskRunner.h"
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"
#include "blinkit/ui/compositor/raster/raster_result.h"
#include "third_party/skia/include/core/SkColor.h"

class SkCanvas;

namespace zed {
class mutex;
}

namespace BlinKit {

class Compositor;

class PaintUITask : public WebTaskRunner::Task
{
public:
    void SetBackgroundColor(SkColor color) {
        m_backgroundColor = color;
    }
    void SetViewportSize(const IntSize &size) {
        m_viewportSize = size;
    }
    void SetFullPaint(void) {
        m_fullPaint = true;
    }

    void PerformComposition(Compositor &compositor, const RasterResult &rasterResult, const IntRect &dirtyRect);
protected:
    PaintUITask(zed::mutex &lock, std::unique_ptr<SkCanvas> &canvas);

    void PerformPaint(const std::function<void(const IntRect &)> &callback);
private:
    void EnsureCanvas(void);
    virtual std::unique_ptr<SkCanvas> CreateCanvas(const IntSize &size) = 0;

    zed::mutex &m_lock;
    std::unique_ptr<SkCanvas> &m_canvas;
    SkColor m_backgroundColor = SK_ColorWHITE;
    IntSize m_viewportSize;
    bool m_fullPaint = false;
    IntRect m_dirtyRect;
};

} // namespace BlinKit

#endif // BLINKIT_PAINT_UI_TASK_H
