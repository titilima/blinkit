// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositor.cpp
// Description: Compositor Class
//      Author: Ziming Li
//     Created: 2021-12-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./compositor.h"

#include "blinkit/ui/compositor/compositor_task.h"
#include "blinkit/ui/compositor/layers/compositing_layer.h"
#include "blinkit/ui/compositor/tree_compositor.h"

namespace BlinKit {

class Compositor::CallbackTask final : public CompositorTask
{
public:
    CallbackTask(std::function<void(Compositor &)> &&callback) : m_callback(std::move(callback)) {}
private:
    void Run(Compositor &compositor) override {
        m_callback(compositor);
    }

    const std::function<void(Compositor &)> m_callback;
};

Compositor::Compositor(void) : thread(this, &Compositor::TaskLoop)
{
}

Compositor::~Compositor(void)
{
    PostCallback([this](Compositor &) {
        m_running = false;
    });
    join();

    ASSERT(m_layers.empty());
    ASSERT(m_trees.empty());
}

void Compositor::DestroyLayer(int layerId)
{
    m_layers.erase(layerId);
}

void Compositor::DestroyTree(int treeId)
{
    m_trees.erase(treeId);
}

CompositingLayer* Compositor::LookupLayer(int layerId) const
{
    ASSERT(0 != layerId);

    auto it = m_layers.find(layerId);
    ASSERT(m_layers.end() != it);
    return it->second.get();
}

TreeCompositor* Compositor::LookupTree(int treeId) const
{
    ASSERT(0 != treeId);

    auto it = m_trees.find(treeId);
    ASSERT(m_trees.end() != it);
    return it->second.get();
}

void Compositor::NewLayer(int layerId)
{
    m_layers.emplace(layerId, new CompositingLayer);
}

void Compositor::NewTree(int treeId)
{
    m_trees.emplace(treeId, new TreeCompositor);
}

void Compositor::PerformComposition(int treeId, SkCanvas *canvas, const IntRect &dirtyRect)
{
    LookupTree(treeId)->PerformComposition(canvas, dirtyRect);
}

void Compositor::PostCallback(Callback &&callback)
{
    PostTask(new CallbackTask(std::move(callback)));
}

void Compositor::PostTasks(std::vector<CompositorTask *> &tasks)
{
    m_queue.add([&tasks](auto &queue) {
        for (CompositorTask *task : tasks)
            queue.emplace(task);
    });
}

void Compositor::RemoveChildLayers(int layerId)
{
    LookupLayer(layerId)->DetachChildren();
}

void Compositor::RemoveLayer(int layerId)
{
    LookupLayer(layerId)->DetachFromParent();
}

void Compositor::SetRootLayer(int treeId, int layerId)
{
    LookupTree(treeId)->SetRootLayer(LookupLayer(layerId));
}

void Compositor::SyncChildLayer(int parentLayerId, size_t index, int childLayerId)
{
    CompositingLayer *parentLayer = LookupLayer(parentLayerId);
    CompositingLayer *childLayer = LookupLayer(childLayerId);
    parentLayer->InsertChild(index, childLayer);
}

void Compositor::SyncLayer(int layerId, const std::function<void(CompositingLayer &)> &callback)
{
    callback(*LookupLayer(layerId));
}

void Compositor::SyncTree(int treeId, const std::function<void(TreeCompositor &)> &callback)
{
    callback(*LookupTree(treeId));
}

void Compositor::TaskLoop(void)
{
#ifndef NDEBUG
    zed::current_thread::set_name("Compositor Thread");
#endif

    zed::task_queue<CompositorTask>::queue_t tasks;
    for (;;)
    {
        m_queue.take(tasks);
        while (!tasks.empty())
        {
            std::unique_ptr<CompositorTask> t = std::move(tasks.front());
            t->Run(*this);

            if (!m_running)
            {
                delete this;
                return;
            }

            tasks.pop();
        }
    }
}

void Compositor::UpdateLayer(int layerId, const DisplayItemList &displayItemList)
{
    LookupLayer(layerId)->Update(displayItemList);
}

} // namespace BlinKit
