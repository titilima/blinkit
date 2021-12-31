#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer_tasks.h
// Description: Layer Task Classes
//      Author: Ziming Li
//     Created: 2021-12-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_LAYER_TASKS_H
#define BLINKIT_LAYER_TASKS_H

#include "blinkit/ui/compositor/compositor_task.h"
#include "third_party/zed/include/zed/memory.hpp"

namespace BlinKit {

class CompositingLayer;
class Layer;

class DestroyLayerTask final : public CompositorTask
{
public:
    DestroyLayerTask(Layer *layer);
private:
    void Run(Compositor &compositor) override;

    const int m_id;
};

class NewLayerTask final : public CompositorTask
{
public:
    NewLayerTask(Layer *layer);
private:
    void Run(Compositor &compositor) override;

    const int m_id;
};

class RemoveChildLayersTask final : public CompositorTask
{
public:
    RemoveChildLayersTask(Layer *layer);
private:
    void Run(Compositor &compositor) override;

    const int m_id;
};

class RemoveLayerTask final : public CompositorTask
{
public:
    RemoveLayerTask(Layer *layer);
private:
    void Run(Compositor &compositor) override;

    const int m_id;
};

class SyncChildLayerTask final : public CompositorTask
{
public:
    SyncChildLayerTask(Layer *parent, size_t index, Layer *child);
private:
    void Run(Compositor &compositor) override;

    const int m_parentId;
    const int m_childId;
    const size_t m_index;
};

class SyncLayerTask final : public CompositorTask
{
public:
    SyncLayerTask(Layer *layer, std::function<void(CompositingLayer &)> &&callback);
private:
    void Run(Compositor &compositor) override;

    const int m_id;
    const std::function<void(CompositingLayer &)> m_callback;
};

} // namespace BlinKit

#endif // BLINKIT_LAYER_TASKS_H
