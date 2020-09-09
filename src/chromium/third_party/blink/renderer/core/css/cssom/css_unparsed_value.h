// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_unparsed_value.h
// Description: CSSUnparsedValue Class
//      Author: Ziming Li
//     Created: 2020-09-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_UNPARSED_VALUE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_UNPARSED_VALUE_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/css/cssom/css_style_value.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class CSSVariableReferenceValue;
class CSSCustomPropertyDeclaration;
class CSSVariableData;
// BKTODO: using CSSUnparsedSegment = StringOrCSSVariableReferenceValue;

class CORE_EXPORT CSSUnparsedValue final : public CSSStyleValue {
  DEFINE_WRAPPERTYPEINFO();

 public:
#if 0 // BKTODO:
  static CSSUnparsedValue* Create(
      const HeapVector<CSSUnparsedSegment>& tokens) {
    return new CSSUnparsedValue(tokens);
  }
#endif

  // Blink-internal constructor
  static CSSUnparsedValue* Create() {
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    return Create(HeapVector<CSSUnparsedSegment>());
#endif
  }
  static CSSUnparsedValue* FromCSSValue(const CSSVariableReferenceValue&);
  static CSSUnparsedValue* FromCSSValue(const CSSCustomPropertyDeclaration&);
  static CSSUnparsedValue* FromCSSVariableData(const CSSVariableData&);

  const CSSValue* ToCSSValue() const override;

  StyleValueType GetType() const override { return kUnparsedType; }

#if 0 // BKTODO:
  CSSUnparsedSegment AnonymousIndexedGetter(unsigned, ExceptionState&) const;
  bool AnonymousIndexedSetter(unsigned,
                              const CSSUnparsedSegment&,
                              ExceptionState&);

  wtf_size_t length() const { return tokens_.size(); }
#endif

 protected:
#if 0 // BKTODO:
  CSSUnparsedValue(const HeapVector<CSSUnparsedSegment>& tokens)
      : CSSStyleValue(), tokens_(tokens) {}
#endif

 private:
  static CSSUnparsedValue* FromString(const String& string) {
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    HeapVector<CSSUnparsedSegment> tokens;
    tokens.push_back(CSSUnparsedSegment::FromString(string));
    return Create(tokens);
#endif
  }

  String ToString() const;

  FRIEND_TEST_ALL_PREFIXES(CSSVariableReferenceValueTest, MixedList);

#if 0 // BKTODO:
  HeapVector<CSSUnparsedSegment> tokens_;
#endif
  DISALLOW_COPY_AND_ASSIGN(CSSUnparsedValue);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_UNPARSED_VALUE_H_
