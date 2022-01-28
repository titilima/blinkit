// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer_snapshot.cpp
// Description: LayerSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-12
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./layer_snapshot.h"

#include "blinkit/ui/compositor/snapshots/simple_snapshot.h"
#include "blinkit/ui/compositor/snapshots/vertical_scrolling_snapshot.h"

namespace BlinKit {

LayerSnapshot::Type LayerSnapshot::AssumeSnapshotType(const IntSize &layerBounds, const IntSize &viewportSize)
{
    const int maxWidth = viewportSize.width() + Tile::Size;
    const int maxHeight = viewportSize.height() + Tile::Size;

    if (layerBounds.height() > maxHeight)
    {
        if (layerBounds.width() > maxWidth)
            return Type::Gridded;
        else
            return Type::VerticalScrolling;
    }

    if (layerBounds.width() > maxWidth)
        return Type::HorizontalScrolling;

    return Type::Simple;
}

LayerSnapshot* LayerSnapshot::Create(Type type, const IntSize &layerBounds, const IntSize &viewportSize)
{
    switch (type)
    {
        case Type::Simple:
            return new SimpleSnapshot(layerBounds);

        case Type::VerticalScrolling:
        {
            VerticalScrollingSnapshot *ret = new VerticalScrollingSnapshot;
            ret->ResetTiles(layerBounds, viewportSize);
            return ret;
        }
        case Type::HorizontalScrolling:
        {
            ASSERT(false); // BKTODO:
            return nullptr;
        }

        default:
            ASSERT(Type::Gridded == type);
    }
    ASSERT(false); // BKTODO:
    return nullptr;
}

} // namespace BlinKit
