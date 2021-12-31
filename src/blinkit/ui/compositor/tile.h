#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: tile.h
// Description: Tile Helpers
//      Author: Ziming Li
//     Created: 2021-12-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_TILE_H
#define BLINKIT_TILE_H

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"

namespace BlinKit {

class Tile
{
    STATIC_ONLY(Tile);
public:
    constexpr static size_t Size = 256;

    static size_t CalculateCount(size_t length);
    static IntSize GetAlignedSize(const IntSize &size);
};

class TileGrid
{
    STATIC_ONLY(TileGrid);
public:
    struct Range {
        size_t minX, maxX;
        size_t minY, maxY;

        void Unite(const Range &other);
    };

    static Range GetRangeForRect(const IntRect &rect);
    static IntRect GetRectForRange(const Range &range);
};

} // namespace BlinKit

#endif // BLINKIT_TILE_H
