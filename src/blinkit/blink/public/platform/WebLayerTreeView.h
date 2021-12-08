// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebLayerTreeView.h
// Description: WebLayerTreeView Class
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

#ifndef WebLayerTreeView_h
#define WebLayerTreeView_h

#include "WebColor.h"
#include "WebCommon.h"
#include "WebTopControlsState.h"
#include "blinkit/blink/renderer/platform/geometry/FloatPoint.h"
#include "blinkit/blink/renderer/platform/geometry/IntSize.h"

class SkBitmap;

namespace blink {

class WebCompositeAndReadbackAsyncCallback;
class WebCompositorAnimationTimeline;
class WebLayer;
class WebLayoutAndPaintAsyncCallback;
struct WebPoint;
struct WebSelectionBound;
class WebSelection;
class WebWidget;

class WebLayerTreeView {
public:
    virtual ~WebLayerTreeView() { }

    // Initialization and lifecycle --------------------------------------

    // Sets the root of the tree. The root is set by way of the constructor.
    virtual void setRootLayer(const WebLayer&) { }
    virtual void clearRootLayer() { }

#if 0 // BKTODO:
    virtual void attachCompositorAnimationTimeline(WebCompositorAnimationTimeline*) { }
    virtual void detachCompositorAnimationTimeline(WebCompositorAnimationTimeline*) { }

    // View properties ---------------------------------------------------

    virtual void setViewportSize(const IntSize& deviceViewportSize) { }

    virtual void setDeviceScaleFactor(float) { }
#endif

    // Sets the background color for the viewport.
    virtual void setBackgroundColor(WebColor) { }

#if 0 // BKTODO:
    // Sets the background transparency for the viewport. The default is 'false'.
    virtual void setHasTransparentBackground(bool) { }
#endif

    // Sets whether this view is visible. In threaded mode, a view that is not visible will not
    // composite or trigger updateAnimations() or layout() calls until it becomes visible.
    virtual void setVisible(bool) { }

    // Sets the current page scale factor and minimum / maximum limits. Both limits are initially 1 (no page scale allowed).
    virtual void setPageScaleFactorAndLimits(float pageScaleFactor, float minimum, float maximum) { }

#if 0 // BKTODO:
    // Starts an animation of the page scale to a target scale factor and scroll offset.
    // If useAnchor is true, destination is a point on the screen that will remain fixed for the duration of the animation.
    // If useAnchor is false, destination is the final top-left scroll position.
    virtual void startPageScaleAnimation(const WebPoint& destination, bool useAnchor, float newPageScale, double durationSec) { }
#endif

    virtual void heuristicsForGpuRasterizationUpdated(bool) { }

    // Sets the amount that the top controls are showing, from 0 (hidden) to 1
    // (fully shown).
    virtual void setTopControlsShownRatio(float) { }

#if 0 // BKTODO:
    // Update top controls permitted and current states
    virtual void updateTopControlsState(WebTopControlsState constraints, WebTopControlsState current, bool animate) { }
#endif

    // Set top controls height. If |shrinkViewport| is set to true, then Blink shrunk the viewport clip
    // layers by the top controls height.
    virtual void setTopControlsHeight(float height, bool shrinkViewport) { }

#if 0 // BKTODO:
    // Flow control and scheduling ---------------------------------------

    // Indicates that an animation needs to be updated.
    // TODO(skyostil): Remove once all callers have migrated to requesting
    // begin frames or compositor updates.
    virtual void setNeedsAnimate() { }

    // Indicates that blink needs a BeginFrame, but that nothing might actually be dirty.
    virtual void setNeedsBeginFrame() { setNeedsAnimate(); }

    // Indicates that blink needs a BeginFrame and to update compositor state.
    virtual void setNeedsCompositorUpdate() { setNeedsAnimate(); }

    // Relays the end of a fling animation.
    virtual void didStopFlinging() { }

    // Run layout and paint of all pending document changes asynchronously.
    // The caller is resposible for keeping the WebLayoutAndPaintAsyncCallback object
    // alive until it is called.
    virtual void layoutAndPaintAsync(WebLayoutAndPaintAsyncCallback*) { }

    // The caller is responsible for keeping the WebCompositeAndReadbackAsyncCallback
    // object alive until it is called.
    virtual void compositeAndReadbackAsync(WebCompositeAndReadbackAsyncCallback*) { }
#endif

    // Prevents updates to layer tree from becoming visible.
    virtual void setDeferCommits(bool deferCommits) { }

    // Take responsiblity for this layer's animations, even if this layer hasn't yet
    // been added to the tree.
    virtual void registerForAnimations(WebLayer* layer) { }

    // Identify key layers to the compositor when using the pinch virtual viewport.
    virtual void registerViewportLayers(
        const WebLayer* overscrollElasticityLayer,
        const WebLayer* pageScaleLayer,
        const WebLayer* innerViewportScrollLayer,
        const WebLayer* outerViewportScrollLayer) { }
    virtual void clearViewportLayers() { }

#if 0 // BKTODO:
    // Used to update the active selection bounds.
    virtual void registerSelection(const WebSelection&) { }
    virtual void clearSelection() { }

    // Debugging / dangerous ---------------------------------------------

    virtual int layerTreeId() const { return 0; }

    // Toggles the FPS counter in the HUD layer
    virtual void setShowFPSCounter(bool) { }

    // Toggles the paint rects in the HUD layer
    virtual void setShowPaintRects(bool) { }

    // Toggles the debug borders on layers
    virtual void setShowDebugBorders(bool) { }

    // Toggles scroll bottleneck rects on the HUD layer
    virtual void setShowScrollBottleneckRects(bool) { }
#endif
};

} // namespace blink

#endif // WebLayerTreeView_h
