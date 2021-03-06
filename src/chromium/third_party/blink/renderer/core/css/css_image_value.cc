// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_image_value.cc
// Description: CSSImageValue Class
//      Author: Ziming Li
//     Created: 2020-09-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
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
 */

#include "third_party/blink/renderer/core/css/css_image_value.h"

#include "third_party/blink/renderer/core/css/css_markup.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/loader/resource/image_resource_content.h"
#include "third_party/blink/renderer/core/style/style_fetched_image.h"
#include "third_party/blink/renderer/platform/cross_origin_attribute_value.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_initiator_type_names.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_parameters.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader_options.h"
#include "third_party/blink/renderer/platform/weborigin/kurl.h"
#include "third_party/blink/renderer/platform/weborigin/security_policy.h"

namespace blink {

#if 0 // BKTODO:
CSSImageValue::CSSImageValue(const AtomicString& raw_value,
                             const KURL& url,
                             const Referrer& referrer,
                             StyleImage* image)
    : CSSValue(kImageClass),
      relative_url_(raw_value),
      referrer_(referrer),
      absolute_url_(url.GetString()),
      cached_image_(image) {}
#endif

CSSImageValue::CSSImageValue(const AtomicString& absolute_url)
    : CSSValue(kImageClass),
      relative_url_(absolute_url),
      absolute_url_(absolute_url) {}

CSSImageValue::~CSSImageValue() = default;

StyleImage* CSSImageValue::CacheImage(
    const Document& document,
    FetchParameters::ImageRequestOptimization image_request_optimization,
    CrossOriginAttributeValue cross_origin) {
  if (!cached_image_) {
    if (absolute_url_.IsEmpty())
      ReResolveURL(document);
    ResourceRequest resource_request(absolute_url_);
    ASSERT(false); // BKTODO:
#if 0
    resource_request.SetHTTPReferrer(SecurityPolicy::GenerateReferrer(
        referrer_.referrer_policy, resource_request.Url(), referrer_.referrer));
#endif
    ResourceLoaderOptions options;
    options.initiator_info.name = initiator_name_.IsEmpty()
                                      ? fetch_initiator_type_names::kCSS
                                      : initiator_name_;
    FetchParameters params(resource_request, options);

    if (cross_origin != kCrossOriginAttributeNotSet) {
      ASSERT(false); // BKTODO:
#if 0
      params.SetCrossOriginAccessControl(document.GetSecurityOrigin(),
                                         cross_origin);
#endif
    }

    ASSERT(false); // BKTODO:
#if 0
    if (document.GetFrame() &&
        image_request_optimization == FetchParameters::kAllowPlaceholder &&
        document.GetFrame()->IsClientLoFiAllowed(params.GetResourceRequest())) {
      params.SetClientLoFiPlaceholder();
    }
#endif
    cached_image_ = StyleFetchedImage::Create(
        document, params,
        image_request_optimization == FetchParameters::kDeferImageLoad);
  }

  return cached_image_.Get();
}

void CSSImageValue::RestoreCachedResourceIfNeeded(
    const Document& document) const {
  if (!cached_image_ || !document.Fetcher() || absolute_url_.IsNull())
    return;

  ImageResourceContent* resource = cached_image_->CachedImage();
  if (!resource)
    return;

  ASSERT(false); // BKTODO:
#if 0
  resource->EmulateLoadStartedForInspector(
      document.Fetcher(), GURL(absolute_url_.StdUtf8()),
      initiator_name_.IsEmpty() ? fetch_initiator_type_names::kCSS
                                : initiator_name_);
#endif
}

bool CSSImageValue::HasFailedOrCanceledSubresources() const {
  if (!cached_image_)
    return false;
  if (ImageResourceContent* cached_resource = cached_image_->CachedImage())
    return cached_resource->LoadFailedOrCanceled();
  return true;
}

bool CSSImageValue::Equals(const CSSImageValue& other) const {
  if (absolute_url_.IsEmpty() && other.absolute_url_.IsEmpty())
    return relative_url_ == other.relative_url_;
  return absolute_url_ == other.absolute_url_;
}

String CSSImageValue::CustomCSSText() const {
  return SerializeURI(relative_url_);
}

bool CSSImageValue::KnownToBeOpaque(const Document& document,
                                    const ComputedStyle& style) const {
  return cached_image_ ? cached_image_->KnownToBeOpaque(document, style)
                       : false;
}

void CSSImageValue::TraceAfterDispatch(blink::Visitor* visitor) {
  visitor->Trace(cached_image_);
  CSSValue::TraceAfterDispatch(visitor);
}

void CSSImageValue::ReResolveURL(const Document& document) const {
  GURL url = document.CompleteURL(relative_url_);
  AtomicString url_string = AtomicString::FromStdUTF8(url.spec());
  if (url_string == absolute_url_)
    return;
  absolute_url_ = url_string;
  ASSERT(false); // BKTODO:
#if 0
  cached_image_.Clear();
#endif
}

}  // namespace blink
