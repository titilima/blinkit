// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebCompositorSupport.h
// Description: WebCompositorSupport Class
//      Author: Ziming Li
//     Created: 2021-07-17
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
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

#ifndef WebCompositorSupport_h
#define WebCompositorSupport_h

#include "blinkit/blink/public/platform/WebCommon.h"
#include "blinkit/blink/public/platform/WebCompositorAnimation.h"
#include "blinkit/blink/public/platform/WebCompositorAnimationCurve.h"
#include "blinkit/blink/public/platform/web_layer_tree_view.h"
#include "blinkit/blink/public/platform/WebScrollOffsetAnimationCurve.h"
#include "blinkit/blink/public/platform/web_scrollbar.h"
#include "blinkit/blink/public/platform/web_scrollbar_theme_painter.h"
#include "blinkit/blink/renderer/platform/geometry/FloatPoint.h"

namespace cc {
class Layer;
}

namespace blink {

class WebCompositorAnimationPlayer;
class WebCompositorAnimationTimeline;
class WebContentLayer;
class WebContentLayerClient;
class WebExternalTextureLayer;
class WebExternalTextureLayerClient;
class WebFilterAnimationCurve;
class WebFloatAnimationCurve;
class WebGraphicsContext3D;
class WebImageLayer;
class WebLayer;
class WebScrollbarLayer;
class WebScrollbarThemeGeometry;
class WebTransformAnimationCurve;
class WebTransformOperations;

class WebCompositorSupport {
public:

#if 0 // BKTODO:
    // Layers -------------------------------------------------------

    virtual WebLayer* createLayer() { return nullptr; }

    virtual WebLayer* createLayerFromCCLayer(cc::Layer*) { return nullptr; }
#endif

    virtual WebContentLayer* createContentLayer(WebContentLayerClient *) { return nullptr; }

#if 0 // BKTODO:
    virtual WebExternalTextureLayer* createExternalTextureLayer(WebExternalTextureLayerClient*) { return nullptr; }

    virtual WebImageLayer* createImageLayer() { return nullptr; }
#endif

    // The ownership of the WebScrollbarThemeGeometry pointer is passed to Chromium.
    virtual WebScrollbarLayer* createScrollbarLayer(std::unique_ptr<WebScrollbar> &&, WebScrollbarThemePainter, std::unique_ptr<WebScrollbarThemeGeometry> &) { return nullptr; }

    virtual WebScrollbarLayer* createSolidColorScrollbarLayer(WebScrollbar::Orientation, int thumbThickness, int trackStart, bool isLeftSideVerticalScrollbar) { return nullptr; }

#if 0 // BKTODO:
    // Animation ----------------------------------------------------

    virtual WebCompositorAnimation* createAnimation(const WebCompositorAnimationCurve&, WebCompositorAnimation::TargetProperty, int groupId = 0, int animationId = 0) { return nullptr; }

    virtual WebFilterAnimationCurve* createFilterAnimationCurve() { return nullptr; }

    virtual WebFloatAnimationCurve* createFloatAnimationCurve() { return nullptr; }

    virtual WebScrollOffsetAnimationCurve* createScrollOffsetAnimationCurve(
        FloatPoint targetValue,
        WebCompositorAnimationCurve::TimingFunctionType,
        WebScrollOffsetAnimationCurve::ScrollDurationBehavior) { return nullptr; }

    virtual WebTransformAnimationCurve* createTransformAnimationCurve() { return nullptr; }

    virtual WebTransformOperations* createTransformOperations() { return nullptr; }

    virtual WebCompositorAnimationPlayer* createAnimationPlayer() { return nullptr; }

    virtual WebCompositorAnimationTimeline* createAnimationTimeline() { return nullptr; }
#endif

protected:
    virtual ~WebCompositorSupport() { }
};

}

#endif // WebCompositorSupport_h
