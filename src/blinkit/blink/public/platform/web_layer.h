#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_layer.h
// Description: WebLayer Class
//      Author: Ziming Li
//     Created: 2021-07-17
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebLayer_h
#define WebLayer_h

#include "blinkit/blink/public/platform/WebBlendMode.h"
#include "blinkit/blink/public/platform/WebColor.h"
#include "blinkit/blink/public/platform/WebCommon.h"
#include "blinkit/blink/public/platform/WebCompositorAnimation.h"
#include "blinkit/blink/public/platform/WebScrollBlocksOn.h"
#include "blinkit/blink/renderer/platform/geometry/DoublePoint.h"
#include "blinkit/blink/renderer/platform/geometry/float_point_3d.h"
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"

class SkMatrix44;
class SkImageFilter;

namespace cc {
class Layer;
}

namespace blink {

class FilterOperations;
class Region;
class WebCompositorAnimationDelegate;
class WebFilterOperations;
class WebLayerScrollClient;
struct WebFloatPoint;
struct WebLayerPositionConstraint;

class WebLayer {
public:
    virtual ~WebLayer() { }

    // Returns a positive ID that will be unique across all WebLayers allocated in this process.
    int id() const {
        static_assert(sizeof(WebLayer *) == sizeof(int));
        return reinterpret_cast<int>(this);
    }

    // Sets a region of the layer as invalid, i.e. needs to update its content.
    virtual void invalidateRect(const IntRect&) = 0;

    // Sets the entire layer as invalid, i.e. needs to update its content.
    virtual void invalidate() = 0;

    // These functions do not take ownership of the WebLayer* parameter.
    virtual void addChild(WebLayer*) = 0;
    virtual void insertChild(WebLayer*, size_t index) = 0;
#if 0 // BKTODO:
    virtual void replaceChild(WebLayer* reference, WebLayer* newLayer) = 0;
#endif
    virtual void removeFromParent() = 0;
    virtual void removeAllChildren() = 0;

    virtual void setBounds(const IntSize&) = 0;
    virtual IntSize bounds() const = 0;

    virtual void setMasksToBounds(bool) = 0;
    virtual bool masksToBounds() const = 0;

    virtual void setMaskLayer(WebLayer*) = 0;
    virtual void setReplicaLayer(WebLayer*) = 0;

    virtual void setOpacity(float) = 0;
    virtual float opacity() const = 0;

#if 0 // BKTODO:
    virtual void setBlendMode(WebBlendMode) = 0;
    virtual WebBlendMode blendMode() const = 0;

    virtual void setIsRootForIsolatedGroup(bool) = 0;
    virtual bool isRootForIsolatedGroup() = 0;
#endif

    virtual void setOpaque(bool) = 0;
    virtual bool opaque() const = 0;

    virtual void setPosition(const FloatPoint&) = 0;
    virtual FloatPoint position() const = 0;

    virtual void setTransform(const SkMatrix44&) = 0;
    virtual SkMatrix44 transform() const = 0;

    virtual void setTransformOrigin(const FloatPoint3D&) { }
    virtual FloatPoint3D transformOrigin() const { return FloatPoint3D(); }

    // Sets whether the layer draws its content when compositing.
    virtual void setDrawsContent(bool) = 0;
    virtual bool drawsContent() const = 0;

    // Set to true if the backside of this layer's contents should be visible
    // when composited. Defaults to false.
    virtual void setDoubleSided(bool) = 0;

#if 0 // BKTODO:
    // Sets whether the layer's transform should be flattened.
    virtual void setShouldFlattenTransform(bool) = 0;
#endif

    // Sets the id of the layer's 3d rendering context. Layers in the same 3d
    // rendering context id are sorted with one another according to their 3d
    // position rather than their tree order.
    virtual void setRenderingContext(int id) = 0;

    // Mark that this layer should use its parent's transform and double-sided
    // properties in determining this layer's backface visibility instead of
    // using its own properties. If this property is set, this layer must
    // have a parent, and the parent may not have this property set.
    // Note: This API is to work around issues with visibility the handling of
    // WebKit layers that have a contents layer (canvas, plugin, WebGL, video,
    // etc).
    virtual void setUseParentBackfaceVisibility(bool) = 0;

    virtual void setBackgroundColor(WebColor) = 0;
    virtual WebColor backgroundColor() const = 0;

    // Clear the filters in use by passing in a newly instantiated
    // WebFilterOperations object.
    virtual void setFilters(FilterOperations &&) = 0;

    // Clear the background filters in use by passing in a newly instantiated
    // WebFilterOperations object.
    virtual void setBackgroundFilters(FilterOperations &&) = 0;

#if 0 // BKTODO:
    // An animation delegate is notified when animations are started and
    // stopped. The WebLayer does not take ownership of the delegate, and it is
    // the responsibility of the client to reset the layer's delegate before
    // deleting the delegate.
    virtual void setAnimationDelegate(WebCompositorAnimationDelegate*) = 0;


    // Returns false if the animation cannot be added.
    // Takes ownership of the WebCompositorAnimation object.
    virtual bool addAnimation(WebCompositorAnimation*) = 0;

    // Removes all animations with the given id.
    virtual void removeAnimation(int animationId) = 0;

    // Removes all animations with the given id targeting the given property.
    virtual void removeAnimation(int animationId, WebCompositorAnimation::TargetProperty) = 0;

    // Pauses all animations with the given id.
    virtual void pauseAnimation(int animationId, double timeOffset) = 0;

    // Aborts all animations with the given id. Different from removeAnimation
    // in that aborting an animation stops it from affecting both the pending
    // and active tree.
    virtual void abortAnimation(int animationId) = 0;

    // Returns true if this layer has any active animations - useful for tests.
    virtual bool hasActiveAnimation() = 0;
#endif

    // If a scroll parent is set, this layer will inherit its parent's scroll
    // delta and offset even though it will not be a descendant of the scroll
    // in the layer hierarchy.
    virtual void setScrollParent(WebLayer*) = 0;

    // A layer will not respect any clips established by layers between it and
    // its nearest clipping ancestor. Note, the clip parent must be an ancestor.
    // This is not a requirement of the scroll parent.
    virtual void setClipParent(WebLayer*) = 0;

    // Scrolling
    virtual void setScrollPositionDouble(const DoublePoint &) = 0;
    virtual DoublePoint scrollPositionDouble() const = 0;
    // Blink tells cc the scroll offset through setScrollPositionDouble() using
    // floating precision but it currently can only position cc layers at integer
    // boundary. So Blink needs to also call setScrollCompensationAdjustment()
    // to tell cc what's the part of the scroll offset that Blink doesn't handle
    // but cc needs to take into consideration, e.g. compensating
    // for fixed-position layer that's positioned in Blink using only integer scroll
    // offset.
    // We make this call explicit, instead of letting cc to infer the fractional part
    // from the scroll offset, to be clear that this is Blink's limitation. Once
    // Blink can fully handle fractional scroll offset, it can stop calling
    // this function and cc side would just work.
    virtual void setScrollCompensationAdjustment(const DoublePoint &) = 0;

    // To set a WebLayer as scrollable we must specify the corresponding clip layer.
    virtual void setScrollClipLayer(WebLayer*) = 0;
#if 0 // BKTODO:
    virtual bool scrollable() const = 0;
#endif
    virtual void setUserScrollable(bool horizontal, bool vertical) = 0;
    virtual bool userScrollableHorizontal() const = 0;
    virtual bool userScrollableVertical() const = 0;

    virtual void setHaveWheelEventHandlers(bool) = 0;
    virtual bool haveWheelEventHandlers() const = 0;

    virtual void setHaveScrollEventHandlers(bool) = 0;
    virtual bool haveScrollEventHandlers() const = 0;

    virtual void setShouldScrollOnMainThread(bool) = 0;
    virtual bool shouldScrollOnMainThread() const = 0;

    virtual void setNonFastScrollableRegion(const Region &) = 0;
    virtual Region nonFastScrollableRegion() const = 0;

#if 0 // BKTODO:
    virtual void setTouchEventHandlerRegion(const std::vector<IntRect>&) = 0;
    virtual std::vector<IntRect> touchEventHandlerRegion() const = 0;

    // Setter and getter for Frame Timing rects.
    // See http://w3c.github.io/frame-timing/ for definition of terms.
    virtual void setFrameTimingRequests(const std::vector<std::pair<int64_t, IntRect>>&) = 0;
    virtual std::vector<std::pair<int64_t, IntRect>> frameTimingRequests() const = 0;
#endif

    // FIXME: Make pure once cc is updated.  crbug.com/347272
    virtual void setScrollBlocksOn(WebScrollBlocksOn) { }
    virtual WebScrollBlocksOn scrollBlocksOn() const { return WebScrollBlocksOnNone; }

    virtual void setIsContainerForFixedPositionLayers(bool) = 0;
#if 0 // BKTODO:
    virtual bool isContainerForFixedPositionLayers() const = 0;
#endif

    // This function sets layer position constraint. The constraint will be used
    // to adjust layer position during threaded scrolling.
    virtual void setPositionConstraint(const WebLayerPositionConstraint&) = 0;
    virtual WebLayerPositionConstraint positionConstraint() const = 0;

    // The scroll client is notified when the scroll position of the WebLayer
    // changes. Only a single scroll client can be set for a WebLayer at a time.
    // The WebLayer does not take ownership of the scroll client, and it is the
    // responsibility of the client to reset the layer's scroll client before
    // deleting the scroll client.
    virtual void setScrollClient(WebLayerScrollClient*) = 0;

#if 0 // BKTODO:
    // Forces this layer to use a render surface. There is no benefit in doing
    // so, but this is to facilitate benchmarks and tests.
    virtual void setForceRenderSurface(bool) = 0;

    // True if the layer is not part of a tree attached to a WebLayerTreeView.
    virtual bool isOrphan() const = 0;

    // Gets the underlying cc layer.
    virtual const cc::Layer* ccLayer() const = 0;

    virtual void setElementId(uint64_t) = 0;
    virtual uint64_t elementId() const = 0;

    virtual void setCompositorMutableProperties(uint32_t) = 0;
    virtual uint32_t compositorMutableProperties() const = 0;
#endif
};

} // namespace blink

#endif // WebLayer_h
