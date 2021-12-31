// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: paint_window_task.cpp
// Description: PaintWindowTask Class
//      Author: Ziming Li
//     Created: 2021-12-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./paint_window_task.h"

namespace BlinKit {

PaintWindowTask::PaintWindowTask(HWND hWnd, HDC hMemoryDC, WebLayerTreeView *tree, zed::mutex &lock, SkCanvas *canvas)
    : PaintUITask(tree, lock, canvas), m_hWnd(hWnd), m_hMemoryDC(hMemoryDC)
{
}

void PaintWindowTask::run(void)
{
    const auto callback = [this](IntRect &dirtyRect) {
        HDC hdc = GetDC(m_hWnd);
        BitBlt(hdc, dirtyRect.x(), dirtyRect.y(), dirtyRect.width(), dirtyRect.height(),
            m_hMemoryDC, dirtyRect.x(), dirtyRect.y(), SRCCOPY);
        ReleaseDC(m_hWnd, hdc);
    };
    PerformPaint(callback);
}

} // namespace BlinKit
