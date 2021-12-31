#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: tree_tasks.h
// Description: Tree Task Classes
//      Author: Ziming Li
//     Created: 2021-12-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_TREE_TASKS_H
#define BLINKIT_TREE_TASKS_H

#include "blinkit/ui/compositor/compositor_task.h"

namespace BlinKit {

class Layer;
class LayerTreeHost;
class TreeCompositor;

class DestroyTreeTask final : public CompositorTask
{
public:
    DestroyTreeTask(LayerTreeHost *tree);
private:
    void Run(Compositor &compositor) override;

    const int m_id;
};

class NewTreeTask final : public CompositorTask
{
public:
    NewTreeTask(LayerTreeHost *tree);
private:
    void Run(Compositor &compositor) override;

    const int m_id;
};

class SetRootLayerTask final : public CompositorTask
{
public:
    SetRootLayerTask(LayerTreeHost *tree, Layer *layer);
private:
    void Run(Compositor &compositor) override;

    const int m_treeId;
    const int m_layerId;
};

class SyncTreeTask final : public CompositorTask
{
public:
    SyncTreeTask(LayerTreeHost *tree, std::function<void(TreeCompositor &)> &&callback);
private:
    void Run(Compositor &compositor) override;

    const int m_id;
    const std::function<void(TreeCompositor &)> m_callback;
};

} // namespace BlinKit

#endif // BLINKIT_TREE_TASKS_H
