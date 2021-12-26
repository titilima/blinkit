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

template <typename T>
class TileGrid
{
public:
    TileGrid(const IntSize &viewportSize)
        : m_columnCount(Tile::CalculateCount(viewportSize.width()))
        , m_rowCount(Tile::CalculateCount(viewportSize.height()))
    {
        ASSERT(!viewportSize.isEmpty());
        m_cells.resize(m_columnCount * m_rowCount);
    }

    T& CellAt(size_t x, size_t y)
    {
        ASSERT(x < m_columnCount);
        ASSERT(y < m_rowCount);
        return m_cells.at(y * m_columnCount + x);
    }

    using Callback = std::function<void(size_t, size_t, T &)>;
    void ProcessTilesForRect(const IntRect &rect, const Callback &callback)
    {
        const size_t minX = rect.x() / Tile::Size;
        const size_t minY = rect.y() / Tile::Size;
        const size_t maxX = Tile::CalculateCount(rect.maxX());
        const size_t maxY = Tile::CalculateCount(rect.maxY());
        for (size_t y = minY; y < maxY; ++y)
        {
            for (size_t x = minX; x < maxX; ++x)
                callback(x, y, CellAt(x, y));
        }
    }
private:
    const size_t m_columnCount;
    const size_t m_rowCount;
    std::vector<T> m_cells;
};

} // namespace BlinKit

#endif // BLINKIT_TILE_H
