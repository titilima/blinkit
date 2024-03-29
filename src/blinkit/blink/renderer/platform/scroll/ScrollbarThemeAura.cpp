// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScrollbarThemeAura.cpp
// Description: ScrollbarThemeAura Class
//      Author: Ziming Li
//     Created: 2021-08-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (c) 2008, 2009, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT{
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,{
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "./ScrollbarThemeAura.h"

#include "blinkit/blink/public/platform/Platform.h"
#include "blinkit/blink/public/platform/web_theme_engine.h"
#include "blinkit/blink/renderer/platform/LayoutTestSupport.h"
#include "blinkit/blink/renderer/platform/PlatformMouseEvent.h"
#include "blinkit/blink/renderer/platform/RuntimeEnabledFeatures.h"
#include "blinkit/blink/renderer/platform/graphics/GraphicsContext.h"
#include "blinkit/blink/renderer/platform/graphics/paint/DrawingRecorder.h"
#include "blinkit/blink/renderer/platform/scroll/ScrollbarThemeClient.h"
#include "blinkit/blink/renderer/platform/scroll/ScrollbarThemeOverlay.h"

namespace blink {

static bool useMockTheme()
{
    return LayoutTestSupport::isRunningLayoutTest();
}

ScrollbarTheme& ScrollbarTheme::nativeTheme()
{
    if (RuntimeEnabledFeatures::overlayScrollbarsEnabled()) {
        DEFINE_STATIC_LOCAL(ScrollbarThemeOverlay, theme, (10, 0, ScrollbarThemeOverlay::AllowHitTest));
        return theme;
    }

    DEFINE_STATIC_LOCAL(ScrollbarThemeAura, theme, ());
    return theme;
}

int ScrollbarThemeAura::scrollbarThickness(ScrollbarControlSize controlSize)
{
    // Horiz and Vert scrollbars are the same thickness.
    // In unit tests we don't have the mock theme engine (because of layering violations), so we hard code the size (see bug 327470).
    if (useMockTheme())
        return 15;
    IntSize scrollbarSize = Platform::current()->themeEngine()->GetViewportSize(WebThemeEngine::PartScrollbarVerticalTrack);
    return scrollbarSize.width();
}

void ScrollbarThemeAura::paintTrackPiece(GraphicsContext& gc, const ScrollbarThemeClient& scrollbar, const IntRect& rect, ScrollbarPart partType)
{
    DisplayItem::Type displayItemType = trackPiecePartToDisplayItemType(partType);
    if (DrawingRecorder::useCachedDrawingIfPossible(gc, scrollbar, displayItemType))
        return;

    DrawingRecorder recorder(gc, scrollbar, displayItemType, rect);

    WebThemeEngine::State state = scrollbar.hoveredPart() == partType ? WebThemeEngine::StateHover : WebThemeEngine::StateNormal;

    if (useMockTheme() && !scrollbar.enabled())
        state = WebThemeEngine::StateDisabled;

    IntRect alignRect = trackRect(scrollbar, false);
    WebThemeEngine::ExtraParams extraParams;
    extraParams.scrollbarTrack.isBack = (partType == BackTrackPart);
    extraParams.scrollbarTrack.trackX = alignRect.x();
    extraParams.scrollbarTrack.trackY = alignRect.y();
    extraParams.scrollbarTrack.trackWidth = alignRect.width();
    extraParams.scrollbarTrack.trackHeight = alignRect.height();
    Platform::current()->themeEngine()->paint(gc.canvas(), scrollbar.orientation() == HorizontalScrollbar ? WebThemeEngine::PartScrollbarHorizontalTrack : WebThemeEngine::PartScrollbarVerticalTrack, state, rect, &extraParams);
}

void ScrollbarThemeAura::paintButton(GraphicsContext& gc, const ScrollbarThemeClient& scrollbar, const IntRect& rect, ScrollbarPart part)
{
    WebThemeEngine::Part paintPart;
    WebThemeEngine::State state = WebThemeEngine::StateNormal;
    bool checkMin = false;
    bool checkMax = false;

    if (scrollbar.orientation() == HorizontalScrollbar) {
        if (part == BackButtonStartPart) {
            paintPart = WebThemeEngine::PartScrollbarLeftArrow;
            checkMin = true;
        } else if (useMockTheme() && part != ForwardButtonEndPart) {
            return;
        } else {
            paintPart = WebThemeEngine::PartScrollbarRightArrow;
            checkMax = true;
        }
    } else {
        if (part == BackButtonStartPart) {
            paintPart = WebThemeEngine::PartScrollbarUpArrow;
            checkMin = true;
        } else if (useMockTheme() && part != ForwardButtonEndPart) {
            return;
        } else {
            paintPart = WebThemeEngine::PartScrollbarDownArrow;
            checkMax = true;
        }
    }

    DisplayItem::Type displayItemType = buttonPartToDisplayItemType(part);
    if (DrawingRecorder::useCachedDrawingIfPossible(gc, scrollbar, displayItemType))
        return;

    DrawingRecorder recorder(gc, scrollbar, displayItemType, rect);

    if (useMockTheme() && !scrollbar.enabled()) {
        state = WebThemeEngine::StateDisabled;
    } else if (!useMockTheme() && ((checkMin && (scrollbar.currentPos() <= 0))
        || (checkMax && scrollbar.currentPos() >= scrollbar.maximum()))) {
        state = WebThemeEngine::StateDisabled;
    } else {
        if (part == scrollbar.pressedPart())
            state = WebThemeEngine::StatePressed;
        else if (part == scrollbar.hoveredPart())
            state = WebThemeEngine::StateHover;
    }
    Platform::current()->themeEngine()->paint(gc.canvas(), paintPart, state, rect, 0);
}

void ScrollbarThemeAura::paintThumb(GraphicsContext& gc, const ScrollbarThemeClient& scrollbar, const IntRect& rect)
{
    if (DrawingRecorder::useCachedDrawingIfPossible(gc, scrollbar, DisplayItem::ScrollbarThumb))
        return;

    DrawingRecorder recorder(gc, scrollbar, DisplayItem::ScrollbarThumb, rect);

    WebThemeEngine::State state;
    WebCanvas* canvas = gc.canvas();
    if (scrollbar.pressedPart() == ThumbPart)
        state = WebThemeEngine::StatePressed;
    else if (scrollbar.hoveredPart() == ThumbPart)
        state = WebThemeEngine::StateHover;
    else
        state = WebThemeEngine::StateNormal;
    Platform::current()->themeEngine()->paint(canvas, scrollbar.orientation() == HorizontalScrollbar ? WebThemeEngine::PartScrollbarHorizontalThumb : WebThemeEngine::PartScrollbarVerticalThumb, state, rect, 0);
}

static void ScaleButton(IntSize &size, int viewportThickness, int thickness)
{
    if (thickness != viewportThickness)
    {
        float scale = static_cast<float>(thickness) / static_cast<float>(viewportThickness);
        size.scale(scale);
    }
}

IntSize ScrollbarThemeAura::buttonSize(const ScrollbarThemeClient& scrollbar)
{
    WebThemeEngine *themeEngine = Platform::current()->themeEngine();

    int viewportThickness = scrollbarThickness(scrollbar.controlSize());
    if (scrollbar.orientation() == VerticalScrollbar) {
        IntSize size = themeEngine->GetViewportSize(WebThemeEngine::PartScrollbarUpArrow);
        ScaleButton(size, viewportThickness, scrollbar.size().width());
        return IntSize(size.width(), scrollbar.height() < 2 * size.height() ? scrollbar.height() / 2 : size.height());
    }

    // HorizontalScrollbar
    IntSize size = Platform::current()->themeEngine()->GetViewportSize(WebThemeEngine::PartScrollbarLeftArrow);
    ScaleButton(size, viewportThickness, scrollbar.size().height());
    return IntSize(scrollbar.width() < 2 * size.width() ? scrollbar.width() / 2 : size.width(), size.height());
}

int ScrollbarThemeAura::minimumThumbLength(const ScrollbarThemeClient& scrollbar)
{
    if (scrollbar.orientation() == VerticalScrollbar) {
        IntSize size = Platform::current()->themeEngine()->GetViewportSize(WebThemeEngine::PartScrollbarVerticalThumb);
        return size.height();
    }

    IntSize size = Platform::current()->themeEngine()->GetViewportSize(WebThemeEngine::PartScrollbarHorizontalThumb);
    return size.width();
}

} // namespace blink
