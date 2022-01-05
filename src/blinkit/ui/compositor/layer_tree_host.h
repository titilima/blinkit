#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer_tree_host.h
// Description: LayerTreeHost Class
//      Author: Ziming Li
//     Created: 2021-12-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_LAYER_TREE_HOST_H
#define BLINKIT_LAYER_TREE_HOST_H

#include <unordered_set>
#include "blinkit/blink/public/platform/web_layer_tree_view.h"
#include "third_party/skia/include/core/SkColor.h"

namespace BlinKit {

class CompositorTask;
class Layer;
class PaintUITask;

class LayerTreeHost final : public WebLayerTreeView
{
public:
    LayerTreeHost(void);
    ~LayerTreeHost(void) override;

    void RegisterLayer(Layer *layer);
    void UnregisterLayer(Layer *layer);

    const IntSize& DeviceViewportSize(void) const { return m_deviceViewportSize; }

    void Update(std::unique_ptr<PaintUITask> &paintTask);

    void SetNeedsCommit(void);
    void SetNeedsFullTreeSync(void);
    void SetNeedsUpdateLayers(void);

    void Paint(HDC hdc, const RECT *rc);

#ifndef NDEBUG
    bool InPaintLayerContents(void) const { return m_inPaintLayerContents; }
#endif

    void setViewportSize(const IntSize &deviceViewportSize) override;
    void setDeviceScaleFactor(float scaleFactor) override;
private:
    void SetPropertyTreesNeedRebuild(void);
    void SetRootLayerImpl(Layer *layer);

    void PostTaskToCompositor(CompositorTask *task);

    void setRootLayer(const WebLayer &layer) override;
    void clearRootLayer(void) override;
    void setBackgroundColor(WebColor color) override { m_backgroundColor = color; }
    void setVisible(bool visible) override;
    void setPageScaleFactorAndLimits(float pageScaleFactor, float minimum, float maximum) override; // BKTODO: Check if necessary.
    void heuristicsForGpuRasterizationUpdated(bool matchesHeuristics) override;
    void setTopControlsShownRatio(float ratio) override;
    void setTopControlsHeight(float height, bool shrinkViewport) override;
    void setDeferCommits(bool deferCommits) override {
        m_deferCommits = deferCommits;
    }
    void registerForAnimations(WebLayer *layer) override;
    void registerViewportLayers(const WebLayer *overscrollElasticityLayer,
        const WebLayer *pageScaleLayer, const WebLayer *innerViewportScrollLayer,
        const WebLayer *outerViewportScrollLayer) override;
    void clearViewportLayers(void) override;

    Layer *m_rootLayer = nullptr;
    std::unordered_set<Layer *> m_layers;

    bool m_deferCommits = false;
    bool m_visible = false;
    bool m_topControlsShrinkBlinkSize = false;
#ifndef NDEBUG
    bool m_inPaintLayerContents = false;
#endif
    SkColor m_backgroundColor = SK_ColorWHITE;

    IntSize m_deviceViewportSize;
    float m_deviceScaleFactor = 1.f;

    float m_topControlsShownRatio = 0.f;
    float m_topControlsHeight = 0.f;
    float m_pageScaleFactor = 1.f;
    float m_minPageScaleFactor = 1.f;
    float m_maxPageScaleFactor = 1.f;
};

} // namespace BlinKit

#endif // BLINKIT_LAYER_TREE_HOST_H
