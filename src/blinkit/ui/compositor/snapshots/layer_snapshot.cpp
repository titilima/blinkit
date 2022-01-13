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

namespace BlinKit {

LayerSnapshot::LayerSnapshot(const IntSize &layerBounds) : m_layerBounds(layerBounds)
{
}

LayerSnapshot::Type LayerSnapshot::AssumeSnapshotType(const IntSize &layerBounds, const IntSize &viewportSize)
{
    const int maxWidth = viewportSize.width() + BlockUnit;
    const int maxHeight = viewportSize.height() + BlockUnit;

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
            ASSERT(false); // BKTODO:
            return nullptr;
        case Type::HorizontalScrolling:
            ASSERT(false); // BKTODO:
            return nullptr;
        default:
            ASSERT(Type::Gridded == type);
    }
    ASSERT(false); // BKTODO:
    return nullptr;
}

} // namespace BlinKit
