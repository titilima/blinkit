// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScrollbarThemeNonMacCommon.cpp
// Description: ScrollbarThemeNonMacCommon Class
//      Author: Ziming Li
//     Created: 2021-08-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 * Copyright (C) 2008, 2009 Google Inc.
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
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "platform/scroll/ScrollbarThemeNonMacCommon.h"

#include "platform/PlatformMouseEvent.h"
#include "platform/graphics/GraphicsContextStateSaver.h"
#include "platform/graphics/paint/DrawingRecorder.h"
#include "platform/scroll/ScrollableArea.h"

namespace blink {

bool ScrollbarThemeNonMacCommon::hasThumb(const ScrollbarThemeClient& scrollbar)
{
    // This method is just called as a paint-time optimization to see if
    // painting the thumb can be skipped. We don't have to be exact here.
    return thumbLength(scrollbar) > 0;
}

IntRect ScrollbarThemeNonMacCommon::backButtonRect(const ScrollbarThemeClient& scrollbar, ScrollbarPart part, bool)
{
    // Windows and Linux just have single arrows.
    if (part == BackButtonEndPart)
        return IntRect();

    IntSize size = buttonSize(scrollbar);
    return IntRect(scrollbar.x(), scrollbar.y(), size.width(), size.height());
}

IntRect ScrollbarThemeNonMacCommon::forwardButtonRect(const ScrollbarThemeClient& scrollbar, ScrollbarPart part, bool)
{
    // Windows and Linux just have single arrows.
    if (part == ForwardButtonStartPart)
        return IntRect();

    IntSize size = buttonSize(scrollbar);
    int x, y;
    if (scrollbar.orientation() == HorizontalScrollbar) {
        x = scrollbar.x() + scrollbar.width() - size.width();
        y = scrollbar.y();
    } else {
        x = scrollbar.x();
        y = scrollbar.y() + scrollbar.height() - size.height();
    }
    return IntRect(x, y, size.width(), size.height());
}

IntRect ScrollbarThemeNonMacCommon::trackRect(const ScrollbarThemeClient& scrollbar, bool)
{
    // The track occupies all space between the two buttons.
    IntSize bs = buttonSize(scrollbar);
    if (scrollbar.orientation() == HorizontalScrollbar) {
        if (scrollbar.width() <= 2 * bs.width())
            return IntRect();
        return IntRect(scrollbar.x() + bs.width(), scrollbar.y(), scrollbar.width() - 2 * bs.width(), scrollbar.height());
    }
    if (scrollbar.height() <= 2 * bs.height())
        return IntRect();
    return IntRect(scrollbar.x(), scrollbar.y() + bs.height(), scrollbar.width(), scrollbar.height() - 2 * bs.height());
}

void ScrollbarThemeNonMacCommon::paintTrackBackground(GraphicsContext& context, const ScrollbarThemeClient& scrollbar, const IntRect& rect)
{
    // Just assume a forward track part. We only paint the track as a single piece when there is no thumb.
    if (!hasThumb(scrollbar))
        paintTrackPiece(context, scrollbar, rect, ForwardTrackPart);
}

void ScrollbarThemeNonMacCommon::paintTickmarks(GraphicsContext& context, const ScrollbarThemeClient& scrollbar, const IntRect& rect)
{
    if (scrollbar.orientation() != VerticalScrollbar)
        return;

    if (rect.height() <= 0 || rect.width() <= 0)
        return;

    // Get the tickmarks for the frameview.
    std::vector<IntRect> tickmarks;
    scrollbar.getTickmarks(tickmarks);
    if (tickmarks.empty())
        return;

    if (DrawingRecorder::useCachedDrawingIfPossible(context, scrollbar, DisplayItem::ScrollbarTickmarks))
        return;

    DrawingRecorder recorder(context, scrollbar, DisplayItem::ScrollbarTickmarks, rect);
    GraphicsContextStateSaver stateSaver(context);
    context.setShouldAntialias(false);

    for (auto i = tickmarks.begin(); i != tickmarks.end(); ++i) {
        // Calculate how far down (in %) the tick-mark should appear.
        const float percent = static_cast<float>(i->y()) / scrollbar.totalSize();

        // Calculate how far down (in pixels) the tick-mark should appear.
        const int yPos = rect.y() + (rect.height() * percent);

        FloatRect tickRect(rect.x(), yPos, rect.width(), 3);
        context.fillRect(tickRect, Color(0xCC, 0xAA, 0x00, 0xFF));

        FloatRect tickStroke(rect.x(), yPos + 1, rect.width(), 1);
        context.fillRect(tickStroke, Color(0xFF, 0xDD, 0x00, 0xFF));
    }
}

} // namespace blink
