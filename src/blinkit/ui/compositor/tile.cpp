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
    size_t columnCount = Tile::CalculateCount(size.width());
    size_t rowCount = Tile::CalculateCount(size.height());
    return IntSize(columnCount * Size, rowCount * Size);
}

} // namespace BlinKit
