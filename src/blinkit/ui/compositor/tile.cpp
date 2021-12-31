// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: tile.cpp
// Description: Tile Helpers
//      Author: Ziming Li
//     Created: 2021-12-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./tile.h"

namespace BlinKit {

size_t Tile::CalculateCount(size_t length)
{
    size_t ret = length / Size;
    if (0 != (length % Size))
        ++ret;
    return ret;
}

IntSize Tile::GetAlignedSize(const IntSize &size)
{
    size_t columnCount = CalculateCount(size.width());
    size_t rowCount = CalculateCount(size.height());
    return IntSize(columnCount * Size, rowCount * Size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TileGrid::Range::Unite(const Range &other)
{
    minX = std::min(minX, other.minX);
    minY = std::min(minY, other.minY);
    maxX = std::max(maxX, other.maxX);
    maxY = std::max(maxY, other.maxY);
}

TileGrid::Range TileGrid::GetRangeForRect(const IntRect &rect)
{
    Range ret;
    ret.minX = rect.x() / Tile::Size;
    ret.minY = rect.y() / Tile::Size;
    ret.maxX = Tile::CalculateCount(rect.maxX());
    ret.maxY = Tile::CalculateCount(rect.maxY());
    return ret;
}

IntRect TileGrid::GetRectForRange(const Range &range)
{
    IntRect ret;
    if (range.minX > 0)
        ret.setX(range.minX * Tile::Size);
    if (range.minY > 0)
        ret.setY(range.minY * Tile::Size);
    ret.setSize(IntSize((range.maxX + 1) * Tile::Size, (range.maxY + 1) * Tile::Size));
    return ret;
}

} // namespace BlinKit
