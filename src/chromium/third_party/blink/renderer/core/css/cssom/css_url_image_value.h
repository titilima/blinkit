// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_url_image_value.h
// Description: CSSURLImageValue Class
//      Author: Ziming Li
//     Created: 2020-09-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_URL_IMAGE_VALUE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_URL_IMAGE_VALUE_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/css/cssom/css_style_image_value.h"

namespace blink {

class ScriptState;
class CSSImageValue;

class CORE_EXPORT CSSURLImageValue final : public CSSStyleImageValue {
 public:
  static CSSURLImageValue* Create(ScriptState*,
                                  const AtomicString& url,
                                  ExceptionState&);

  static CSSURLImageValue* FromCSSValue(const CSSImageValue&);

  const String& url() const;

  // CSSStyleImageValue
  base::Optional<IntSize> IntrinsicSize() const final;

  // CanvasImageSource
  ResourceStatus Status() const final;
#if 0 // BKTODO:
  scoped_refptr<Image> GetSourceImageForCanvas(SourceImageStatus*,
                                               AccelerationHint,
                                               const FloatSize&) final;
  bool IsAccelerated() const final;
#endif

  // CSSStyleValue
  StyleValueType GetType() const final { return kURLImageType; }
  const CSSValue* ToCSSValue() const final;

 private:
  explicit CSSURLImageValue(const CSSImageValue& value) : value_(value) {}

#if 0 // BKTODO:
  scoped_refptr<Image> GetImage() const;
#endif

  Member<const CSSImageValue> value_;
  DISALLOW_COPY_AND_ASSIGN(CSSURLImageValue);
};

}  // namespace blink

#endif  // CSSResourceValue_h
