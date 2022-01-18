#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: single_axis_scrolling_snapshot.h
// Description: SingleAxisScrollingSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-13
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SINGLE_AXIS_SCROLLING_SNAPSHOT_H
#define BLINKIT_SINGLE_AXIS_SCROLLING_SNAPSHOT_H

#include <tuple>
#include "blinkit/ui/compositor/snapshots/layer_snapshot.h"

namespace BlinKit {

class Tile;

class SingleAxisScrollingSnapshot : public LayerSnapshot
{
public:
    ~SingleAxisScrollingSnapshot(void) override;

    void UpdateTiles(const IntSize &layerBounds, const IntSize &viewportSize);
protected:
    SingleAxisScrollingSnapshot(void);

    struct Metrics {
        IntSize tileSize, offset;
        int visibleSize;
        int scrollingSize;
    };
    virtual Metrics GetMetrics(const IntSize &layerBounds, const IntSize &viewportSize) const = 0;
private:
    struct DirtyEntry {
        Tile *tile;
        IntRect dirtyRect; // of tile
    };
    std::vector<DirtyEntry> Translate(const IntRect &dirtyRect) const;

    void Update(const IntPoint &position, const IntRect &dirtyRect, const UpdateCallback &callback) final;
    void BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect) final;

    int m_scrollPosition = 0;
    std::vector<std::unique_ptr<Tile>> m_tiles;
};

} // namespace BlinKit

#endif // BLINKIT_SINGLE_AXIS_SCROLLING_SNAPSHOT_H
