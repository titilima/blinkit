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

    UpdateTiles(layerBounds, viewportSize);
    return true;
}

} // namespace BlinKit
