// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_scrollbar_theme_geometry.h
// Description: WebScrollbarThemeGeometry Class
//      Author: Ziming Li
//     Created: 2021-07-20
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

#ifndef WebScrollbarThemeGeometry_h
#define WebScrollbarThemeGeometry_h

#include "blinkit/blink/renderer/platform/geometry/int_rect.h"

namespace blink {

class WebScrollbar;

class WebScrollbarThemeGeometry
{
public:
    virtual ~WebScrollbarThemeGeometry() { }

    virtual bool hasButtons(WebScrollbar*) = 0;
    virtual bool hasThumb(WebScrollbar*) = 0;
    virtual IntRect trackRect(WebScrollbar*) = 0;
    virtual IntRect thumbRect(WebScrollbar*) = 0;
    virtual IntRect backButtonStartRect(WebScrollbar*) = 0;
    virtual IntRect backButtonEndRect(WebScrollbar*) = 0;
    virtual IntRect forwardButtonStartRect(WebScrollbar*) = 0;
    virtual IntRect forwardButtonEndRect(WebScrollbar*) = 0;
};

} // namespace blink

#endif
