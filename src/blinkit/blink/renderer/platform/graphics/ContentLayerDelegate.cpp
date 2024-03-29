// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ContentLayerDelegate.cpp
// Description: ContentLayerDelegate Class
//      Author: Ziming Li
//     Created: 2021-07-28
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "./ContentLayerDelegate.h"

#include "blinkit/blink/public/platform/WebDisplayItemList.h"
#include "blinkit/blink/renderer/platform/RuntimeEnabledFeatures.h"
#include "blinkit/blink/renderer/platform/TraceEvent.h"
#include "blinkit/blink/renderer/platform/TracedValue.h"
#include "blinkit/blink/renderer/platform/graphics/GraphicsContext.h"
#include "blinkit/blink/renderer/platform/graphics/GraphicsLayer.h"
#include "blinkit/blink/renderer/platform/graphics/paint/PaintArtifactToSkCanvas.h"
#include "blinkit/blink/renderer/platform/graphics/paint/PaintController.h"
#include "third_party/skia/include/core/SkPicture.h"

namespace blink {

ContentLayerDelegate::ContentLayerDelegate(GraphicsLayer* graphicsLayer)
    : m_graphicsLayer(graphicsLayer)
{
}

ContentLayerDelegate::~ContentLayerDelegate()
{
}

static void paintArtifactToWebDisplayItemList(WebDisplayItemList* list, const PaintArtifact& artifact, const IntRect& bounds)
{
    if (RuntimeEnabledFeatures::slimmingPaintV2Enabled()) {
        // This is a temporary path to paint the artifact using the paint chunk
        // properties. Ultimately, we should instead split the artifact into
        // separate layers and send those to the compositor, instead of sending
        // one big flat SkPicture.
        SkRect skBounds = SkRect::MakeXYWH(bounds.x(), bounds.y(), bounds.width(), bounds.height());
        RefPtr<SkPicture> picture = paintArtifactToSkPicture(artifact, skBounds);
        list->appendDrawingItem(bounds, picture.get());
        return;
    }
    artifact.appendToWebDisplayItemList(list);
}

IntRect ContentLayerDelegate::paintableRegion()
{
    return m_graphicsLayer->interestRect();
}

void ContentLayerDelegate::paintContents(
    WebDisplayItemList* webDisplayItemList, WebContentLayerClient::PaintingControlSetting paintingControl)
{
    TRACE_EVENT0("blink,benchmark", "ContentLayerDelegate::paintContents");

    PaintController& paintController = m_graphicsLayer->paintController();
    paintController.setDisplayItemConstructionIsDisabled(
        paintingControl == WebContentLayerClient::DisplayListConstructionDisabled);
    paintController.setSubsequenceCachingIsDisabled(
        paintingControl == WebContentLayerClient::SubsequenceCachingDisabled);

    // We also disable caching when Painting or Construction are disabled. In both cases we would like
    // to compare assuming the full cost of recording, not the cost of re-using cached content.
    if (paintingControl != WebContentLayerClient::PaintDefaultBehavior
        && paintingControl != WebContentLayerClient::SubsequenceCachingDisabled)
        paintController.invalidateAll();

    GraphicsContext::DisabledMode disabledMode = GraphicsContext::NothingDisabled;
    if (paintingControl == WebContentLayerClient::DisplayListPaintingDisabled
        || paintingControl == WebContentLayerClient::DisplayListConstructionDisabled)
        disabledMode = GraphicsContext::FullyDisabled;

    if (paintingControl != PaintDefaultBehavior)
        m_graphicsLayer->paint(nullptr, disabledMode);
    paintArtifactToWebDisplayItemList(webDisplayItemList, paintController.paintArtifact(), paintableRegion());

    paintController.setDisplayItemConstructionIsDisabled(false);
    paintController.setSubsequenceCachingIsDisabled(false);
}

size_t ContentLayerDelegate::approximateUnsharedMemoryUsage() const
{
    return m_graphicsLayer->paintController().approximateUnsharedMemoryUsage();
}

} // namespace blink
