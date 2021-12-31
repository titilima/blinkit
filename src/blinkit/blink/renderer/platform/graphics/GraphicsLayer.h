// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: GraphicsLayer.h
// Description: GraphicsLayer Class
//      Author: Ziming Li
//     Created: 2021-07-17
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2013 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GraphicsLayer_h
#define GraphicsLayer_h

#include "blinkit/blink/public/platform/web_content_layer.h"
#include "blinkit/blink/public/platform/web_image_layer.h"
#include "platform/geometry/FloatPoint.h"
#include "platform/geometry/FloatPoint3D.h"
#include "platform/geometry/FloatSize.h"
#include "platform/geometry/IntRect.h"
#include "platform/graphics/Color.h"
#include "platform/graphics/ContentLayerDelegate.h"
#include "platform/graphics/GraphicsContext.h"
#include "platform/graphics/GraphicsLayerClient.h"
// BKTODO: #include "platform/graphics/GraphicsLayerDebugInfo.h"
#include "platform/graphics/ImageOrientation.h"
#include "platform/graphics/PaintInvalidationReason.h"
#include "platform/graphics/filters/FilterOperations.h"
#include "platform/graphics/paint/CachedDisplayItem.h"
#include "platform/graphics/paint/DisplayItemClient.h"
#include "platform/graphics/paint/PaintController.h"
#include "platform/heap/Handle.h"
#include "platform/transforms/TransformationMatrix.h"
#include "public/platform/WebCompositorAnimationDelegate.h"
#include "public/platform/WebLayerScrollClient.h"
#include "public/platform/WebScrollBlocksOn.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "wtf/OwnPtr.h"
#include "wtf/PassOwnPtr.h"
#include "wtf/Vector.h"

namespace BlinKit {
class GraphicsLayerFactoryImpl;
}

namespace blink {

class FloatRect;
class GraphicsLayerFactory;
class Image;
class LinkHighlight;
class JSONObject;
class PaintController;
class ScrollableArea;
class WebCompositorAnimation;
class WebLayer;

using GraphicsLayerVector = std::vector<GraphicsLayer *>;

// GraphicsLayer is an abstraction for a rendering surface with backing store,
// which may have associated transformation and animations.

class GraphicsLayer : public WebCompositorAnimationDelegate
                    , public WebLayerScrollClient
                    , public DisplayItemClient
{
    WTF_MAKE_NONCOPYABLE(GraphicsLayer);
public:
    static std::unique_ptr<GraphicsLayer> create(GraphicsLayerFactory*, GraphicsLayerClient*);

    ~GraphicsLayer() override;

    GraphicsLayerClient* client() const { return m_client; }

#if 0 // BKTODO:
    GraphicsLayerDebugInfo& debugInfo();

    void setCompositingReasons(CompositingReasons);
    CompositingReasons compositingReasons() const { return m_debugInfo.compositingReasons(); }
#endif
    void setOwnerNodeId(int);

    GraphicsLayer* parent() const { return m_parent; }
    void setParent(GraphicsLayer*); // Internal use only.

    const std::vector<GraphicsLayer *>& children() const { return m_children; }
    // Returns true if the child list changed.
    bool setChildren(const GraphicsLayerVector&);

    // Add child layers. If the child is already parented, it will be removed from its old parent.
    void addChild(GraphicsLayer*);
    void addChildBelow(GraphicsLayer*, GraphicsLayer* sibling);

    void removeAllChildren();
    void removeFromParent();

    GraphicsLayer* maskLayer() const { return m_maskLayer; }
    void setMaskLayer(GraphicsLayer*);

    GraphicsLayer* contentsClippingMaskLayer() const { return m_contentsClippingMaskLayer; }
    void setContentsClippingMaskLayer(GraphicsLayer*);

    // The given layer will replicate this layer and its children; the replica layoutObjects behind this layer.
    void setReplicatedByLayer(GraphicsLayer*);
    // The layer that replicates this layer (if any).
    GraphicsLayer* replicaLayer() const { return m_replicaLayer; }
    // The layer being replicated.
    GraphicsLayer* replicatedLayer() const { return m_replicatedLayer; }

    enum ShouldSetNeedsDisplay {
        DontSetNeedsDisplay,
        SetNeedsDisplay
    };

    // Offset is origin of the layoutObject minus origin of the graphics layer (so either zero or negative).
    IntSize offsetFromLayoutObject() const { return flooredIntSize(m_offsetFromLayoutObject); }
    void setOffsetFromLayoutObject(const IntSize&, ShouldSetNeedsDisplay = SetNeedsDisplay);

    // The double version is only used in |updateScrollingLayerGeometry()| for detecting
    // scroll offset change at floating point precision.
    DoubleSize offsetDoubleFromLayoutObject() const { return m_offsetFromLayoutObject; }
    void setOffsetDoubleFromLayoutObject(const DoubleSize&, ShouldSetNeedsDisplay = SetNeedsDisplay);

    // The position of the layer (the location of its top-left corner in its parent)
    const FloatPoint& position() const { return m_position; }
    void setPosition(const FloatPoint&);

    const FloatPoint3D& transformOrigin() const { return m_transformOrigin; }
    void setTransformOrigin(const FloatPoint3D&);

    // The size of the layer.
    const FloatSize& size() const { return m_size; }
    void setSize(const FloatSize&);

    const TransformationMatrix& transform() const { return m_transform; }
    void setTransform(const TransformationMatrix&);
    void setShouldFlattenTransform(bool);
    void setRenderingContext(int id);
    void setMasksToBounds(bool);

    bool drawsContent() const { return m_drawsContent; }
    void setDrawsContent(bool);

    bool contentsAreVisible() const { return m_contentsVisible; }
    void setContentsVisible(bool);

    void setScrollParent(WebLayer*);
    void setClipParent(WebLayer*);

    // For special cases, e.g. drawing missing tiles on Android.
    // The compositor should never paint this color in normal cases because the Layer
    // will paint background by itself.
    void setBackgroundColor(const Color&);

    // opaque means that we know the layer contents have no alpha
    bool contentsOpaque() const { return m_contentsOpaque; }
    void setContentsOpaque(bool);

    bool backfaceVisibility() const { return m_backfaceVisibility; }
    void setBackfaceVisibility(bool visible);

    float opacity() const { return m_opacity; }
    void setOpacity(float);

    void setBlendMode(WebBlendMode);
    void setScrollBlocksOn(WebScrollBlocksOn);
    void setIsRootForIsolatedGroup(bool);

    void setFilters(FilterOperations &&);
    void setBackdropFilters(FilterOperations &&);

    void setFilterQuality(SkFilterQuality);

    // Some GraphicsLayers paint only the foreground or the background content
    void setPaintingPhase(GraphicsLayerPaintingPhase);

    void setNeedsDisplay();
    // Mark the given rect (in layer coords) as needing display. Never goes deep.
    void setNeedsDisplayInRect(const IntRect&, PaintInvalidationReason);

    void setContentsNeedsDisplay();

    void invalidateDisplayItemClient(const DisplayItemClient&, PaintInvalidationReason);

    // Set that the position/size of the contents (image or video).
    void setContentsRect(const IntRect&);

    // Return true if the animation is handled by the compositing system. If this returns
    // false, the animation will be run by AnimationController.
    // These methods handle both transitions and keyframe animations.
    bool addAnimation(PassOwnPtr<WebCompositorAnimation>);
    void pauseAnimation(int animationId, double /*timeOffset*/);
    void removeAnimation(int animationId);
    void abortAnimation(int animationId);

    // Layer contents
    void setContentsToImage(Image*, RespectImageOrientationEnum = DoNotRespectImageOrientation);
    void setContentsToPlatformLayer(WebLayer* layer) { setContentsTo(layer); }
    bool hasContentsLayer() const { return m_contentsLayer; }

    // For hosting this GraphicsLayer in a native layer hierarchy.
    WebLayer* platformLayer() const;

    typedef HashMap<int, int> RenderingContextMap;
    // BKTODO: PassRefPtr<JSONObject> layerTreeAsJSON(LayerTreeFlags, RenderingContextMap&) const;

    int paintCount() const { return m_paintCount; }

#if 0 // BKTODO:
    // Return a string with a human readable form of the layer tree, If debug is true
    // pointers for the layers and timing data will be included in the returned string.
    String layerTreeAsText(LayerTreeFlags = LayerTreeNormal) const;
#endif

    bool isTrackingPaintInvalidations() const { return m_client->isTrackingPaintInvalidations(); }
    void resetTrackedPaintInvalidations();
    bool hasTrackedPaintInvalidations() const;
    void trackPaintInvalidationRect(const FloatRect&);
    void trackPaintInvalidationObject(const String&);

    void addLinkHighlight(LinkHighlight*);
    void removeLinkHighlight(LinkHighlight*);
    // Exposed for tests
    unsigned numLinkHighlights() { return m_linkHighlights.size(); }
    LinkHighlight* linkHighlight(int i) { return m_linkHighlights[i]; }

    void setScrollableArea(ScrollableArea*, bool isViewport);
    ScrollableArea* scrollableArea() const { return m_scrollableArea; }

    WebContentLayer* contentLayer() const { return m_layer.get(); }

    static void registerContentsLayer(WebLayer*);
    static void unregisterContentsLayer(WebLayer*);

    IntRect interestRect();
    void paint(const IntRect* interestRect, GraphicsContext::DisabledMode = GraphicsContext::NothingDisabled);

    // WebCompositorAnimationDelegate implementation.
    void notifyAnimationStarted(double monotonicTime, int group) override;
    void notifyAnimationFinished(double monotonicTime, int group) override;
    void notifyAnimationAborted(double monotonicTime, int group) override;

    // WebLayerScrollClient implementation.
    void didScroll() override;

#if 0 // BKTODO:
    // cc::LayerClient implementation.
    scoped_refptr<base::trace_event::ConvertableToTraceFormat> TakeDebugInfo(cc::Layer*) override;
#endif

    PaintController& paintController();

    // Exposed for tests.
    WebLayer* contentsLayer() const { return m_contentsLayer; }

    void setElementId(uint64_t);
    void setCompositorMutableProperties(uint32_t);

    static void setDrawDebugRedFillForTesting(bool);
    ContentLayerDelegate* contentLayerDelegateForTesting() const { return m_contentLayerDelegate.get(); }

    // DisplayItemClient methods
    String debugName() const final { return m_client->debugName(this); }
    IntRect visualRect() const override;

protected:
    String debugName(cc::Layer*) const;
    bool shouldFlattenTransform() const { return m_shouldFlattenTransform; }

    explicit GraphicsLayer(GraphicsLayerClient*);
    // GraphicsLayerFactoryImpl that wants to create a GraphicsLayer need to be friends.
    friend class BlinKit::GraphicsLayerFactoryImpl;
    // for testing
    friend class CompositedLayerMappingTest;
    friend class FakeGraphicsLayerFactory;
    friend class PaintControllerPaintTestBase;

private:
    // Returns true if PaintController::paintArtifact() changed and needs commit.
    bool paintWithoutCommit(const IntRect* interestRect, GraphicsContext::DisabledMode = GraphicsContext::NothingDisabled);

    // Adds a child without calling updateChildList(), so that adding children
    // can be batched before updating.
    void addChildInternal(GraphicsLayer*);

#if ENABLE(ASSERT)
    bool hasAncestor(GraphicsLayer*) const;
#endif

    void setReplicatedLayer(GraphicsLayer* layer) { m_replicatedLayer = layer; }

    void incrementPaintCount() { ++m_paintCount; }

    void notifyFirstPaintToClient();

    // Helper functions used by settors to keep layer's the state consistent.
    void updateChildList();
    void updateLayerIsDrawable();
    void updateContentsRect();

    void setContentsTo(WebLayer*);
    void setupContentsLayer(WebLayer*);
    void clearContentsLayerIfUnregistered();
    WebLayer* contentsLayerIfRegistered();

    GraphicsLayerClient* m_client;

    // Offset from the owning layoutObject
    DoubleSize m_offsetFromLayoutObject;

    // Position is relative to the parent GraphicsLayer
    FloatPoint m_position;
    FloatSize m_size;

    TransformationMatrix m_transform;
    FloatPoint3D m_transformOrigin;

    Color m_backgroundColor;
    float m_opacity;

    WebBlendMode m_blendMode;

    WebScrollBlocksOn m_scrollBlocksOn;

    bool m_hasTransformOrigin : 1;
    bool m_contentsOpaque : 1;
    bool m_shouldFlattenTransform: 1;
    bool m_backfaceVisibility : 1;
    bool m_masksToBounds : 1;
    bool m_drawsContent : 1;
    bool m_contentsVisible : 1;
    bool m_isRootForIsolatedGroup : 1;

    bool m_hasScrollParent : 1;
    bool m_hasClipParent : 1;

    bool m_painted : 1;
    bool m_textPainted : 1;
    bool m_imagePainted : 1;

    GraphicsLayerPaintingPhase m_paintingPhase;

    std::vector<GraphicsLayer *> m_children;
    GraphicsLayer* m_parent;

    GraphicsLayer* m_maskLayer; // Reference to mask layer. We don't own this.
    GraphicsLayer* m_contentsClippingMaskLayer; // Reference to clipping mask layer. We don't own this.

    // A layer that replicates this layer. We only allow one, for now.
    // The replica is not parented; this is the primary reference to it.
    GraphicsLayer* m_replicaLayer;
    GraphicsLayer* m_replicatedLayer; // For a replica layer, a reference to the original layer.
    FloatPoint m_replicatedLayerPosition; // For a replica layer, the position of the replica.

    IntRect m_contentsRect;

    int m_paintCount;

    std::unique_ptr<WebContentLayer> m_layer;
    OwnPtr<WebImageLayer> m_imageLayer;
    WebLayer* m_contentsLayer;
    // We don't have ownership of m_contentsLayer, but we do want to know if a given layer is the
    // same as our current layer in setContentsTo(). Since m_contentsLayer may be deleted at this point,
    // we stash an ID away when we know m_contentsLayer is alive and use that for comparisons from that point
    // on.
    int m_contentsLayerId;

    Vector<LinkHighlight*> m_linkHighlights;

    OwnPtr<ContentLayerDelegate> m_contentLayerDelegate;

    ScrollableArea *m_scrollableArea = nullptr;
    // BKTODO: GraphicsLayerDebugInfo m_debugInfo;
    int m_3dRenderingContext;

    OwnPtr<PaintController> m_paintController;

    IntRect m_previousInterestRect;
};

} // namespace blink

#endif // GraphicsLayer_h
