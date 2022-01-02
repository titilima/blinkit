// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: tree_compositor.cpp
// Description: TreeCompositor Class
//      Author: Ziming Li
//     Created: 2021-12-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./tree_compositor.h"

#include "blinkit/ui/compositor/layers/compositing_layer.h"

namespace BlinKit {

void TreeCompositor::PerformComposition(SkCanvas *canvas, const IntRect &dirtyRect)
{
    SyncLayerTreeIfNecessary();

    if (!m_needsCommit)
        return;

    for (CompositingLayer *layer : m_compositedLayers)
        layer->BlendTo(canvas, dirtyRect);
}

void TreeCompositor::SetRootLayer(CompositingLayer *layer)
{
    ASSERT(m_rootLayer != layer);
    m_rootLayer = layer;
}

void TreeCompositor::SyncLayerTreeIfNecessary(void)
{
    if (!m_needsFullTreeSync)
        return;

    m_compositedLayers.clear();
    if (nullptr != m_rootLayer)
        m_rootLayer->SyncToCompositor(m_compositedLayers);

    m_needsFullTreeSync = false;
}

} // namespace BlinKit
