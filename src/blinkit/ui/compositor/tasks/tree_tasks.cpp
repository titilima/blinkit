// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: tree_tasks.cpp
// Description: Tree Task Classes
//      Author: Ziming Li
//     Created: 2021-12-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./tree_tasks.h"

#include "blinkit/ui/compositor/blink/layer.h"
#include "blinkit/ui/compositor/compositor.h"
#include "blinkit/ui/compositor/layer_tree_host.h"

namespace BlinKit {

DestroyTreeTask::DestroyTreeTask(LayerTreeHost *tree) : m_id(tree->layerTreeId())
{
}

void DestroyTreeTask::Run(Compositor &compositor)
{
    compositor.DestroyTree(m_id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NewTreeTask::NewTreeTask(LayerTreeHost *tree) : m_id(tree->layerTreeId())
{
}

void NewTreeTask::Run(Compositor &compositor)
{
    compositor.NewTree(m_id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SetRootLayerTask::SetRootLayerTask(LayerTreeHost *tree, Layer *layer)
    : m_treeId(tree->layerTreeId()), m_layerId(layer->id())
{

}

void SetRootLayerTask::Run(Compositor &compositor)
{
    compositor.SetRootLayer(m_treeId, m_layerId);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SyncTreeTask::SyncTreeTask(LayerTreeHost *tree, std::function<void(TreeCompositor &)> &&callback)
    : m_id(tree->layerTreeId()), m_callback(std::move(callback))
{
}

void SyncTreeTask::Run(Compositor &compositor)
{
    compositor.SyncTree(m_id, m_callback);
}

} // namespace BlinKit
