// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScrollbarThemeOverlay.cpp
// Description: ScrollbarThemeOverlay Class
//      Author: Ziming Li
//     Created: 2021-07-31
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

#include "./ScrollbarThemeOverlay.h"

#include <algorithm>
#include "blinkit/blink/public/platform/Platform.h"
#include "blinkit/blink/public/platform/web_theme_engine.h"
#include "blinkit/blink/renderer/platform/PlatformMouseEvent.h"
#include "blinkit/blink/renderer/platform/graphics/GraphicsContext.h"
#include "blinkit/blink/renderer/platform/graphics/paint/DrawingRecorder.h"
#include "blinkit/blink/renderer/platform/scroll/ScrollbarThemeClient.h"
#include "blinkit/blink/renderer/platform/transforms/TransformationMatrix.h"
#include "blinkit/blink/renderer/wtf/MathExtras.h"

namespace blink {

ScrollbarThemeOverlay::ScrollbarThemeOverlay(int thumbThickness, int scrollbarMargin, HitTestBehavior allowHitTest, Color color)
    : ScrollbarTheme()
    , m_thumbThickness(thumbThickness)
    , m_scrollbarMargin(scrollbarMargin)
    , m_allowHitTest(allowHitTest)
    , m_color(color)
    , m_useSolidColor(true)
{
}

ScrollbarThemeOverlay::ScrollbarThemeOverlay(int thumbThickness, int scrollbarMargin, HitTestBehavior allowHitTest)
    : ScrollbarTheme()
    , m_thumbThickness(thumbThickness)
    , m_scrollbarMargin(scrollbarMargin)
    , m_allowHitTest(allowHitTest)
    , m_useSolidColor(false)
{
}

int ScrollbarThemeOverlay::scrollbarThickness(ScrollbarControlSize controlSize)
{
    return m_thumbThickness + m_scrollbarMargin;
}

int ScrollbarThemeOverlay::scrollbarMargin() const
{
    return m_scrollbarMargin;
}

bool ScrollbarThemeOverlay::usesOverlayScrollbars() const
{
    return true;
}

int ScrollbarThemeOverlay::thumbPosition(const ScrollbarThemeClient& scrollbar, float scrollPosition)
{
    if (!scrollbar.totalSize())
        return 0;

    int trackLen = trackLength(scrollbar);
    float proportion = static_cast<float>(scrollPosition) / scrollbar.totalSize();
    return round(proportion * trackLen);
}

int ScrollbarThemeOverlay::thumbLength(const ScrollbarThemeClient& scrollbar)
{
    int trackLen = trackLength(scrollbar);

    if (!scrollbar.totalSize())
        return trackLen;

    float proportion = static_cast<float>(scrollbar.visibleSize()) / scrollbar.totalSize();
    int length = round(proportion * trackLen);
    int minLen = std::min(minimumThumbLength(scrollbar), trackLen);
    length = clampTo(length, minLen, trackLen);
    return length;
}

bool ScrollbarThemeOverlay::hasThumb(const ScrollbarThemeClient& scrollbar)
{
    return true;
}

IntRect ScrollbarThemeOverlay::backButtonRect(const ScrollbarThemeClient&, ScrollbarPart, bool)
{
    return IntRect();
}

IntRect ScrollbarThemeOverlay::forwardButtonRect(const ScrollbarThemeClient&, ScrollbarPart, bool)
{
    return IntRect();
}

IntRect ScrollbarThemeOverlay::trackRect(const ScrollbarThemeClient& scrollbar, bool)
{
    IntRect rect = scrollbar.frameRect();
    if (scrollbar.orientation() == HorizontalScrollbar)
        rect.inflateX(-m_scrollbarMargin);
    else
        rect.inflateY(-m_scrollbarMargin);
    return rect;
}

int ScrollbarThemeOverlay::thumbThickness(const ScrollbarThemeClient&)
{
    return m_thumbThickness;
}

void ScrollbarThemeOverlay::paintThumb(GraphicsContext& context, const ScrollbarThemeClient& scrollbar, const IntRect& rect)
{
    if (DrawingRecorder::useCachedDrawingIfPossible(context, scrollbar, DisplayItem::ScrollbarThumb))
        return;

    DrawingRecorder recorder(context, scrollbar, DisplayItem::ScrollbarThumb, rect);

    IntRect thumbRect = rect;
    if (scrollbar.orientation() == HorizontalScrollbar) {
        thumbRect.setHeight(thumbRect.height() - m_scrollbarMargin);
    } else {
        thumbRect.setWidth(thumbRect.width() - m_scrollbarMargin);
        if (scrollbar.isLeftSideVerticalScrollbar())
            thumbRect.setX(thumbRect.x() + m_scrollbarMargin);
    }

    if (m_useSolidColor) {
        context.fillRect(thumbRect, m_color);
        return;
    }

    WebThemeEngine::State state = WebThemeEngine::StateNormal;
    if (scrollbar.pressedPart() == ThumbPart)
        state = WebThemeEngine::StatePressed;
    else if (scrollbar.hoveredPart() == ThumbPart)
        state = WebThemeEngine::StateHover;

    WebCanvas* canvas = context.canvas();

    WebThemeEngine::Part part = WebThemeEngine::PartScrollbarHorizontalThumb;
    if (scrollbar.orientation() == VerticalScrollbar)
        part = WebThemeEngine::PartScrollbarVerticalThumb;

    Platform::current()->themeEngine()->paint(canvas, part, state, rect, 0);
}

ScrollbarPart ScrollbarThemeOverlay::hitTest(const ScrollbarThemeClient& scrollbar, const IntPoint& position)
{
    if (m_allowHitTest == DisallowHitTest)
        return NoPart;

    return ScrollbarTheme::hitTest(scrollbar, position);
}

ScrollbarThemeOverlay& ScrollbarThemeOverlay::mobileTheme()
{
    DEFINE_STATIC_LOCAL(ScrollbarThemeOverlay, theme, (3, 3, ScrollbarThemeOverlay::DisallowHitTest, Color(128, 128, 128, 128)));
    return theme;
}

} // namespace blink
