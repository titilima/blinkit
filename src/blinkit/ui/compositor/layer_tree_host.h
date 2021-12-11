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
#include "blinkit/blink/public/platform/WebLayerTreeView.h"
#include "third_party/skia/include/core/SkColor.h"

namespace BlinKit {

class Layer;

class LayerTreeHost final : public blink::WebLayerTreeView
{
public:
    void RegisterLayer(Layer *layer);
    void UnregisterLayer(Layer *layer);

    void SetNeedsCommit(void);
    void SetNeedsFullTreeSync(void);
    void SetNeedsUpdateLayers(void);

    void Paint(HDC hdc, const RECT *rc);

#ifndef NDEBUG
    bool InPaintLayerContents(void) const { return m_inPaintLayerContents; }
#endif
private:
    void SetPropertyTreesNeedRebuild(void);
    void SetRootLayerImpl(Layer *layer);

    void setRootLayer(const blink::WebLayer &layer) override;
    void clearRootLayer(void) override;
    void setBackgroundColor(blink::WebColor color) override { m_backgroundColor = color; }
    void setVisible(bool visible) override;
    void setPageScaleFactorAndLimits(float pageScaleFactor, float minimum, float maximum) override;
    void heuristicsForGpuRasterizationUpdated(bool matchesHeuristics) override;
    void setTopControlsShownRatio(float ratio) override;
    void setTopControlsHeight(float height, bool shrinkViewport) override;
    void setDeferCommits(bool deferCommits) override;
    void registerForAnimations(blink::WebLayer *layer) override;
    void registerViewportLayers(const blink::WebLayer *overscrollElasticityLayer,
        const blink::WebLayer *pageScaleLayer, const blink::WebLayer *innerViewportScrollLayer,
        const blink::WebLayer *outerViewportScrollLayer) override;
    void clearViewportLayers(void) override;

    Layer *m_rootLayer = nullptr;
    std::unordered_set<Layer *> m_layers;

    bool m_visible = false;
    bool m_topControlsShrinkBlinkSize = false;
#ifndef NDEBUG
    bool m_inPaintLayerContents = false;
#endif
    SkColor m_backgroundColor = SK_ColorWHITE;
    float m_topControlsShownRatio = 0.f;
    float m_topControlsHeight = 0.f;
    float m_pageScaleFactor = 1.f;
    float m_minPageScaleFactor = 1.f;
    float m_maxPageScaleFactor = 1.f;
};

} // namespace BlinKit

#endif // BLINKIT_LAYER_TREE_HOST_H
