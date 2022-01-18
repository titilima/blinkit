#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: simple_snapshot.h
// Description: SimpleSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-12
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SIMPLE_SNAPSHOT_H
#define BLINKIT_SIMPLE_SNAPSHOT_H

#include "blinkit/ui/compositor/snapshots/layer_snapshot.h"
#include "blinkit/ui/compositor/snapshots/tile.h"

namespace BlinKit {

class SimpleSnapshot final : public LayerSnapshot
{
public:
    SimpleSnapshot(const IntSize &layerBounds);
private:
    bool TryToReuse(Type assumedType, const IntSize &layerBounds, const IntSize &viewportSize) override;
    void Update(const IntPoint &position, const IntRect &dirtyRect, const UpdateCallback &callback) override;
    void BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect) override;

    Tile m_tile;
};

} // namespace BlinKit

#endif // BLINKIT_SIMPLE_SNAPSHOT_H
