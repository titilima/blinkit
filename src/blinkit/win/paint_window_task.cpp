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

#include <windowsx.h>
#include "blinkit/win/bk_bitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"

namespace BlinKit {

PaintWindowTask::PaintWindowTask(HWND hWnd, HDC hMemoryDC, zed::mutex &lock, std::unique_ptr<SkCanvas> &canvas)
    : PaintUITask(lock, canvas), m_hWnd(hWnd), m_hMemoryDC(hMemoryDC)
{
}

std::unique_ptr<SkCanvas> PaintWindowTask::CreateCanvas(const IntSize &size)
{
    BkBitmap bitmap;
    HBITMAP hBitmap = bitmap.InstallDIBSection(size.width(), size.height(), m_hMemoryDC);
    SelectBitmap(m_hMemoryDC, hBitmap);
    return std::make_unique<SkCanvas>(bitmap);
}

void PaintWindowTask::run(void)
{
    const auto callback = [this](const IntRect &dirtyRect) {
        HDC hdc = GetDC(m_hWnd);
        BitBlt(hdc, dirtyRect.x(), dirtyRect.y(), dirtyRect.width(), dirtyRect.height(),
            m_hMemoryDC, dirtyRect.x(), dirtyRect.y(), SRCCOPY);
        ReleaseDC(m_hWnd, hdc);
    };
    PerformPaint(callback);
}

} // namespace BlinKit
