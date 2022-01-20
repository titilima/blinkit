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

    Tile tile(dirtyRect);
    tile.Update(dirtyRect, callback);
    for (const DirtyEntry &e : dirtyTiles)
    {
        IntPoint from(e.dirtyRect.location());
        from.move(-dirtyRect.x(), -dirtyRect.y());

        IntPoint to(e.dirtyRect.location());
        to.move(-e.tile->Rect().x(), -e.tile->Rect().y());

        e.tile->Update(tile.Bitmap(), from, to, e.dirtyRect.size());
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
