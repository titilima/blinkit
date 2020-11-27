// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: style_fetched_image.h
// Description: StyleFetchedImage Class
//      Author: Ziming Li
//     Created: 2020-09-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
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

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_STYLE_STYLE_FETCHED_IMAGE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_STYLE_STYLE_FETCHED_IMAGE_H_

#include "third_party/blink/renderer/core/loader/resource/image_resource_observer.h"
#include "third_party/blink/renderer/core/style/style_image.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "url/gurl.h"

namespace blink {

class Document;
class FetchParameters;

// This class represents an <image> that loads a single image resource (the
// url(...) function.)
class StyleFetchedImage final : public StyleImage,
                                public ImageResourceObserver {
  // BKTODO: USING_PRE_FINALIZER(StyleFetchedImage, Dispose);

 public:
  static StyleFetchedImage* Create(const Document& document,
                                   FetchParameters& params,
                                   bool is_lazyload_deferred) {
    return new StyleFetchedImage(document, params, is_lazyload_deferred);
  }
  ~StyleFetchedImage() override;

  WrappedImagePtr Data() const override;

  CSSValue* CssValue() const override;
  CSSValue* ComputedCSSValue() const override;

  bool CanRender() const override;
  bool IsLoaded() const override;
  bool ErrorOccurred() const override;
  FloatSize ImageSize(const Document&,
                      float multiplier,
                      const LayoutSize& default_object_size) const override;
  bool ImageHasRelativeSize() const override;
  bool UsesImageContainerSize() const override;
  void AddClient(ImageResourceObserver*) override;
  void RemoveClient(ImageResourceObserver*) override;
  void ImageNotifyFinished(ImageResourceContent*) override;
  bool GetImageAnimationPolicy(ImageAnimationPolicy&) override;
  String DebugName() const override { return "StyleFetchedImage"; }
  scoped_refptr<Image> GetImage(const ImageResourceObserver&,
                                const Document&,
                                const ComputedStyle&,
                                const FloatSize& target_size) const override;
  bool KnownToBeOpaque(const Document&, const ComputedStyle&) const override;
  ImageResourceContent* CachedImage() const override;

  void LoadDeferredImage(const Document& document);

  void Trace(blink::Visitor*) override;

 private:
  StyleFetchedImage(const Document&,
                    FetchParameters&,
                    bool is_lazyload_deferred);

  bool IsEqual(const StyleImage&) const override;
  void Dispose();

  Member<ImageResourceContent> image_;
  Member<const Document> document_;
  const GURL url_;
};

DEFINE_STYLE_IMAGE_TYPE_CASTS(StyleFetchedImage, IsImageResource());

}  // namespace blink
#endif
