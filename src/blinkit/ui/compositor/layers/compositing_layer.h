#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositing_layer.h
// Description: CompositingLayer Class
//      Author: Ziming Li
//     Created: 2021-12-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_COMPOSITING_LAYER_H
#define BLINKIT_COMPOSITING_LAYER_H

#include "blinkit/blink/renderer/platform/geometry/FloatPoint.h"
#include "blinkit/ui/compositor/tile.h"

class SkBitmap;
class SkCanvas;

namespace BlinKit {

class DisplayItemList;

class CompositingLayer
{
public:
    CompositingLayer(void);
    ~CompositingLayer(void);

    bool InTree(void) const { return nullptr != m_parent; }

    void DetachChildren(void);
    void DetachFromParent(void);
    void InsertChild(size_t index, CompositingLayer *child);

    void SetDrawsContent(bool drawsContent) {
        m_drawsContent = drawsContent;
    }
    void SetBounds(const IntSize &bounds) {
        m_bounds = bounds;
    }
    void SetPosition(const FloatPoint &position) {
        m_position = position;
    }

    void Update(const DisplayItemList &displayItemList);
    void SyncToCompositor(std::vector<CompositingLayer *> &compositedLayers);
    void BlendTo(SkCanvas *canvas, const TileGrid::Range &dirtyTiles);

#ifndef NDEBUG
    void DebugPrint(void) const {
        DebugPrint(0);
    }
#endif
private:
    void RemoveChild(CompositingLayer *child);

    const SkBitmap& EnsureBitmap(void);

#ifndef NDEBUG
    void DebugPrint(int depth) const;
#endif

    CompositingLayer *m_parent = nullptr;
    using Children = std::vector<CompositingLayer *>;
    Children m_children;

    FloatPoint m_position;
    IntSize m_bounds;
    bool m_drawsContent = true;

    std::unique_ptr<SkBitmap> m_bitmap;
};

} // namespace BlinKit

#endif // BLINKIT_COMPOSITING_LAYER_H
