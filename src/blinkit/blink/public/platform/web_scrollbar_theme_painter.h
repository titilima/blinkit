// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_scrollbar_theme_painter.h
// Description: WebScrollbarThemePainter Class
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

#ifndef WebScrollbarThemePainter_h
#define WebScrollbarThemePainter_h

#include "blinkit/blink/public/platform/WebCanvas.h"
#include "blinkit/gc/gc_def.h"

namespace blink {

class IntRect;
class ScrollbarTheme;
class Scrollbar;
class WebScrollbar;

class WebScrollbarThemePainter {
public:
    WebScrollbarThemePainter(void);
    WebScrollbarThemePainter(const WebScrollbarThemePainter &painter);
    virtual ~WebScrollbarThemePainter(void)
    {
        reset();
    }

    WebScrollbarThemePainter& operator=(const WebScrollbarThemePainter& painter)
    {
        assign(painter);
        return *this;
    }

    BLINK_PLATFORM_EXPORT void assign(const WebScrollbarThemePainter&);
    BLINK_PLATFORM_EXPORT void reset();

    BLINK_PLATFORM_EXPORT void paintScrollbarBackground(WebCanvas*, const IntRect&);
    BLINK_PLATFORM_EXPORT void paintTrackBackground(WebCanvas*, const IntRect&);
    BLINK_PLATFORM_EXPORT void paintBackTrackPart(WebCanvas*, const IntRect&);
    BLINK_PLATFORM_EXPORT void paintForwardTrackPart(WebCanvas*, const IntRect&);
    BLINK_PLATFORM_EXPORT void paintBackButtonStart(WebCanvas*, const IntRect&);
    BLINK_PLATFORM_EXPORT void paintBackButtonEnd(WebCanvas*, const IntRect&);
    BLINK_PLATFORM_EXPORT void paintForwardButtonStart(WebCanvas*, const IntRect&);
    BLINK_PLATFORM_EXPORT void paintForwardButtonEnd(WebCanvas*, const IntRect&);
    BLINK_PLATFORM_EXPORT void paintTickmarks(WebCanvas*, const IntRect&);
    BLINK_PLATFORM_EXPORT void paintThumb(WebCanvas*, const IntRect&);

    // This opacity is applied on top of the content that is painted for the thumb.
    BLINK_PLATFORM_EXPORT float thumbOpacity() const;

    BLINK_PLATFORM_EXPORT bool trackNeedsRepaint() const;
    BLINK_PLATFORM_EXPORT bool thumbNeedsRepaint() const;

#if INSIDE_BLINK
    BLINK_PLATFORM_EXPORT WebScrollbarThemePainter(ScrollbarTheme&, Scrollbar&, float deviceScaleFactor);
#endif

private:
    // The theme is not owned by this class. It is assumed that the theme is a
    // static pointer and its lifetime is essentially infinite. The functions
    // called from the painter may not be thread-safe, so all calls must be made
    // from the same thread that it is created on.
    ScrollbarTheme* m_theme;

    // It is assumed that the constructor of this paint object is responsible
    // for the lifetime of this scrollbar. The painter has to use the real
    // scrollbar (and not a WebScrollbar wrapper) due to static_casts for
    // LayoutScrollbar and pointer-based HashMap lookups for Lion scrollbars.
    GCRefPtr<Scrollbar> m_scrollbar;

    float m_deviceScaleFactor;
};

} // namespace blink

#endif
