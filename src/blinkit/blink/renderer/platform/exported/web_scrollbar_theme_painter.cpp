// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_scrollbar_theme_painter.cpp
// Description: WebScrollbarThemePainter Class
//      Author: Ziming Li
//     Created: 2021-08-05
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

#include "blinkit/blink/public/platform/web_scrollbar_theme_painter.h"

#include "blinkit/blink/renderer/platform/graphics/GraphicsContext.h"
#include "blinkit/blink/renderer/platform/graphics/paint/SkPictureBuilder.h"
#include "blinkit/blink/renderer/platform/scroll/Scrollbar.h"
#include "blinkit/blink/renderer/platform/scroll/ScrollbarTheme.h"
#include "third_party/skia/include/core/SkCanvas.h"

namespace blink {

WebScrollbarThemePainter::WebScrollbarThemePainter(void) : m_theme(0), m_deviceScaleFactor(1.0)
{
}

WebScrollbarThemePainter::WebScrollbarThemePainter(const WebScrollbarThemePainter &painter)
{
    assign(painter);
}

void WebScrollbarThemePainter::assign(const WebScrollbarThemePainter& painter)
{
    // This is a pointer to a static object, so no ownership transferral.
    m_theme = painter.m_theme;
    m_scrollbar = painter.m_scrollbar;
    m_deviceScaleFactor = painter.m_deviceScaleFactor;
}

void WebScrollbarThemePainter::reset()
{
    m_scrollbar.clear();
}

void WebScrollbarThemePainter::paintScrollbarBackground(WebCanvas* canvas, const IntRect& rect)
{
    SkRect clip = SkRect::MakeXYWH(rect.x(), rect.y(), rect.width(), rect.height());
    canvas->clipRect(clip);

    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintScrollbarBackground(pictureBuilder.context(), *m_scrollbar);
    pictureBuilder.endRecording()->playback(canvas);
}

void WebScrollbarThemePainter::paintTrackBackground(WebCanvas* canvas, const IntRect& rect)
{
    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintTrackBackground(pictureBuilder.context(), *m_scrollbar, rect);
    pictureBuilder.endRecording()->playback(canvas);
    if (!m_theme->shouldRepaintAllPartsOnInvalidation())
        m_scrollbar->clearTrackNeedsRepaint();
}

void WebScrollbarThemePainter::paintBackTrackPart(WebCanvas* canvas, const IntRect& rect)
{
    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintTrackPiece(pictureBuilder.context(), *m_scrollbar, rect, BackTrackPart);
    pictureBuilder.endRecording()->playback(canvas);
}

void WebScrollbarThemePainter::paintForwardTrackPart(WebCanvas* canvas, const IntRect& rect)
{
    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintTrackPiece(pictureBuilder.context(), *m_scrollbar, rect, ForwardTrackPart);
    pictureBuilder.endRecording()->playback(canvas);
}

void WebScrollbarThemePainter::paintBackButtonStart(WebCanvas* canvas, const IntRect& rect)
{
    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintButton(pictureBuilder.context(), *m_scrollbar, rect, BackButtonStartPart);
    pictureBuilder.endRecording()->playback(canvas);
}

void WebScrollbarThemePainter::paintBackButtonEnd(WebCanvas* canvas, const IntRect& rect)
{
    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintButton(pictureBuilder.context(), *m_scrollbar, rect, BackButtonEndPart);
    pictureBuilder.endRecording()->playback(canvas);
}

void WebScrollbarThemePainter::paintForwardButtonStart(WebCanvas* canvas, const IntRect& rect)
{
    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintButton(pictureBuilder.context(), *m_scrollbar, rect, ForwardButtonStartPart);
    pictureBuilder.endRecording()->playback(canvas);
}

void WebScrollbarThemePainter::paintForwardButtonEnd(WebCanvas* canvas, const IntRect& rect)
{
    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintButton(pictureBuilder.context(), *m_scrollbar, rect, ForwardButtonEndPart);
    pictureBuilder.endRecording()->playback(canvas);
}

void WebScrollbarThemePainter::paintTickmarks(WebCanvas* canvas, const IntRect& rect)
{
    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintTickmarks(pictureBuilder.context(), *m_scrollbar, rect);
    pictureBuilder.endRecording()->playback(canvas);
}

void WebScrollbarThemePainter::paintThumb(WebCanvas* canvas, const IntRect& rect)
{
    SkPictureBuilder pictureBuilder(rect);
    pictureBuilder.context().setDeviceScaleFactor(m_deviceScaleFactor);
    m_theme->paintThumb(pictureBuilder.context(), *m_scrollbar, rect);
    pictureBuilder.endRecording()->playback(canvas);
    if (!m_theme->shouldRepaintAllPartsOnInvalidation())
        m_scrollbar->clearThumbNeedsRepaint();
}

WebScrollbarThemePainter::WebScrollbarThemePainter(ScrollbarTheme& theme, Scrollbar& scrollbar, float deviceScaleFactor)
    : m_theme(&theme)
    , m_scrollbar(&scrollbar)
    , m_deviceScaleFactor(deviceScaleFactor)
{
}

float WebScrollbarThemePainter::thumbOpacity() const
{
    return m_theme->thumbOpacity(*m_scrollbar);
}

bool WebScrollbarThemePainter::trackNeedsRepaint() const
{
    return m_scrollbar->trackNeedsRepaint();
}

bool WebScrollbarThemePainter::thumbNeedsRepaint() const
{
    return m_scrollbar->thumbNeedsRepaint();
}

} // namespace blink
