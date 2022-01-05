// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer_tree_host.cpp
// Description: LayerTreeHost Class
//      Author: Ziming Li
//     Created: 2021-12-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./layer_tree_host.h"

#include "blinkit/app/app_impl.h"
#include "blinkit/ui/compositor/blink/layer.h"
#include "blinkit/ui/compositor/compositor.h"
#include "blinkit/ui/compositor/raster/raster_context.h"
#include "blinkit/ui/compositor/tasks/raster_task.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

LayerTreeHost::LayerTreeHost(void) = default;
LayerTreeHost::~LayerTreeHost(void) = default;

void LayerTreeHost::clearRootLayer(void)
{
    if (nullptr == m_rootLayer)
        return;

    m_rootLayer->SetLayerTreeHost(nullptr);
    m_rootLayer = nullptr;
}

void LayerTreeHost::clearViewportLayers(void)
{
    registerViewportLayers(nullptr, nullptr, nullptr, nullptr);
}

void LayerTreeHost::heuristicsForGpuRasterizationUpdated(bool matchesHeuristics)
{
    // BKTODO: Check if necessary.
}

void LayerTreeHost::Paint(HDC hdc, const RECT *rc)
{
    // BKTODO:
}

void LayerTreeHost::PostTaskToCompositor(CompositorTask *task)
{
    AppImpl::Get().GetCompositor().PostTask(task);
}

void LayerTreeHost::registerForAnimations(WebLayer *layer)
{
    Layer *impl = static_cast<Layer *>(layer);
    // BKTODO: impl->RegisterForAnimations(layer_tree_host_->animation_registrar());
}

void LayerTreeHost::RegisterLayer(Layer *layer)
{
    ASSERT(!zed::key_exists(m_layers, layer));
    ASSERT(!m_inPaintLayerContents);
    m_layers.emplace(layer);
#if 0 // BKTODO:
    if (animation_host_)
        animation_host_->RegisterLayer(layer->id(), LayerTreeType::ACTIVE);
#endif
}

void LayerTreeHost::registerViewportLayers(
    const WebLayer *overscrollElasticityLayer,
    const WebLayer *pageScaleLayer,
    const WebLayer *innerViewportScrollLayer,
    const WebLayer *outerViewportScrollLayer)
{
#if 0 // BKTODO:
    DCHECK(!inner_viewport_scroll_layer ||
        inner_viewport_scroll_layer != outer_viewport_scroll_layer);
    overscroll_elasticity_layer_ = overscroll_elasticity_layer;
    page_scale_layer_ = page_scale_layer;
    inner_viewport_scroll_layer_ = inner_viewport_scroll_layer;
    outer_viewport_scroll_layer_ = outer_viewport_scroll_layer;
#endif
}

void LayerTreeHost::setDeviceScaleFactor(float scaleFactor)
{
    if (zed::almost_equals(m_deviceScaleFactor, scaleFactor))
        return;
    m_deviceScaleFactor = scaleFactor;

    // BKTODO: property_trees_.needs_rebuild = true;
    SetNeedsCommit();
}

void LayerTreeHost::SetNeedsCommit(void)
{
#if 0 // BKTODO:
    proxy_->SetNeedsCommit();
    NotifySwapPromiseMonitorsOfSetNeedsCommit();
#endif
}

void LayerTreeHost::SetNeedsFullTreeSync(void)
{
#if 0 // BKTODO:
    needs_full_tree_sync_ = true;
    needs_meta_info_recomputation_ = true;

    property_trees_.needs_rebuild = true;
#endif
    SetNeedsCommit();
}

void LayerTreeHost::SetNeedsUpdateLayers(void)
{
#if 0 // BKTODO:
    proxy_->SetNeedsUpdateLayers();
    NotifySwapPromiseMonitorsOfSetNeedsCommit();
#endif
}

void LayerTreeHost::setPageScaleFactorAndLimits(float pageScaleFactor, float minimum, float maximum)
{
    if (zed::almost_equals(m_pageScaleFactor, pageScaleFactor)
        && zed::almost_equals(m_minPageScaleFactor, minimum)
        && zed::almost_equals(m_maxPageScaleFactor, maximum))
    {
        return;
    }

    m_pageScaleFactor = pageScaleFactor;
    m_minPageScaleFactor = minimum;
    m_maxPageScaleFactor = maximum;
    SetPropertyTreesNeedRebuild();
    SetNeedsCommit();
}

void LayerTreeHost::SetPropertyTreesNeedRebuild(void)
{
    // BKTODO: property_trees_.needs_rebuild = true;
    SetNeedsUpdateLayers();
}

void LayerTreeHost::setRootLayer(const WebLayer &webLayer)
{
    Layer *layer = const_cast<Layer *>(static_cast<const Layer *>(&webLayer));

    if (m_rootLayer == layer)
        return;

    if (nullptr != m_rootLayer)
        m_rootLayer->SetLayerTreeHost(nullptr);

    m_rootLayer = layer;

    ASSERT(nullptr == m_rootLayer->Parent());
    m_rootLayer->SetLayerTreeHost(this);

    SetNeedsFullTreeSync();
}

void LayerTreeHost::SetRootLayerImpl(Layer *layer)
{
    if (m_rootLayer == layer)
        return;

    if (nullptr != m_rootLayer)
        m_rootLayer->SetLayerTreeHost(nullptr);
    m_rootLayer = layer;
    if (nullptr != m_rootLayer)
    {
        ASSERT(nullptr == m_rootLayer->Parent());
        m_rootLayer->SetLayerTreeHost(this);
    }

#if 0 // BKTODO:
    if (hud_layer_.get())
        hud_layer_->RemoveFromParent();

    // Reset gpu rasterization flag.
    // This flag is sticky until a new tree comes along.
    content_is_suitable_for_gpu_rasterization_ = true;
    gpu_rasterization_histogram_recorded_ = false;
#endif

    SetNeedsFullTreeSync();
}

void LayerTreeHost::setTopControlsHeight(float height, bool shrinkViewport)
{
    if (zed::almost_equals(m_topControlsHeight, height) && m_topControlsShrinkBlinkSize == shrinkViewport)
        return;

    m_topControlsHeight = height;
    m_topControlsShrinkBlinkSize = shrinkViewport;
    SetNeedsCommit();
}

void LayerTreeHost::setTopControlsShownRatio(float ratio)
{
    if (zed::almost_equals(m_topControlsShownRatio, ratio))
        return;

    m_topControlsShownRatio = ratio;
    SetNeedsCommit();
}

void LayerTreeHost::setViewportSize(const IntSize &deviceViewportSize)
{
    if (m_deviceViewportSize == deviceViewportSize)
        return;

    m_deviceViewportSize = deviceViewportSize;

    SetPropertyTreesNeedRebuild();
    SetNeedsCommit();
}

void LayerTreeHost::setVisible(bool visible)
{
#if 0 // BKTODO:
    if (never_visible_)
        return;
#endif

    if (m_visible == visible)
        return;
    m_visible = visible;
#if 0 // BKTODO:
    proxy_->SetVisible(visible);
#endif
}

void LayerTreeHost::UnregisterLayer(Layer *layer)
{
    ASSERT(zed::key_exists(m_layers, layer));
    ASSERT(!m_inPaintLayerContents);
#if 0 // BKTODO:
    if (animation_host_)
        animation_host_->UnregisterLayer(layer->id(), LayerTreeType::ACTIVE);
#endif
    m_layers.erase(layer);
}

void LayerTreeHost::Update(std::unique_ptr<PaintUITask> &paintTask)
{
    if (nullptr == m_rootLayer || m_deferCommits || m_deviceViewportSize.isEmpty())
        return;

    std::unique_ptr<RasterTask> task = std::make_unique<RasterTask>(m_deviceViewportSize);
    m_rootLayer->Update(RasterContext(), *task);
    if (task->HasNothingToDo())
        return;

    task->SavePaintTask(paintTask);
    PostTaskToCompositor(task.release());
}

} // namespace BlinKit
