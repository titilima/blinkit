// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: single_axis_scrolling_snapshot.cpp
// Description: SingleAxisScrollingSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-13
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./single_axis_scrolling_snapshot.h"

#include <queue>
#include "blinkit/ui/compositor/snapshots/tile.h"

namespace BlinKit {

class SingleAxisScrollingSnapshot::TileFinder
{
public:
    TileFinder(const IntSize &offset) : offset(offset) {}

    bool operator()(const TileInfo &info) const {
        return info.offset == offset;
    }
private:
    const IntSize &offset;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SingleAxisScrollingSnapshot::Updater
{
    STACK_ALLOCATED();
public:
    Updater(const IntRect &dirtyRect, const UpdateCallback &callback) : m_dirtyRect(dirtyRect), m_callback(callback) {}
    ~Updater(void);

    void AddIfDirty(Tile *tile);
private:
    const IntRect &m_dirtyRect;
    const UpdateCallback &m_callback;

    struct DirtyEntry {
        Tile *tile;
        IntRect dirtyRect; // of tile
    };
    std::vector<DirtyEntry> m_dirtyParts;
    IntRect m_combinedDirtyRect;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SingleAxisScrollingSnapshot::SingleAxisScrollingSnapshot(void) = default;
SingleAxisScrollingSnapshot::~SingleAxisScrollingSnapshot(void) = default;

void SingleAxisScrollingSnapshot::BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect, const SkPaint *paint)
{
    for (const auto &tile : m_tiles)
    {
        const IntRect &tileRect = tile->Rect();
        if (!tileRect.intersects(dirtyRect))
            continue;

        tile->BlendToCanvas(canvas, intersection(tileRect, dirtyRect), paint);
    }
}

int SingleAxisScrollingSnapshot::CalculateTileCount(int visibleSize, int scrollingSize)
{
    int ret = visibleSize / Tile::Size;
    if (0 != (visibleSize % Tile::Size))
        ++ret;
    if (Tile::Size * ret < scrollingSize)
        ++ret;
    return ret;
}

int SingleAxisScrollingSnapshot::CalculateTileStartOffset(int layerStartPosition)
{
    ASSERT(layerStartPosition <= 0);
    int tileIndex = (-layerStartPosition) / Tile::Size;
    return tileIndex * Tile::Size;
}

int SingleAxisScrollingSnapshot::CalculateTileStartPosition(int layerStartPosition)
{
    ASSERT(layerStartPosition <= 0);
    return layerStartPosition % Tile::Size;
}

void SingleAxisScrollingSnapshot::Initialize(const IntSize &layerBounds, const IntSize &viewportSize)
{
    m_layerBounds = layerBounds;

    std::vector<TileInfo> visibleTiles = CalculateVisibleTiles(IntPoint(), viewportSize);

    const size_t size = visibleTiles.size();
    m_tiles.resize(size);
    for (size_t i = 0; i < size; ++i)
    {
        const TileInfo &info = visibleTiles.at(i);
        m_tiles[i] = std::make_unique<Tile>(info.rect, info.offset);
    }
}

void SingleAxisScrollingSnapshot::LayoutTiles(const IntPoint &layerPosition, const IntSize &viewportSize)
{
    std::vector<TileInfo> visibleTiles = CalculateVisibleTiles(layerPosition, viewportSize);
    ASSERT(visibleTiles.size() == m_tiles.size());

    const TileInfo &firstInfo = visibleTiles.front();
    const Tile     &firstTile = *(m_tiles.front());

    if (firstInfo.rect == firstTile.Rect())
        return;

    const size_t tileCount = visibleTiles.size();
    if (firstInfo.offset == firstTile.Offset())
    {
        for (size_t i = 0; i < tileCount; ++i)
        {
            const TileInfo &info = visibleTiles.at(i);
            ASSERT(info.offset == m_tiles.at(i)->Offset());
            m_tiles.at(i)->UpdatePosition(info.rect.location());
        }
        return;
    }

    // BKTODO: Optimize the logic below.
    std::unordered_map<size_t, size_t> reusePool;
    std::queue<std::unique_ptr<Tile>> resetPool;
    for (size_t i = 0; i < tileCount; ++i)
    {
        std::unique_ptr<Tile> &tile = m_tiles.at(i);

        auto it = std::find_if(visibleTiles.begin(), visibleTiles.end(), TileFinder(tile->Offset()));
        if (visibleTiles.end() != it)
        {
            size_t newIndex = it - visibleTiles.begin();
            reusePool.emplace(newIndex, i);
        }
        else
        {
            resetPool.emplace(std::move(tile));
        }
    }

    std::vector<std::unique_ptr<Tile>> newLayout(tileCount);
    for (size_t i = 0; i < tileCount; ++i)
    {
        const TileInfo &info = visibleTiles.at(i);

        auto it = reusePool.find(i);
        if (reusePool.end() == it)
        {
            ASSERT(!resetPool.empty());

            std::unique_ptr<Tile> &tile = resetPool.front();
            tile->Reset(info.rect, info.offset);
            newLayout[i] = std::move(tile);

            resetPool.pop();
        }
        else
        {
            std::unique_ptr<Tile> &tile = m_tiles.at(it->second);
            tile->UpdatePosition(info.rect.location());
            newLayout[i] = std::move(tile);
        }
    }
    m_tiles.swap(newLayout);
}

void SingleAxisScrollingSnapshot::ReuseTiles(int visibleSize, int scrollingSize, const IntSize &tileOffset)
{
    size_t tileCount = CalculateTileCount(visibleSize, scrollingSize);

    size_t oldCount = m_tiles.size();
    if (tileCount == oldCount)
        return;

    m_tiles.resize(tileCount);
    if (tileCount < oldCount)
        return;

    const Tile &lastTile = *(m_tiles.at(oldCount - 1));
    IntRect rect   = lastTile.Rect();
    IntSize offset = lastTile.Offset();
    for (size_t i = oldCount; i < tileCount; ++i)
    {
        rect.move(tileOffset);
        offset += tileOffset;
        m_tiles[i] = std::make_unique<Tile>(rect, offset);
    }
}

void SingleAxisScrollingSnapshot::Update(const IntSize &viewportSize, const LayerContext &context, const UpdateCallback &callback)
{
    LayoutTiles(context.positionOfViewport, viewportSize);

    Updater updater(context.dirtyRectOfLayer, callback);
    for (const std::unique_ptr<Tile> &tile : m_tiles)
        updater.AddIfDirty(tile.get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SingleAxisScrollingSnapshot::Updater::~Updater(void)
{
    if (1 == m_dirtyParts.size())
    {
        const DirtyEntry &e = m_dirtyParts.front();
        e.tile->Update(e.dirtyRect, m_callback);
        return;
    }

    Tile tile(m_combinedDirtyRect);
#ifndef NDEBUG
    tile.DisableDebugInfo();
#endif
    tile.Update(m_combinedDirtyRect, m_callback);

    const SkBitmap &bitmap = tile.Lock();
    for (const DirtyEntry &e : m_dirtyParts)
    {
        IntPoint from(e.dirtyRect.location());
        from.move(-m_combinedDirtyRect.x(), -m_combinedDirtyRect.y());

        IntPoint to(e.dirtyRect.location());
        to.move(-e.tile->Rect().x(), -e.tile->Rect().y());

        e.tile->Update(bitmap, from, to, e.dirtyRect.size());
    }
}

void SingleAxisScrollingSnapshot::Updater::AddIfDirty(Tile *tile)
{
    IntRect tileRect(IntPoint(tile->Offset()), tile->Rect().size());

    DirtyEntry e;
    if (tile->Painted())
    {
        e.dirtyRect = intersection(tileRect, m_dirtyRect);
        if (e.dirtyRect.isEmpty())
            return;
    }
    else
    {
        e.dirtyRect = tileRect;
    }

    e.tile = tile;
    m_dirtyParts.emplace_back(e);
    m_combinedDirtyRect.unite(e.dirtyRect);
}

} // namespace BlinKit
