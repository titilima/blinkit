// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_uri_value.cc
// Description: CSSURIValue Class
//      Author: Ziming Li
//     Created: 2020-09-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/css_uri_value.h"

#include "third_party/blink/renderer/core/css/css_markup.h"
#include "third_party/blink/renderer/core/dom/document.h"
// BKTODO: #include "third_party/blink/renderer/core/svg/svg_resource.h"
#include "url/gurl.h"

namespace blink {

CSSURIValue::CSSURIValue(const AtomicString& relative_url,
                         const AtomicString& absolute_url)
    : CSSValue(kURIClass),
      relative_url_(relative_url),
      is_local_(relative_url.StartsWith('#')),
      absolute_url_(absolute_url) {}

CSSURIValue::CSSURIValue(const AtomicString& relative_url, const GURL& url)
    : CSSURIValue(relative_url, AtomicString::FromStdUTF8(url.spec())) {}

CSSURIValue::~CSSURIValue() = default;

#if 0 // BKTODO:
SVGResource* CSSURIValue::EnsureResourceReference() const {
  if (!resource_)
    resource_ = new ExternalSVGResource(AbsoluteUrl());
  return resource_;
}
#endif

void CSSURIValue::ReResolveUrl(const Document& document) const {
  if (is_local_)
    return;
  GURL url = document.CompleteURL(relative_url_);
  AtomicString url_string = AtomicString::FromStdUTF8(url.spec());
  if (url_string == absolute_url_)
    return;
  absolute_url_ = url_string;
  ASSERT(false); // BKTODO:
#if 0
  resource_ = nullptr;
#endif
}

String CSSURIValue::CustomCSSText() const {
  return SerializeURI(relative_url_);
}

AtomicString CSSURIValue::FragmentIdentifier() const {
  if (is_local_)
    return AtomicString(relative_url_.GetString().Substring(1));
  return AtomicString::FromStdUTF8(AbsoluteUrl().ref());
}

GURL CSSURIValue::AbsoluteUrl() const {
  return GURL(absolute_url_.StdUtf8());
}

bool CSSURIValue::IsLocal(const Document& document) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return is_local_ ||
         EqualIgnoringFragmentIdentifier(AbsoluteUrl(), document.Url());
#endif
}

bool CSSURIValue::Equals(const CSSURIValue& other) const {
  // If only one has the 'local url' flag set, the URLs can't match.
  if (is_local_ != other.is_local_)
    return false;
  if (is_local_)
    return relative_url_ == other.relative_url_;
  return absolute_url_ == other.absolute_url_;
}

void CSSURIValue::TraceAfterDispatch(blink::Visitor* visitor) {
  // BKTODO: visitor->Trace(resource_);
  CSSValue::TraceAfterDispatch(visitor);
}

}  // namespace blink
