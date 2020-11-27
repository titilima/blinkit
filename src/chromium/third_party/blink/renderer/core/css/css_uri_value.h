// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_uri_value.h
// Description: CSSURIValue Class
//      Author: Ziming Li
//     Created: 2020-09-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_URI_VALUE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_URI_VALUE_H_

#include "third_party/blink/renderer/core/css/css_value.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

class GURL;

namespace blink {

class Document;

class CSSURIValue : public CSSValue {
 public:
  static CSSURIValue* Create(const String& relative_url, const GURL& url) {
    return new CSSURIValue(AtomicString(relative_url), url);
  }
  static CSSURIValue* Create(const AtomicString& absolute_url) {
    return new CSSURIValue(absolute_url, absolute_url);
  }
  ~CSSURIValue();

#if 0 // BKTODO:
  SVGResource* EnsureResourceReference() const;
#endif
  void ReResolveUrl(const Document&) const;

  const AtomicString& ValueForSerialization() const {
    return is_local_ ? relative_url_ : absolute_url_;
  }

  String CustomCSSText() const;

  bool IsLocal(const Document&) const;
  AtomicString FragmentIdentifier() const;

  bool Equals(const CSSURIValue&) const;

  void TraceAfterDispatch(blink::Visitor*);

 private:
  CSSURIValue(const AtomicString&, const GURL&);
  CSSURIValue(const AtomicString& relative_url,
              const AtomicString& absolute_url);

  GURL AbsoluteUrl() const;

  AtomicString relative_url_;
  bool is_local_;

  // BKTODO: mutable Member<SVGResource> resource_;
  mutable AtomicString absolute_url_;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSURIValue, IsURIValue());

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_URI_VALUE_H_
