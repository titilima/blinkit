// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ImageBitmap.h
// Description: ImageBitmap Class
//      Author: Ziming Li
//     Created: 2021-07-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ImageBitmap_h
#define ImageBitmap_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "core/CoreExport.h"
#include "core/html/HTMLImageElement.h"
// BKTODO: #include "core/html/canvas/CanvasImageSource.h"
#include "core/imagebitmap/ImageBitmapSource.h"
#include "platform/geometry/IntRect.h"
#include "platform/graphics/Image.h"
#include "platform/graphics/ImageBuffer.h"
#include "platform/graphics/StaticBitmapImage.h"
#include "platform/heap/Handle.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"

namespace blink {

class HTMLCanvasElement;
class HTMLVideoElement;
class ImageData;

class CORE_EXPORT ImageBitmap final : public RefCountedWillBeGarbageCollectedFinalized<ImageBitmap>, public ScriptWrappable, public ImageLoaderClient, public ImageBitmapSource { // TODO: , public CanvasImageSource
    DEFINE_WRAPPERTYPEINFO();
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(ImageBitmap);
public:
    static PassRefPtrWillBeRawPtr<ImageBitmap> create(HTMLImageElement*, const IntRect&, Document*);
#if 0 // BKTODO:
    static PassRefPtrWillBeRawPtr<ImageBitmap> create(HTMLVideoElement*, const IntRect&, Document*);
    static PassRefPtrWillBeRawPtr<ImageBitmap> create(HTMLCanvasElement*, const IntRect&);
#endif
    static PassRefPtrWillBeRawPtr<ImageBitmap> create(ImageData*, const IntRect&);
    static PassRefPtrWillBeRawPtr<ImageBitmap> create(ImageBitmap*, const IntRect&);
    static PassRefPtrWillBeRawPtr<ImageBitmap> create(PassRefPtr<StaticBitmapImage>);
    static PassRefPtrWillBeRawPtr<ImageBitmap> create(PassRefPtr<StaticBitmapImage>, const IntRect&);

    StaticBitmapImage* bitmapImage() const { return (m_image) ? m_image.get() : nullptr; }
    unsigned long width() const;
    unsigned long height() const;
    IntSize size() const;

    bool isNeutered() const { return m_isNeutered; }
    bool originClean() const { return m_image->originClean(); }
    PassRefPtr<StaticBitmapImage> transfer();

    ~ImageBitmap() override;

#if 0 // BKTODO:
    // CanvasImageSource implementation
    PassRefPtr<Image> getSourceImageForCanvas(SourceImageStatus*, AccelerationHint) const override;
    bool wouldTaintOrigin(SecurityOrigin*) const override { return !m_image->originClean(); }
    void adjustDrawRects(FloatRect* srcRect, FloatRect* dstRect) const override;
    FloatSize elementSize() const override;
#endif

    // ImageBitmapSource implementation
    IntSize bitmapSourceSize() const override { return size(); }
    // BKTODO: ScriptPromise createImageBitmap(ScriptState*, EventTarget&, int sx, int sy, int sw, int sh, ExceptionState&) override;

    DECLARE_VIRTUAL_TRACE();

private:
    ImageBitmap(HTMLImageElement*, const IntRect&, Document*);
#if 0 // BKTODO:
    ImageBitmap(HTMLVideoElement*, const IntRect&, Document*);
    ImageBitmap(HTMLCanvasElement*, const IntRect&);
#endif
    ImageBitmap(ImageData*, const IntRect&);
    ImageBitmap(ImageBitmap*, const IntRect&);
    ImageBitmap(PassRefPtr<StaticBitmapImage>);
    ImageBitmap(PassRefPtr<StaticBitmapImage>, const IntRect&);

    // ImageLoaderClient
    void notifyImageSourceChanged() override;
    bool requestsHighLiveResourceCachePriority() override { return true; }

    RefPtr<StaticBitmapImage> m_image;
    bool m_isNeutered = false;
};

} // namespace blink

#endif // ImageBitmap_h
