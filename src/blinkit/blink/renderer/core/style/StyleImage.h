// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyleImage.h
// Description: StyleImage Class
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2000 Lars Knoll (knoll@kde.org)
 *           (C) 2000 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef StyleImage_h
#define StyleImage_h

#include "blinkit/blink/renderer/core/css/CSSValue.h"
#include "blinkit/blink/renderer/platform/geometry/LayoutSize.h"
#include "blinkit/blink/renderer/platform/graphics/Image.h"
#include "blinkit/blink/renderer/wtf/PassRefPtr.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"

namespace blink {

class ImageResource;
class CSSValue;
class LayoutObject;

typedef void* WrappedImagePtr;

class CORE_EXPORT StyleImage : public BlinKit::GCObject {
public:
    virtual ~StyleImage() { }

    bool operator==(const StyleImage& other) const
    {
        return data() == other.data();
    }

    virtual GCRefPtr<CSSValue> cssValue() const = 0;
    virtual GCRefPtr<CSSValue> computedCSSValue() const = 0;

    virtual bool canRender() const { return true; }
    virtual bool isLoaded() const { return true; }
    virtual bool errorOccurred() const { return false; }
    virtual LayoutSize imageSize(const LayoutObject*, float multiplier) const = 0;
    virtual void computeIntrinsicDimensions(const LayoutObject*, Length& intrinsicWidth, Length& intrinsicHeight, FloatSize& intrinsicRatio) = 0;
    virtual bool imageHasRelativeWidth() const = 0;
    virtual bool imageHasRelativeHeight() const = 0;
    virtual bool usesImageContainerSize() const = 0;
    virtual void addClient(LayoutObject*) = 0;
    virtual void removeClient(LayoutObject*) = 0;
    virtual PassRefPtr<Image> image(const LayoutObject*, const IntSize&, float) const = 0;
    virtual WrappedImagePtr data() const = 0;
    virtual float imageScaleFactor() const { return 1; }
    virtual bool knownToBeOpaque(const LayoutObject*) const = 0;
    virtual ImageResource* cachedImage() const { return 0; }

    ALWAYS_INLINE bool isImageResource() const { return m_isImageResource; }
    ALWAYS_INLINE bool isPendingImage() const { return m_isPendingImage; }
    ALWAYS_INLINE bool isGeneratedImage() const { return m_isGeneratedImage; }
    ALWAYS_INLINE bool isImageResourceSet() const { return m_isImageResourceSet; }

    DEFINE_INLINE_VIRTUAL_TRACE() { }

protected:
    StyleImage()
        : m_isImageResource(false)
        , m_isPendingImage(false)
        , m_isGeneratedImage(false)
        , m_isImageResourceSet(false)
    {
    }
    bool m_isImageResource:1;
    bool m_isPendingImage:1;
    bool m_isGeneratedImage:1;
    bool m_isImageResourceSet:1;
};

#define DEFINE_STYLE_IMAGE_TYPE_CASTS(thisType, function) \
    DEFINE_TYPE_CASTS(thisType, StyleImage, styleImage, styleImage->function, styleImage.function); \
    inline thisType* to##thisType(const RefPtrWillBeMember<StyleImage>& styleImage) { return to##thisType(styleImage.get()); } \
    typedef int NeedsSemiColonAfterDefineStyleImageTypeCasts

}
#endif
