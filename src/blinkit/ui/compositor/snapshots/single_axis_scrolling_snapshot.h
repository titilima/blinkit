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

#include "blinkit/ui/compositor/snapshots/layer_snapshot.h"

namespace BlinKit {

class Tile;

class SingleAxisScrollingSnapshot : public LayerSnapshot
{
public:
    ~SingleAxisScrollingSnapshot(void) override;

    void Initialize(const IntSize &layerBounds, const IntSize &viewportSize);
protected:
    SingleAxisScrollingSnapshot(void);

    int LayerWidth(void) const { return m_layerBounds.width(); }
    int LayerHeight(void) const { return m_layerBounds.height(); }
    void ReuseTiles(int visibleSize, int scrollingSize, const IntSize &tileOffset);

    static int CalculateTileCount(int visibleSize, int scrollingSize);
    static int CalculateTileStartPosition(int layerStartPosition);
    static int CalculateTileStartOffset(int layerStartPosition);

    struct Metrics {
        IntSize tileSize, offset;
        int visibleSize;
        int scrollingSize;
    };
    virtual Metrics GetMetrics(const IntSize &layerBounds, const IntSize &viewportSize) const = 0;

    struct TileInfo {
        IntRect rect;   // of viewport
        IntSize offset; // of owner
    };
    virtual std::vector<TileInfo> CalculateVisibleTiles(const IntPoint &layerPosition, const IntSize &viewportSize) const = 0;
private:
    void LayoutTiles(const IntPoint &layerPosition, const IntSize &viewportSize);

    void Update(const IntSize &viewportSize, const LayerContext &context, const UpdateCallback &callback) final;
    void BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect, const SkPaint *paint) final;

    IntSize m_layerBounds;
    std::vector<std::unique_ptr<Tile>> m_tiles;

    class TileFinder;
    class Updater;
};

} // namespace BlinKit

#endif // BLINKIT_SINGLE_AXIS_SCROLLING_SNAPSHOT_H
