#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer.h
// Description: Layer Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_LAYER_H
#define BLINKIT_LAYER_H

#include "blinkit/blink/public/platform/web_layer.h"
#include "blinkit/blink/public/platform/web_layer_position_constraint.h"
#include "blinkit/blink/renderer/platform/geometry/Region.h"
#include "blinkit/blink/renderer/platform/graphics/filters/FilterOperations.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/utils/SkMatrix44.h"

namespace BlinKit {

class CompositorTask;
class LayerClient;
class LayerTreeHost;
struct PaintContext;
class RasterContext;
class RasterTask;

class Layer : public WebLayer
{
public:
    Layer(LayerClient *client);
    ~Layer(void) override;
    enum { INVALID_ID = 0 };

    LayerClient* Client(void) const { return m_client; }
    Layer* Parent(void) { return m_parent; }

    LayerTreeHost* GetLayerTreeHost(void) { return m_layerTreeHost; }
    const LayerTreeHost* GetLayerTreeHost(void) const { return m_layerTreeHost; }
    void SetLayerTreeHost(LayerTreeHost *host);

    const FloatPoint& Position(void) const { return m_position; }
    const IntSize& Bounds(void) const { return m_bounds; }
    const SkMatrix44& Transform(void) const { return m_transform; }

    void Update(const RasterContext &context, RasterTask &session);

    // Called when there's been a change in layer structure.  Implies both
    // SetNeedsUpdate and SetNeedsCommit, but not SetNeedsPushProperties.
    void SetNeedsFullTreeSync(void);

#ifndef NDEBUG
    void DebugPrint(void) const {
        DebugPrint(0);
    }
#endif

    // WebLayer
    void removeFromParent(void) override;
    bool drawsContent(void) const override { return m_drawsContent; }
private:
    void AddDrawableDescendants(int num);
    int NumDescendantsThatDrawContent(void) const { return m_numDescendantsThatDrawContent; }

    void PreparePaintData(PaintContext &dst, const RasterContext &context, const IntRect &visibleRect);

    void PostTaskToCompositor(CompositorTask *task);

#ifndef NDEBUG
    void DebugPrint(int depth) const;
    bool HasAncestor(const Layer *ancestor) const;
    bool IsPropertyChangeAllowed(void) const;
#endif

    // Called when a property has been modified in a way that the layer
    // knows immediately that a commit is required.  This implies SetNeedsUpdate
    // as well as SetNeedsPushProperties to push that property.
    void SetNeedsCommit(void);
    // This is identical to SetNeedsCommit, but the former requests a rebuild of
    // the property trees.
    void SetNeedsCommitNoRebuild(void);
    void SetNeedsPushProperties(void);
    // Called when this layer has been modified in some way, but isn't sure
    // that it needs a commit yet.  It needs CalcDrawProperties and UpdateLayers
    // before it knows whether or not a commit is required.
    void SetNeedsUpdate(void);

    void SetParent(Layer *layer);
    void RemoveChildOrDependent(Layer *child);

    // WebLayer
    void invalidateRect(const IntRect &rect) override;
    void invalidate(void) override;
    void addChild(WebLayer *child) override;
    void insertChild(WebLayer *child, size_t index) override;
    void removeAllChildren(void) override;
    void setBounds(const IntSize &size) override;
    IntSize bounds(void) const override { return m_bounds; }
    void setMasksToBounds(bool masksToBounds) override;
    bool masksToBounds(void) const override { return m_masksToBounds; }
    void setMaskLayer(WebLayer *maskLayer) override;
    void setReplicaLayer(WebLayer *replicaLayer) override;
    void setOpacity(float opacity) override;
    float opacity(void) const override { return m_opacity; }
    void setOpaque(bool opaque) override;
    bool opaque(void) const { return m_opaque; }
    void setPosition(const FloatPoint &position) override;
    FloatPoint position(void) const override { return m_position; }
    void setTransform(const SkMatrix44 &transform) override;
    SkMatrix44 transform(void) const override { return m_transform; }
    void setTransformOrigin(const FloatPoint3D &transformOrigin) override;
    FloatPoint3D transformOrigin(void) const override { return m_transformOrigin; }
    void setDrawsContent(bool drawsContent) override;
    void setDoubleSided(bool doubleSided) override;
    void setRenderingContext(int id) override;
    void setUseParentBackfaceVisibility(bool use) override { m_useParentBackfaceVisibility = use; }
    void setBackgroundColor(WebColor color) override;
    WebColor backgroundColor(void) const override { return m_backgroundColor; }
    void setFilters(FilterOperations &&filters) override;
    void setBackgroundFilters(FilterOperations &&backgroundFilters) override;
    void setScrollParent(WebLayer *parent) override;
    void setClipParent(WebLayer *parent) override;
    void setScrollPositionDouble(const DoublePoint &scrollPosition) override;
    DoublePoint scrollPositionDouble(void) const override { return m_scrollPosition; }
    void setScrollCompensationAdjustment(const DoublePoint &) override;
    void setScrollClipLayer(WebLayer *clipLayer) override;
    void setUserScrollable(bool horizontal, bool vertical) override;
    bool userScrollableHorizontal(void) const override { return m_userScrollableHorizontal; }
    bool userScrollableVertical(void) const override { return m_userScrollableVertical; }
    void setHaveWheelEventHandlers(bool haveWheelEventHandlers) override { m_haveWheelEventHandlers = haveWheelEventHandlers; }
    bool haveWheelEventHandlers(void) const override { return m_haveWheelEventHandlers; }
    void setHaveScrollEventHandlers(bool haveScrollEventHandlers) override { m_haveScrollEventHandlers = haveScrollEventHandlers; }
    bool haveScrollEventHandlers(void) const override { return m_haveScrollEventHandlers; }
    void setShouldScrollOnMainThread(bool shouldScrollOnMainThread) override;
    bool shouldScrollOnMainThread(void) const override { return m_shouldScrollOnMainThread; }
    void setNonFastScrollableRegion(const Region &region) override;
    Region nonFastScrollableRegion(void) const override { return m_nonFastScrollableRegion; }
    void setScrollBlocksOn(WebScrollBlocksOn scrollBlocksOn) override;
    WebScrollBlocksOn scrollBlocksOn(void) const override { return m_scrollBlocksOn; }
    void setIsContainerForFixedPositionLayers(bool isContainer) override;
    void setPositionConstraint(const WebLayerPositionConstraint &constraint) override;
    WebLayerPositionConstraint positionConstraint(void) const override { return m_positionConstraint;  }
    void setScrollClient(WebLayerScrollClient *scrollClient) override { m_scrollClient = scrollClient; }

    LayerClient *m_client;

    Layer *m_parent = nullptr;
    using Children = std::vector<Layer *>;
    Children m_children;
    Layer *m_mask = nullptr, *m_replica = nullptr;
    Layer *m_scrollParent = nullptr, *m_clipParent = nullptr;

    // BKTODO: Use bitfields for all boolean members.
    SkColor m_backgroundColor = 0;
    IntSize m_bounds;
    bool m_dirty = false, m_doubleSided = true, m_drawsContent = true;
    FilterOperations m_filters, m_backgroundFilters;
    bool m_haveScrollEventHandlers = false, m_haveWheelEventHandlers = false;
    // When true, the layer is about to perform an update. Any commit requests
    // will be handled implicitly after the update completes.
    bool m_ignoreSetNeedsCommit = false;
    bool m_isContainerForFixedPositionLayers = false;
    LayerTreeHost *m_layerTreeHost = nullptr;
    bool m_masksToBounds = false;
    Region m_nonFastScrollableRegion;
    int m_numDescendantsThatDrawContent = 0;
    float m_opacity = 1.f;
    bool m_opaque = false;
    FloatPoint m_position; // of parent
    WebLayerPositionConstraint m_positionConstraint;
    WebScrollBlocksOn m_scrollBlocksOn = WebScrollBlocksOnNone;
    WebLayerScrollClient *m_scrollClient = nullptr;
    int m_scrollClipLayerId = Layer::INVALID_ID;
    DoublePoint m_scrollPosition, m_scrollCompensationAdjustment;
    bool m_shouldScrollOnMainThread = true;
    int m_sortingContextId = 0;
    // Tracks whether this layer may have changed stacking order with its
    // siblings.
    bool m_stackingOrderChanged = false;
    SkMatrix44 m_transform{ SkMatrix44::kIdentity_Constructor };
    FloatPoint3D m_transformOrigin;

    bool m_fullyInvalidation = false;
    IntRect m_dirtyRect; // of viewport

    bool m_useParentBackfaceVisibility = false;
    bool m_userScrollableHorizontal = true, m_userScrollableVertical = true;
};

#ifndef NDEBUG
extern std::unordered_set<Layer *> g_allLayers;
#endif

} // namespace BlinKit

#ifndef NDEBUG
namespace zed {
template <>
void log_serializer::push<BlinKit::Layer>(std::vector<std::string> &dst, const BlinKit::Layer &layer);
}
#endif

#endif // BLINKIT_LAYER_H
