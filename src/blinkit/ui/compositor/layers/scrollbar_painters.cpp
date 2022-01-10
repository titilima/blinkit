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
#include "third_party/skia/include/core/SkCanvas.h"

namespace BlinKit {

class ThemedScrollbarPainter::CanvasWrapper final : public SkCanvas
{
public:
    CanvasWrapper(const IntPoint &offset, SkCanvas &canvas) : m_offset(offset), m_realCanvas(canvas) {}
private:
    void onDrawBitmap(const SkBitmap &bitmap, SkScalar dx, SkScalar dy, const SkPaint *paint) override
    {
        ASSERT(!bitmap.drawsNothing());
        m_realCanvas.drawBitmap(bitmap, dx - m_offset.x(), dy - m_offset.y(), paint);
    }
#ifndef NDEBUG
    void onDrawDRRect(const SkRRect &, const SkRRect &, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawText(const void *, size_t, SkScalar, SkScalar, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawPosText(const void *, size_t, const SkPoint[], const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawPosTextH(const void *, size_t, const SkScalar[], SkScalar, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawTextOnPath(const void *, size_t, const SkPath &, const SkMatrix *, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawTextBlob(const SkTextBlob *, SkScalar, SkScalar, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawPatch(const SkPoint[12], const SkColor[4], const SkPoint[4], SkXfermode *, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawDrawable(SkDrawable *, const SkMatrix *) override {
        NOTREACHED();
    }
    void onDrawPaint(const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawRect(const SkRect &, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawOval(const SkRect &, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawRRect(const SkRRect &, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawPoints(PointMode, size_t, const SkPoint[], const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawVertices(VertexMode, int, const SkPoint[], const SkPoint[], const SkColor[], SkXfermode *, const uint16_t[], int, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawAtlas(const SkImage *, const SkRSXform[], const SkRect[], const SkColor[], int, SkXfermode::Mode, const SkRect *, const SkPaint *) override {
        NOTREACHED();
    }
    void onDrawPath(const SkPath &, const SkPaint &) override {
        NOTREACHED();
    }
    void onDrawImage(const SkImage *, SkScalar, SkScalar, const SkPaint *) override {
        NOTREACHED();
    }
    void onDrawImageRect(const SkImage *, const SkRect *, const SkRect &, const SkPaint *, SrcRectConstraint) override {
        NOTREACHED();
    }
    void onDrawImageNine(const SkImage *, const SkIRect &, const SkRect &, const SkPaint *) override {
        NOTREACHED();
    }
    void onDrawBitmapRect(const SkBitmap &, const SkRect *, const SkRect &, const SkPaint *, SrcRectConstraint) override {
        NOTREACHED();
    }
    void onDrawBitmapNine(const SkBitmap &, const SkIRect &, const SkRect &, const SkPaint *) override {
        NOTREACHED();
    }
    void onClipRect(const SkRect &, SkRegion::Op, ClipEdgeStyle) override {
        NOTREACHED();
    }
    void onClipRRect(const SkRRect &, SkRegion::Op, ClipEdgeStyle) override {
        NOTREACHED();
    }
    void onClipPath(const SkPath &, SkRegion::Op, ClipEdgeStyle) override {
        NOTREACHED();
    }
    void onClipRegion(const SkRegion &, SkRegion::Op) override {
        NOTREACHED();
    }
    void onDiscard(void) override {
        NOTREACHED();
    }
    void onDrawPicture(const SkPicture *, const SkMatrix *, const SkPaint *) override {
        NOTREACHED();
    }
#endif // NDEBUG

    const IntPoint m_offset;
    SkCanvas &m_realCanvas;
};

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

void ThemedScrollbarPainter::Paint(SkCanvas *canvas, const IntSize &bounds)
{
    IntRect rect;

    rect.setSize(bounds);
    m_painter.paintScrollbarBackground(canvas, rect);

    CanvasWrapper wrapper(m_scrollbar->location(), *canvas);
    if (m_geometry->hasButtons(m_scrollbar.get()))
        PaintButtons(&wrapper);

    rect = m_geometry->trackRect(m_scrollbar.get());
    if (!rect.isEmpty())
    {
        m_painter.paintTrackBackground(&wrapper, rect);

        if (m_geometry->hasThumb(m_scrollbar.get()))
        {
            m_painter.paintForwardTrackPart(&wrapper, rect);
            m_painter.paintBackTrackPart(&wrapper, rect);

            IntRect thumbRect = m_geometry->thumbRect(m_scrollbar.get());
            m_painter.paintThumb(&wrapper, thumbRect);
        }

        m_painter.paintTickmarks(&wrapper, rect);
    }
}

void ThemedScrollbarPainter::PaintButtons(SkCanvas *canvas)
{
    IntRect rect;

    rect = m_geometry->backButtonStartRect(m_scrollbar.get());
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

} // namespace BlinKit
