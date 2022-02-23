#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: tile.h
// Description: Tile Class
//      Author: Ziming Li
//     Created: 2022-01-17
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_TILE_H
#define BLINKIT_TILE_H

#include "blinkit/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/skia/include/core/SkBitmap.h"
#ifndef NDEBUG
#   include "third_party/zed/include/zed/file/path.hpp"
#endif

class SkCanvas;

namespace BlinKit {

class Tile
{
public:
    Tile(const IntRect &rect, const IntSize &offset = IntSize());
    Tile(const IntSize &size) : Tile(IntRect(IntPoint::zero(), size)) {}

    static constexpr int Size = 256;

    void Reset(const IntRect &rect, const IntSize &offset = IntSize());
    void Update(const IntRect &dirtyRect, const std::function<void(SkCanvas &)> &callback);
    void Update(const SkBitmap &bitmap, const IntPoint &from, const IntPoint &to, const IntSize &size);
    void BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect, const SkPaint *paint);

    bool Painted(void) const { return m_painted; }

    const IntRect& Rect(void) const { return m_rect; }
    void UpdatePosition(const IntPoint &position) {
        m_rect.setLocation(position);
    }

    const IntSize& Offset(void) const { return m_offset; }

    const SkBitmap& Bitmap(void) const { return m_bitmap; }
    const SkBitmap& Lock(void);

#ifndef NDEBUG
    void EnableDebugInfo(void) { m_drawDebugInfo = true; }
    void DisableDebugInfo(void) { m_drawDebugInfo = false; }

    void SaveAsPNG(zed::path::psz_t filePath);
#endif
private:
    void ResetBitmap(void);
#ifndef NDEBUG
    void DrawDebugInfo(SkCanvas &canvas) const;
#endif

    bool m_painted = false;
    IntRect m_rect;   // of viewport
    IntSize m_offset; // of owner
    SkBitmap m_bitmap;

#ifndef NDEBUG
    bool m_drawDebugInfo;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_TILE_H
