// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: vertical_scrolling_snapshot.cpp
// Description: VerticalScrollingSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-13
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./vertical_scrolling_snapshot.h"

#include "blinkit/ui/compositor/snapshots/tile.h"

namespace BlinKit {

std::vector<SingleAxisScrollingSnapshot::TileInfo> VerticalScrollingSnapshot::CalculateVisibleTiles(
    const IntPoint &layerPosition,
    const IntSize &viewportSize) const
{
    int tileCount = CalculateTileCount(viewportSize.height(), LayerHeight());

    std::vector<TileInfo> ret;
    ret.reserve(tileCount);

    TileInfo tile;
    tile.rect.setSize(IntSize(LayerWidth(), Tile::Size));
    tile.rect.setY(CalculateTileStartPosition(layerPosition.y()));
    tile.offset.setHeight(CalculateTileStartOffset(layerPosition.y()));
    for (int i = 0; i < tileCount; ++i)
    {
        ret.emplace_back(tile);
        tile.rect.move(0, Tile::Size);
        tile.offset.expand(0, Tile::Size);
    }

    return ret;
}

SingleAxisScrollingSnapshot::Metrics VerticalScrollingSnapshot::GetMetrics(
    const IntSize &layerBounds, const IntSize &viewportSize) const
{
    Metrics ret;
    ret.tileSize = IntSize(layerBounds.width(), Tile::Size);
    ret.offset = IntSize(0, Tile::Size);
    ret.visibleSize = viewportSize.height();
    ret.scrollingSize = layerBounds.height();
    return ret;
}

bool VerticalScrollingSnapshot::TryToReuse(Type assumedType, const IntSize &layerBounds, const IntSize &viewportSize)
{
    if (Type::VerticalScrolling != assumedType)
        return false;

    ReuseTiles(viewportSize.height(), layerBounds.height(), IntSize(0, Tile::Size));
    return true;
}

} // namespace BlinKit
