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
#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/ui/compositor/raster/raster_result.h"

class SkCanvas;

namespace zed {
class mutex;
}

namespace BlinKit {

class Compositor;

class PaintUITask : public WebTaskRunner::Task
{
public:
    void PerformComposition(Compositor &compositor, const RasterResult &rasterResult, const IntRect &dirtyRect);
protected:
    PaintUITask(zed::mutex &lock, SkCanvas *canvas);

    void PerformPaint(const std::function<void(IntRect &)> &callback);
private:
    zed::mutex &m_lock;
    SkCanvas *m_canvas;
    IntRect m_dirtyRect;
};

} // namespace BlinKit

#endif // BLINKIT_PAINT_UI_TASK_H
