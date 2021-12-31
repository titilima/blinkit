#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositor.h
// Description: Compositor Class
//      Author: Ziming Li
//     Created: 2021-12-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_COMPOSITOR_H
#define BLINKIT_COMPOSITOR_H

#include "blinkit/ui/compositor/tile.h"
#include "third_party/zed/include/zed/threading/task_queue.hpp"

class SkCanvas;

namespace BlinKit {

class CompositingLayer;
class CompositorTask;
class DisplayItemList;
class TreeCompositor;

class Compositor final : private zed::thread<Compositor>
{
public:
    Compositor(void);
    ~Compositor(void);

    void PostTask(CompositorTask *task) { m_queue.add(task); }
    void PostTasks(std::vector<CompositorTask *> &tasks);

    using Callback = std::function<void(Compositor &)>;
    void PostCallback(Callback &&callback);

    void DestroyTree(int treeId);
    void NewTree(int treeId);
    void PerformComposition(int treeId, SkCanvas *canvas, const TileGrid::Range &dirtyTiles);
    void SyncTree(int treeId, const std::function<void(TreeCompositor &)> &callback);
    void SetRootLayer(int treeId, int layerId);

    void DestroyLayer(int layerId);
    void NewLayer(int layerId);
    void RemoveChildLayers(int layerId);
    void RemoveLayer(int layerId);
    void SyncLayer(int layerId, const std::function<void(CompositingLayer &)> &callback);
    void SyncChildLayer(int parentLayerId, size_t index, int childLayerId);
    void UpdateLayer(int layerId, const DisplayItemList &displayItemList);
private:
    CompositingLayer* LookupLayer(int layerId) const;
    TreeCompositor* LookupTree(int treeId) const;

    void TaskLoop(void);

    class CallbackTask;
    bool m_running = true;
    zed::task_queue<CompositorTask> m_queue;

    std::unordered_map<int, std::unique_ptr<CompositingLayer>> m_layers;
    std::unordered_map<int, std::unique_ptr<TreeCompositor>> m_trees;
};

} // namespace BlinKit

#endif // BLINKIT_COMPOSITOR_H
