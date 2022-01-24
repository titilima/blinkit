#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ImageBufferSurface.h
// Description: ImageBufferSurface Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (c) 2013, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ImageBufferSurface_h
#define ImageBufferSurface_h

#include "blinkit/blink/renderer/platform/geometry/int_size.h"
#include "blinkit/blink/renderer/platform/graphics/GraphicsTypes.h"
#include "blinkit/blink/renderer/platform/graphics/GraphicsTypes3D.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"
#include "blinkit/blink/renderer/wtf/PassRefPtr.h"
#include "third_party/skia/include/core/SkPaint.h"

class SkBitmap;
class SkCanvas;
class SkImage;
struct SkImageInfo;
class SkPicture;

namespace blink {

class ImageBuffer;
class WebLayer;
class FloatRect;
class GraphicsContext;

class ImageBufferSurface
{
    WTF_MAKE_NONCOPYABLE(ImageBufferSurface);
public:
    virtual ~ImageBufferSurface();

    virtual SkCanvas* canvas() = 0;
    virtual void disableDeferral() { }
    virtual void willOverwriteCanvas() { }
    virtual void didDraw(const FloatRect& rect) { }
    virtual bool isValid() const = 0;
    virtual bool restore() { return false; }
    virtual WebLayer* layer() const { return 0; }
    virtual bool isAccelerated() const { return false; }
    virtual bool isRecording() const { return false; }
    virtual bool isExpensiveToPaint() { return false; }
    virtual void setFilterQuality(SkFilterQuality) { }
    virtual void setIsHidden(bool) { }
    virtual void setImageBuffer(ImageBuffer*) { }
    virtual PassRefPtr<SkPicture> getPicture();
    virtual void finalizeFrame(const FloatRect &dirtyRect) { }
    virtual void draw(GraphicsContext&, const FloatRect& destRect, const FloatRect& srcRect, SkXfermode::Mode);
    virtual void setHasExpensiveOp() { }
    virtual Platform3DObject getBackingTextureHandleForOverwrite() { return 0; }
    virtual void flush(); // Execute all deferred rendering immediately
    virtual void flushGpu() { flush(); } // Like flush, but flushes all the way down to the GPU context if the surface uses the GPU
    virtual void prepareSurfaceForPaintingIfNeeded() { }
    virtual bool writePixels(const SkImageInfo& origInfo, const void* pixels, size_t rowBytes, int x, int y);

    // May return nullptr if the surface is GPU-backed and the GPU context was lost.
    virtual PassRefPtr<SkImage> newImageSnapshot(AccelerationHint) = 0;

    OpacityMode opacityMode() const { return m_opacityMode; }
    const IntSize& size() const { return m_size; }
    void notifyIsValidChanged(bool isValid) const;

protected:
    ImageBufferSurface(const IntSize&, OpacityMode);
    void clear();

private:
    OpacityMode m_opacityMode;
    IntSize m_size;
};

} // namespace blink

#endif
