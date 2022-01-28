// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scrollbar_painters.cpp
// Description: Scrollbar Painter Classes
//      Author: Ziming Li
//     Created: 2022-01-06
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./scrollbar_painters.h"

#include "blinkit/blink/public/platform/web_scrollbar.h"
#include "blinkit/blink/public/platform/web_scrollbar_theme_geometry.h"
#include "blinkit/ui/compositor/snapshots/tile.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkPictureRecorder.h"

namespace BlinKit {

ThemedScrollbarPainter::ThemedScrollbarPainter(
    std::unique_ptr<WebScrollbar> &&scrollbar,
    WebScrollbarThemePainter painter,
    std::unique_ptr<WebScrollbarThemeGeometry> &geometry)
    : m_scrollbar(std::move(scrollbar))
    , m_painter(painter)
    , m_geometry(std::move(geometry))
{
}

ThemedScrollbarPainter::~ThemedScrollbarPainter(void) = default;

SkPicture* ThemedScrollbarPainter::Paint(const IntSize &bounds)
{
    ASSERT(m_scrollbar->size() == bounds);

    Tile tile(bounds);
#ifndef NDEBUG
    tile.DisableDebugInfo();
#endif
    tile.UpdatePosition(m_scrollbar->location());

    const IntRect &rect = tile.Rect();
    const auto callback = [this, &rect](SkCanvas &canvas) {
        if (rect.x() != 0 || rect.y() != 0)
            canvas.setMatrix(SkMatrix::MakeTrans(-rect.x(), -rect.y()));
        m_painter.paintScrollbarBackground(&canvas, rect);
        PaintButtons(&canvas);
        PaintTrack(&canvas);
    };
    tile.Update(rect, callback);

    SkPictureRecorder recorder;
    SkCanvas *canvas = recorder.beginRecording(SkRect::MakeIWH(bounds.width(), bounds.height()));
    canvas->drawBitmap(tile.Lock(), 0, 0);
    return recorder.endRecordingAsPicture();
}

void ThemedScrollbarPainter::PaintButtons(SkCanvas *canvas)
{
    if (!m_geometry->hasButtons(m_scrollbar.get()))
        return;

    IntRect rect = m_geometry->backButtonStartRect(m_scrollbar.get());
    if (!rect.isEmpty())
        m_painter.paintBackButtonStart(canvas, rect);

    rect = m_geometry->backButtonEndRect(m_scrollbar.get());
    if (!rect.isEmpty())
        m_painter.paintBackButtonEnd(canvas, rect);

    rect = m_geometry->forwardButtonStartRect(m_scrollbar.get());
    if (!rect.isEmpty())
        m_painter.paintForwardButtonStart(canvas, rect);

    rect = m_geometry->forwardButtonEndRect(m_scrollbar.get());
    if (!rect.isEmpty())
        m_painter.paintForwardButtonEnd(canvas, rect);
}

void ThemedScrollbarPainter::PaintTrack(SkCanvas *canvas)
{
    IntRect rect = m_geometry->trackRect(m_scrollbar.get());
    if (rect.isEmpty())
        return;

    m_painter.paintTrackBackground(canvas, rect);

    if (m_geometry->hasThumb(m_scrollbar.get()))
    {
        m_painter.paintForwardTrackPart(canvas, rect);
        m_painter.paintBackTrackPart(canvas, rect);
        m_painter.paintThumb(canvas, m_geometry->thumbRect(m_scrollbar.get()));
    }

    m_painter.paintTickmarks(canvas, rect);
}

} // namespace BlinKit
