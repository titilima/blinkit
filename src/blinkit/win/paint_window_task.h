#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: paint_window_task.h
// Description: PaintWindowTask Class
//      Author: Ziming Li
//     Created: 2021-12-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_PAINT_WINDOW_TASK_H
#define BLINKIT_PAINT_WINDOW_TASK_H

#include "blinkit/ui/compositor/paint_ui_task.h"

namespace BlinKit {

class PaintWindowTask final : public PaintUITask
{
public:
    PaintWindowTask(HWND hWnd, HDC hMemoryDC, WebLayerTreeView *tree, zed::mutex &lock, SkCanvas *canvas);
private:
    void run(void) override;

    HWND m_hWnd;
    HDC m_hMemoryDC;
};

} // namespace BlinKit

#endif // BLINKIT_PAINT_WINDOW_TASK_H
