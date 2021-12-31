// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer_tasks.cpp
// Description: Layer Task Classes
//      Author: Ziming Li
//     Created: 2021-12-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./layer_tasks.h"

#include "blinkit/ui/compositor/blink/layer.h"
#include "blinkit/ui/compositor/compositor.h"

namespace BlinKit {

DestroyLayerTask::DestroyLayerTask(Layer *layer) : m_id(layer->id())
{
}

void DestroyLayerTask::Run(Compositor &compositor)
{
    compositor.DestroyLayer(m_id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NewLayerTask::NewLayerTask(Layer *layer) : m_id(layer->id())
{
}

void NewLayerTask::Run(Compositor &compositor)
{
    compositor.NewLayer(m_id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RemoveChildLayersTask::RemoveChildLayersTask(Layer *layer) : m_id(layer->id())
{
}

void RemoveChildLayersTask::Run(Compositor &compositor)
{
    compositor.RemoveChildLayers(m_id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RemoveLayerTask::RemoveLayerTask(Layer *layer) : m_id(layer->id())
{
}

void RemoveLayerTask::Run(Compositor &compositor)
{
    compositor.RemoveLayer(m_id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SyncChildLayerTask::SyncChildLayerTask(Layer *parent, size_t index, Layer *child)
    : m_parentId(parent->id()), m_index(index), m_childId(child->id())
{
}

void SyncChildLayerTask::Run(Compositor &compositor)
{
    compositor.SyncChildLayer(m_parentId, m_index, m_childId);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SyncLayerTask::SyncLayerTask(Layer *layer, std::function<void(CompositingLayer &)> &&callback)
    : m_id(layer->id()), m_callback(std::move(callback))
{
}

void SyncLayerTask::Run(Compositor &compositor)
{
    compositor.SyncLayer(m_id, m_callback);
}

} // namespace BlinKit
