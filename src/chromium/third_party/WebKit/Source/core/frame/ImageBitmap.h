// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ImageBitmap.h
// Description: ImageBitmap Class
//      Author: Ziming Li
//     Created: 2019-02-09
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ImageBitmap_h
#define ImageBitmap_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "core/CoreExport.h"
#include "core/html/HTMLImageElement.h"
#include "core/imagebitmap/ImageBitmapSource.h"
#include "platform/geometry/IntRect.h"
#include "platform/graphics/Image.h"
#include "platform/graphics/ImageBuffer.h"
#include "platform/graphics/StaticBitmapImage.h"
#include "platform/heap/Handle.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"

namespace blink {

class ImageData;

class CORE_EXPORT ImageBitmap final : public RefCountedWillBeGarbageCollectedFinalized<ImageBitmap>, public ScriptWrappable, public ImageLoaderClient, public ImageBitmapSource {
    DEFINE_WRAPPERTYPEINFO();
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(ImageBitmap);
public:
    static PassRefPtrWillBeRawPtr<ImageBitmap> create(HTMLImageElement*, const IntRect&, Document*);
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

    // ImageBitmapSource implementation
    IntSize bitmapSourceSize() const override { return size(); }

    DECLARE_VIRTUAL_TRACE();

private:
    ImageBitmap(HTMLImageElement*, const IntRect&, Document*);
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
