#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CrossfadeGeneratedImage.h
// Description: CrossfadeGeneratedImage Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CrossfadeGeneratedImage_h
#define CrossfadeGeneratedImage_h

#include "blinkit/blink/renderer/platform/graphics/GeneratedImage.h"
#include "blinkit/blink/renderer/platform/graphics/ImageObserver.h"
#include "wtf/RefPtr.h"

namespace blink {

class CrossfadeGeneratedImage final : public GeneratedImage
{
public:
    static PassRefPtr<CrossfadeGeneratedImage> create(PassRefPtr<Image> fromImage, PassRefPtr<Image> toImage, float percentage, IntSize crossfadeSize, const IntSize& size)
    {
        return adoptRef(new CrossfadeGeneratedImage(fromImage, toImage, percentage, crossfadeSize, size));
    }

    bool usesContainerSize() const override { return false; }
    bool hasRelativeWidth() const override { return false; }
    bool hasRelativeHeight() const override { return false; }

    IntSize size() const override { return m_crossfadeSize; }

protected:
    void draw(SkCanvas*, const SkPaint&, const FloatRect&, const FloatRect&, RespectImageOrientationEnum, ImageClampingMode) override;
    void drawTile(GraphicsContext&, const FloatRect&) final;

    CrossfadeGeneratedImage(PassRefPtr<Image> fromImage, PassRefPtr<Image> toImage, float percentage, IntSize crossfadeSize, const IntSize&);

private:
    void drawCrossfade(SkCanvas*, const SkPaint&, ImageClampingMode);

    RefPtr<Image> m_fromImage;
    RefPtr<Image> m_toImage;

    float m_percentage;
    IntSize m_crossfadeSize;
};

} // namespace blink

#endif
