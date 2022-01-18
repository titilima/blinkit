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

namespace BlinKit {

Tile::Tile(const IntRect &rect) : m_rect(rect)
{
    ResetBitmap();
}

void Tile::BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect)
{
    SkRect srcRect(dirtyRect);
    srcRect.offset(-m_rect.x(), -m_rect.y());
    canvas.drawBitmapRect(m_bitmap, srcRect, dirtyRect, nullptr);
}

void Tile::Reset(const IntRect &rect)
{
    m_rect = rect;
    
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
    SkCanvas canvas(m_bitmap);
    canvas.setMatrix(SkMatrix::MakeTrans(-m_rect.x(), -m_rect.y()));
    canvas.clipRect(dirtyRect, SkRegion::kIntersect_Op, true);
    callback(canvas);
}

void Tile::Update(const SkBitmap &bitmap, const IntPoint &from, const IntPoint &to, const IntSize &size)
{
    SkCanvas canvas(m_bitmap);
    SkIRect srcRect = SkIRect::MakeXYWH(from.x(), from.y(), size.width(), size.height());
    SkRect dstRect = SkRect::MakeXYWH(to.x(), to.y(), size.width(), size.height());
    canvas.drawBitmapRect(bitmap, srcRect, dstRect, nullptr);
}

} // namespace BlinKit
