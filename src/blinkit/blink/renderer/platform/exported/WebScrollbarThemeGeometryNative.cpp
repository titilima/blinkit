// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebScrollbarThemeGeometryNative.cpp
// Description: WebScrollbarThemeGeometryNative Class
//      Author: Ziming Li
//     Created: 2021-07-31
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

#include "platform/exported/WebScrollbarThemeGeometryNative.h"

#include "blinkit/blink/public/platform/web_scrollbar.h"
#include "blinkit/blink/renderer/platform/exported/WebScrollbarThemeClientImpl.h"
#include "blinkit/blink/renderer/platform/scroll/ScrollbarTheme.h"
#include "third_party/zed/include/zed/memory.hpp"

namespace blink {

std::unique_ptr<WebScrollbarThemeGeometryNative> WebScrollbarThemeGeometryNative::create(ScrollbarTheme& theme)
{
    return zed::wrap_unique(new WebScrollbarThemeGeometryNative(theme));
}

WebScrollbarThemeGeometryNative::WebScrollbarThemeGeometryNative(ScrollbarTheme& theme)
    : m_theme(theme)
{
}

bool WebScrollbarThemeGeometryNative::hasButtons(WebScrollbar* scrollbar)
{
    return m_theme.hasButtons(WebScrollbarThemeClientImpl(*scrollbar));
}

bool WebScrollbarThemeGeometryNative::hasThumb(WebScrollbar* scrollbar)
{
    return m_theme.hasThumb(WebScrollbarThemeClientImpl(*scrollbar));
}

IntRect WebScrollbarThemeGeometryNative::trackRect(WebScrollbar* scrollbar)
{
    return m_theme.trackRect(WebScrollbarThemeClientImpl(*scrollbar));
}

IntRect WebScrollbarThemeGeometryNative::thumbRect(WebScrollbar* scrollbar)
{
    return m_theme.thumbRect(WebScrollbarThemeClientImpl(*scrollbar));
}

IntRect WebScrollbarThemeGeometryNative::backButtonStartRect(WebScrollbar* scrollbar)
{
    return m_theme.backButtonRect(WebScrollbarThemeClientImpl(*scrollbar), BackButtonStartPart, false);
}

IntRect WebScrollbarThemeGeometryNative::backButtonEndRect(WebScrollbar* scrollbar)
{
    return m_theme.backButtonRect(WebScrollbarThemeClientImpl(*scrollbar), BackButtonEndPart, false);
}

IntRect WebScrollbarThemeGeometryNative::forwardButtonStartRect(WebScrollbar* scrollbar)
{
    return m_theme.forwardButtonRect(WebScrollbarThemeClientImpl(*scrollbar), ForwardButtonStartPart, false);
}

IntRect WebScrollbarThemeGeometryNative::forwardButtonEndRect(WebScrollbar* scrollbar)
{
    return m_theme.forwardButtonRect(WebScrollbarThemeClientImpl(*scrollbar), ForwardButtonEndPart, false);
}

} // namespace blink
