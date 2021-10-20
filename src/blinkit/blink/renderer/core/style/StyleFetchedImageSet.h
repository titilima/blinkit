// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyleFetchedImageSet.h
// Description: StyleFetchedImageSet Class
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef StyleFetchedImageSet_h
#define StyleFetchedImageSet_h

#include "core/fetch/ImageResourceClient.h"
#include "core/fetch/ResourcePtr.h"
#include "core/style/StyleImage.h"
#include "platform/geometry/LayoutSize.h"

namespace blink {

class ImageResource;
class CSSImageSetValue;

// This class keeps one cached image and has access to a set of alternatives.

class StyleFetchedImageSet final : public StyleImage, private ImageResourceClient {
public:
    static GCRefPtr<StyleFetchedImageSet> create(ImageResource* image, float imageScaleFactor, CSSImageSetValue* value, const KURL& url)
    {
        return BlinKit::GCWrapShared(new StyleFetchedImageSet(image, imageScaleFactor, value, url));
    }
    ~StyleFetchedImageSet() override;

    GCRefPtr<CSSValue> cssValue() const override;
    GCRefPtr<CSSValue> computedCSSValue() const override;

    // FIXME: This is used by StyleImage for equals comparison, but this implementation
    // only looks at the image from the set that we have loaded. I'm not sure if that is
    // meaningful enough or not.
    WrappedImagePtr data() const override;

#if !ENABLE(OILPAN)
    void clearImageSetValue() { m_imageSetValue = nullptr; }
#endif

    bool canRender() const override;
    bool isLoaded() const override;
    bool errorOccurred() const override;
    LayoutSize imageSize(const LayoutObject*, float multiplier) const override;
    bool imageHasRelativeWidth() const override;
    bool imageHasRelativeHeight() const override;
    void computeIntrinsicDimensions(const LayoutObject*, Length& intrinsicWidth, Length& intrinsicHeight, FloatSize& intrinsicRatio) override;
    bool usesImageContainerSize() const override;
    void addClient(LayoutObject*) override;
    void removeClient(LayoutObject*) override;
    PassRefPtr<Image> image(const LayoutObject*, const IntSize&, float) const override;
    float imageScaleFactor() const override { return m_imageScaleFactor; }
    bool knownToBeOpaque(const LayoutObject*) const override;
    ImageResource* cachedImage() const override;

    DECLARE_VIRTUAL_TRACE();

private:
    StyleFetchedImageSet(ImageResource*, float imageScaleFactor, CSSImageSetValue*, const KURL&);

    void dispose();

    String debugName() const override { return "StyleFetchedImageSet"; }

    ResourcePtr<ImageResource> m_bestFitImage;
    float m_imageScaleFactor;

    CSSImageSetValue *m_imageSetValue; // Not retained; it owns us.
    const KURL m_url;
};

DEFINE_STYLE_IMAGE_TYPE_CASTS(StyleFetchedImageSet, isImageResourceSet());

} // namespace blink

#endif // StyleFetchedImageSet_h
