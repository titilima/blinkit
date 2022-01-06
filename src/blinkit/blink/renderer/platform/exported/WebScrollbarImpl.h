// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebScrollbarImpl.h
// Description: WebScrollbarImpl Class
//      Author: Ziming Li
//     Created: 2021-07-27
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

#ifndef WebScrollbarImpl_h
#define WebScrollbarImpl_h

#include "blinkit/blink/public/platform/web_scrollbar.h"
#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"

namespace blink {

class Scrollbar;

class WebScrollbarImpl final : public WebScrollbar
{
    WTF_MAKE_NONCOPYABLE(WebScrollbarImpl);
public:
    static std::unique_ptr<WebScrollbarImpl> create(Scrollbar* scrollbar)
    {
        return zed::wrap_unique(new WebScrollbarImpl(scrollbar));
    }

    // Implement WebScrollbar methods
    bool isOverlay() const override;
    int value() const override;
    IntPoint location() const override;
    IntSize size() const override;
    bool enabled() const override;
    int maximum() const override;
    int totalSize() const override;
    bool isScrollableAreaActive() const override;
    void getTickmarks(std::vector<IntRect>& tickmarks) const override;
    ScrollbarControlSize controlSize() const override;
    ScrollbarPart pressedPart() const override;
    ScrollbarPart hoveredPart() const override;
    ScrollbarOverlayStyle scrollbarOverlayStyle() const override;
    bool isCustomScrollbar() const override;
    Orientation orientation() const override;
    bool isLeftSideVerticalScrollbar() const override;
    float elasticOverscroll() const override;
    void setElasticOverscroll(float) override;

private:
    explicit WebScrollbarImpl(Scrollbar*);

    GCRefPtr<Scrollbar> m_scrollbar;
};

} // namespace blink

#endif
