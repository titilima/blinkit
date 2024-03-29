#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: GradientGeneratedImage.h
// Description: GradientGeneratedImage Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008, 2012 Apple Computer, Inc.  All rights reserved.
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

#ifndef GradientGeneratedImage_h
#define GradientGeneratedImage_h

#include "blinkit/blink/renderer/platform/graphics/GeneratedImage.h"
#include "blinkit/blink/renderer/platform/graphics/Gradient.h"
#include "blinkit/blink/renderer/platform/graphics/ImageBuffer.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"

namespace blink {

class GradientGeneratedImage : public GeneratedImage
{
public:
    static PassRefPtr<GradientGeneratedImage> create(PassRefPtr<Gradient> generator, const IntSize& size)
    {
        return adoptRef(new GradientGeneratedImage(generator, size));
    }

    ~GradientGeneratedImage() override {}

protected:
    void draw(SkCanvas*, const SkPaint&, const FloatRect&, const FloatRect&, RespectImageOrientationEnum, ImageClampingMode) override;
    void drawTile(GraphicsContext&, const FloatRect&) final;

    GradientGeneratedImage(PassRefPtr<Gradient> generator, const IntSize& size)
        : GeneratedImage(size)
        , m_gradient(generator)
    {
    }

    RefPtr<Gradient> m_gradient;
};

} // namespace blink

#endif
