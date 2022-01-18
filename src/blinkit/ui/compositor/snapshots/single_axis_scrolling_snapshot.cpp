// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: single_axis_scrolling_snapshot.cpp
// Description: SingleAxisScrollingSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-13
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./single_axis_scrolling_snapshot.h"

#include "blinkit/ui/compositor/snapshots/tile.h"

namespace BlinKit {

SingleAxisScrollingSnapshot::SingleAxisScrollingSnapshot(void) = default;
SingleAxisScrollingSnapshot::~SingleAxisScrollingSnapshot(void) = default;

void SingleAxisScrollingSnapshot::BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect)
{
    for (const auto &tile : m_tiles)
    {
        const IntRect &tileRect = tile->Rect();
        if (!tileRect.intersects(dirtyRect))
            continue;

        tile->BlendToCanvas(canvas, intersection(tileRect, dirtyRect));
    }
}

std::vector<SingleAxisScrollingSnapshot::DirtyEntry> SingleAxisScrollingSnapshot::Translate(const IntRect &dirtyRect) const
{
    std::vector<DirtyEntry> ret;
    for (const auto &tile : m_tiles)
    {
        const IntRect &tileRect = tile->Rect();
        if (!tileRect.intersects(dirtyRect))
            continue;

        DirtyEntry e;
        e.tile = tile.get();
        e.dirtyRect = intersection(tileRect, dirtyRect);
        ret.emplace_back(e);
    }
    return ret;
}

void SingleAxisScrollingSnapshot::Update(const IntPoint &position, const IntRect &dirtyRect, const UpdateCallback &callback)
{
    std::vector<DirtyEntry> dirtyTiles = Translate(dirtyRect);
    if (1 == dirtyTiles.size())
    {
        const DirtyEntry &e = dirtyTiles.front();
        e.tile->Update(e.dirtyRect, callback);
        return;
    }

    const IntPoint offset = dirtyRect.location();
    const IntSize dirtySize = dirtyRect.size();

    SkBitmap bitmap;
    bitmap.allocN32Pixels(dirtyRect.width(), dirtyRect.height(), kPremul_SkAlphaType);

    SkCanvas canvas(bitmap);
    if (0 != offset.x() && 0 != offset.y())
    {
        SkMatrix transform = SkMatrix::MakeTrans(-offset.x(), -offset.y());
        canvas.setMatrix(transform);
    }
    callback(canvas);

    for (const DirtyEntry &e : dirtyTiles)
    {
        IntPoint from(e.dirtyRect.location());
        from.moveBy(-offset);

        IntPoint to(e.dirtyRect.location());
        to.moveBy(-e.tile->Rect().location());

        e.tile->Update(bitmap, from, to, e.dirtyRect.size());
    }
}

void SingleAxisScrollingSnapshot::UpdateTiles(const IntSize &layerBounds, const IntSize &viewportSize)
{
    const Metrics metrics = GetMetrics(layerBounds, viewportSize);

    int tileCount = metrics.visibleSize / Tile::Size;
    if (0 != (metrics.visibleSize % Tile::Size))
        ++tileCount;

    IntPoint position;
    m_tiles.resize(tileCount);
    for (std::unique_ptr<Tile> &tile : m_tiles)
    {
        IntRect rect(position, metrics.tileSize);
        if (tile)
            tile->Reset(rect);
        else
            tile = std::make_unique<Tile>(rect);
        position += metrics.offset;
    }
}

} // namespace BlinKit
