// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: tile.cpp
// Description: Tile Class
//      Author: Ziming Li
//     Created: 2022-01-17
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./tile.h"

#include "third_party/skia/include/core/SkCanvas.h"
#ifndef NDEBUG
#   include "third_party/skia/include/core/SkTypeface.h"
#endif

#ifndef NDEBUG
static bool g_debugInfoEnabled = true;
#endif

namespace BlinKit {

Tile::Tile(const IntRect &rect, const IntSize &offset)
    : m_rect(rect), m_offset(offset)
#ifndef NDEBUG
    , m_drawDebugInfo(g_debugInfoEnabled)
#endif
{
    ResetBitmap();
}

void Tile::BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect)
{
    SkRect srcRect(dirtyRect);
    srcRect.offset(-m_rect.x(), -m_rect.y());
    canvas.drawBitmapRect(m_bitmap, srcRect, dirtyRect, nullptr);
}

const SkBitmap& Tile::Lock(void)
{
    m_bitmap.setImmutable();
    return m_bitmap;
}

void Tile::Reset(const IntRect &rect, const IntSize &offset)
{
    m_painted = false;
    m_rect    = rect;
    m_offset  = offset;

    const SkImageInfo &info = m_bitmap.info();
    if (info.width() < m_rect.width() || info.height() < m_rect.height())
        ResetBitmap();
}

void Tile::ResetBitmap(void)
{
    m_bitmap.allocN32Pixels(m_rect.width(), m_rect.height(), kPremul_SkAlphaType);
}

void Tile::Update(const IntRect &dirtyRect, const std::function<void(SkCanvas &)> &callback)
{
    if (dirtyRect.isEmpty() && m_painted)
        return;

    SkCanvas canvas(m_bitmap);
#ifndef NDEBUG
    canvas.save();
#endif

    if (!m_offset.isZero())
    {
        SkMatrix transform = SkMatrix::MakeTrans(-m_offset.width(), -m_offset.height());
        canvas.setMatrix(transform);
        if (m_painted)
            canvas.clipRect(dirtyRect, SkRegion::kIntersect_Op, true);
        else
            m_painted = true;
    }
    else if (m_painted)
    {
        ASSERT(m_rect.contains(dirtyRect));

        IntRect dirtyRectOfLayer(dirtyRect);
        if (0 != m_rect.x() || 0 != m_rect.y())
            dirtyRectOfLayer.move(-m_rect.x(), -m_rect.y());
        canvas.clipRect(dirtyRectOfLayer, SkRegion::kIntersect_Op, true);
    }
    else
    {
        m_painted = true;
    }

    callback(canvas);

#ifndef NDEBUG
    canvas.restore();
    DrawDebugInfo(canvas);
#endif
}

void Tile::Update(const SkBitmap &bitmap, const IntPoint &from, const IntPoint &to, const IntSize &size)
{
    SkCanvas canvas(m_bitmap);

    SkIRect srcRect = SkIRect::MakeXYWH(from.x(), from.y(), size.width(), size.height());
    SkRect dstRect = SkRect::MakeXYWH(to.x(), to.y(), size.width(), size.height());

    if (!m_painted)
        m_painted = true;
#ifndef NDEBUG
    else
        canvas.clipRect(dstRect, SkRegion::kIntersect_Op, true);
#endif
    canvas.drawBitmapRect(bitmap, srcRect, dstRect, nullptr);

#ifndef NDEBUG
    DrawDebugInfo(canvas);
#endif
}

#ifndef NDEBUG
static SkColor GetColorForDebugInfo(void)
{
    static const SkColor s_colors[] = {
        SK_ColorBLACK, SK_ColorRED, SK_ColorGREEN, SK_ColorBLUE, SK_ColorYELLOW, SK_ColorCYAN, SK_ColorMAGENTA
    };

    static int s_colorIndex = 0;
    s_colorIndex = (s_colorIndex + 1) % std::size(s_colors);
    return s_colors[s_colorIndex];
}

void Tile::DrawDebugInfo(SkCanvas &canvas) const
{
    if (!m_drawDebugInfo)
        return;

    SkPaint paint;
    paint.setColor(GetColorForDebugInfo());
    paint.setStrokeWidth(4);
    paint.setTextSize(13);
    paint.setTextEncoding(SkPaint::kUTF8_TextEncoding);

    static SkTypeface *typeface = SkTypeface::RefDefault(SkTypeface::kNormal);
    paint.setTypeface(typeface);

    paint.setStrokeWidth(1);
    canvas.drawLine(0, 0, m_rect.width(), m_rect.height(), paint);
    canvas.drawLine(m_rect.width(), 0, 0, m_rect.height(), paint);

    char text[128];
    int len = sprintf(text, "(%d, %d) %dx%d", m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height());
    canvas.drawText(text, len, 5, 15, paint);
}
#endif

} // namespace BlinKit
