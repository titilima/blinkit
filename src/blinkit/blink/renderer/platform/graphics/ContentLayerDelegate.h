#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ContentLayerDelegate.h
// Description: ContentLayerDelegate Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
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

#ifndef ContentLayerDelegate_h
#define ContentLayerDelegate_h

#include "blinkit/blink/public/platform/WebContentLayerClient.h"
#include "blinkit/blink/renderer/platform/geometry/int_size.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"
#include "blinkit/blink/renderer/wtf/PassOwnPtr.h"

class SkCanvas;

namespace blink {

class GraphicsLayer;

class ContentLayerDelegate : public WebContentLayerClient
{
    WTF_MAKE_NONCOPYABLE(ContentLayerDelegate);
public:
    explicit ContentLayerDelegate(GraphicsLayer*);
    ~ContentLayerDelegate() override;

    IntRect paintableRegion() override;

    // WebContentLayerClient implementation.
    void paintContents(WebDisplayItemList*, WebContentLayerClient::PaintingControlSetting = PaintDefaultBehavior) override;
    size_t approximateUnsharedMemoryUsage() const override;

private:
    GraphicsLayer* m_graphicsLayer;
};

} // namespace blink

#endif // ContentLayerDelegate_h
