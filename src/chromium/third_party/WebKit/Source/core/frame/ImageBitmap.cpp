// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ImageBitmap.cpp
// Description: ImageBitmap Class
//      Author: Ziming Li
//     Created: 2019-02-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/frame/ImageBitmap.h"

#include "third_party/skia/include/core/SkSurface.h"
#include "wtf/RefPtr.h"

namespace blink {

static inline IntRect normalizeRect(const IntRect& rect)
{
    return IntRect(std::min(rect.x(), rect.maxX()),
        std::min(rect.y(), rect.maxY()),
        std::max(rect.width(), -rect.width()),
        std::max(rect.height(), -rect.height()));
}

static PassRefPtr<StaticBitmapImage> cropImage(Image* image, const IntRect& cropRect)
{
    ASSERT(image);

    IntRect imgRect(IntPoint(), IntSize(image->width(), image->height()));
    const IntRect srcRect = intersection(imgRect, cropRect);

    if (cropRect == srcRect)
        return StaticBitmapImage::create(adoptRef(image->imageForCurrentFrame()->newSubset(srcRect)));

    RefPtr<SkSurface> surface = adoptRef(SkSurface::NewRasterN32Premul(cropRect.width(), cropRect.height()));

    if (srcRect.isEmpty())
        return StaticBitmapImage::create(adoptRef(surface->newImageSnapshot()));

    SkScalar dstLeft = std::min(0, -cropRect.x());
    SkScalar dstTop = std::min(0, -cropRect.y());
    if (cropRect.x() < 0)
        dstLeft = -cropRect.x();
    if (cropRect.y() < 0)
        dstTop = -cropRect.y();
    surface->getCanvas()->drawImage(image->imageForCurrentFrame().get(), dstLeft, dstTop);
    return StaticBitmapImage::create(adoptRef(surface->newImageSnapshot()));
}

ImageBitmap::ImageBitmap(HTMLImageElement* image, const IntRect& cropRect, Document* document)
{
    m_image = cropImage(image->cachedImage()->image(), cropRect);
}

ImageBitmap::ImageBitmap(ImageBitmap* bitmap, const IntRect& cropRect)
{
    m_image = cropImage(bitmap->bitmapImage(), cropRect);
    m_image->setOriginClean(bitmap->originClean());
}

ImageBitmap::ImageBitmap(PassRefPtr<StaticBitmapImage> image, const IntRect& cropRect)
{
    m_image = cropImage(image.get(), cropRect);
    m_image->setOriginClean(image->originClean());
}

ImageBitmap::ImageBitmap(PassRefPtr<StaticBitmapImage> image)
{
    m_image = image;
}

PassRefPtr<StaticBitmapImage> ImageBitmap::transfer()
{
    ASSERT(!isNeutered());
    m_isNeutered = true;
    return m_image.release();
}

ImageBitmap::~ImageBitmap()
{
}

PassRefPtrWillBeRawPtr<ImageBitmap> ImageBitmap::create(HTMLImageElement* image, const IntRect& cropRect, Document* document)
{
    IntRect normalizedCropRect = normalizeRect(cropRect);
    return adoptRefWillBeNoop(new ImageBitmap(image, normalizedCropRect, document));
}

PassRefPtrWillBeRawPtr<ImageBitmap> ImageBitmap::create(ImageBitmap* bitmap, const IntRect& cropRect)
{
    IntRect normalizedCropRect = normalizeRect(cropRect);
    return adoptRefWillBeNoop(new ImageBitmap(bitmap, normalizedCropRect));
}

PassRefPtrWillBeRawPtr<ImageBitmap> ImageBitmap::create(PassRefPtr<StaticBitmapImage> image, const IntRect& cropRect)
{
    IntRect normalizedCropRect = normalizeRect(cropRect);
    return adoptRefWillBeNoop(new ImageBitmap(image, normalizedCropRect));
}

PassRefPtrWillBeRawPtr<ImageBitmap> ImageBitmap::create(PassRefPtr<StaticBitmapImage> image)
{
    return adoptRefWillBeNoop(new ImageBitmap(image));
}

unsigned long ImageBitmap::width() const
{
    if (!m_image)
        return 0;
    ASSERT(m_image->width() > 0);
    return m_image->width();
}

unsigned long ImageBitmap::height() const
{
    if (!m_image)
        return 0;
    ASSERT(m_image->height() > 0);
    return m_image->height();
}

IntSize ImageBitmap::size() const
{
    if (!m_image)
        return IntSize();
    ASSERT(m_image->width() > 0 && m_image->height() > 0);
    return IntSize(m_image->width(), m_image->height());
}

void ImageBitmap::notifyImageSourceChanged()
{
}

DEFINE_TRACE(ImageBitmap)
{
    ImageLoaderClient::trace(visitor);
}

} // namespace blink
