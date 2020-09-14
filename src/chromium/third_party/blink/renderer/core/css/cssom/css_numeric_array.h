// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_numeric_array.h
// Description: CSSNumericArray Class
//      Author: Ziming Li
//     Created: 2020-09-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_NUMERIC_ARRAY_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_NUMERIC_ARRAY_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/css/cssom/css_numeric_value.h"

namespace blink {

// See CSSNumericArray.idl for more information about this class.
class CORE_EXPORT CSSNumericArray final : public ScriptWrappable {
  DEFINE_WRAPPERTYPEINFO();

 public:
  // blink internal
  static CSSNumericArray* Create(CSSNumericValueVector values) {
    return new CSSNumericArray(std::move(values));
  }
#if 0 // BKTODO:
  static CSSNumericArray* FromNumberishes(
      const HeapVector<CSSNumberish>& values) {
    return new CSSNumericArray(CSSNumberishesToNumericValues(values));
  }
#endif

  unsigned length() const { return values_.size(); }
  CSSNumericValue* AnonymousIndexedGetter(unsigned index) {
    if (index < values_.size())
      return values_[index].Get();
    return nullptr;
  }

  const CSSNumericValueVector& Values() const { return values_; }

 private:
  explicit CSSNumericArray(CSSNumericValueVector values)
      : values_(std::move(values)) {}

  GCType GetGCType(void) const override
  {
    ASSERT(false); // BKTODO:
    return GC_MANUAL;
  }

  CSSNumericValueVector values_;
  DISALLOW_COPY_AND_ASSIGN(CSSNumericArray);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_NUMERIC_ARRAY_H_
