#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: tree_compositor.h
// Description: TreeCompositor Class
//      Author: Ziming Li
//     Created: 2021-12-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_TREE_COMPOSITOR_H
#define BLINKIT_TREE_COMPOSITOR_H

#include "blinkit/ui/compositor/tile.h"

class SkCanvas;

namespace BlinKit {

class CompositingLayer;

class TreeCompositor
{
public:
    void SetNeedsCommit(void) {
        m_needsCommit = true;
    }
    void SetNeedsFullTreeSync(void) {
        m_needsFullTreeSync = true;
    }

    void SetRootLayer(CompositingLayer *layer);

    void PerformComposition(SkCanvas *canvas, const TileGrid::Range &dirtyTiles);
private:
    void SyncLayerTreeIfNecessary(void);

    bool m_needsCommit = false;
    bool m_needsFullTreeSync = false;

    CompositingLayer *m_rootLayer = nullptr;
    std::vector<CompositingLayer *> m_compositedLayers;
};

} // namespace BlinKit

#endif // BLINKIT_TREE_COMPOSITOR_H
