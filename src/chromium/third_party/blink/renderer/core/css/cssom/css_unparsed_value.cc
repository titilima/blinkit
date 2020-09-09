// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_unparsed_value.cc
// Description: CSSUnparsedValue Class
//      Author: Ziming Li
//     Created: 2020-09-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/cssom/css_unparsed_value.h"

#include "third_party/blink/renderer/core/css/css_custom_property_declaration.h"
#include "third_party/blink/renderer/core/css/css_variable_data.h"
#include "third_party/blink/renderer/core/css/css_variable_reference_value.h"
#include "third_party/blink/renderer/core/css/cssom/css_style_variable_reference_value.h"
#include "third_party/blink/renderer/core/css/parser/css_tokenizer.h"
#include "third_party/blink/renderer/platform/bindings/exception_messages.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"

namespace blink {

namespace {

StringView FindVariableName(CSSParserTokenRange& range) {
  range.ConsumeWhitespace();
  return range.Consume().Value();
}

#if 0 // BKTODO:
CSSUnparsedSegment VariableReferenceValue(
    const StringView& variable_name,
    const HeapVector<CSSUnparsedSegment>& tokens) {
  CSSUnparsedValue* unparsed_value;
  if (tokens.size() == 0)
    unparsed_value = nullptr;
  else
    unparsed_value = CSSUnparsedValue::Create(tokens);

  CSSStyleVariableReferenceValue* variable_reference =
      CSSStyleVariableReferenceValue::Create(variable_name.ToString(),
                                             unparsed_value);
  return CSSUnparsedSegment::FromCSSVariableReferenceValue(variable_reference);
}

HeapVector<CSSUnparsedSegment> ParserTokenRangeToTokens(
    CSSParserTokenRange range) {
  HeapVector<CSSUnparsedSegment> tokens;
  StringBuilder builder;
  while (!range.AtEnd()) {
    if (range.Peek().FunctionId() == CSSValueVar ||
        range.Peek().FunctionId() == CSSValueEnv) {
      if (!builder.IsEmpty()) {
        tokens.push_back(CSSUnparsedSegment::FromString(builder.ToString()));
        builder.Clear();
      }
      CSSParserTokenRange block = range.ConsumeBlock();
      StringView variable_name = FindVariableName(block);
      block.ConsumeWhitespace();
      if (block.Peek().GetType() == CSSParserTokenType::kCommaToken)
        block.Consume();
      tokens.push_back(VariableReferenceValue(variable_name,
                                              ParserTokenRangeToTokens(block)));
    } else {
      range.Consume().Serialize(builder);
    }
  }
  if (!builder.IsEmpty()) {
    tokens.push_back(CSSUnparsedSegment::FromString(builder.ToString()));
  }
  return tokens;
}
#endif

}  // namespace

CSSUnparsedValue* CSSUnparsedValue::FromCSSValue(
    const CSSVariableReferenceValue& value) {
  DCHECK(value.VariableDataValue());
  return FromCSSVariableData(*value.VariableDataValue());
}

CSSUnparsedValue* CSSUnparsedValue::FromCSSValue(
    const CSSCustomPropertyDeclaration& value) {
  if (const CSSVariableData* data = value.Value())
    return FromCSSVariableData(*data);

  // Otherwise, it's a CSS-wide keyword
  return FromString(value.CustomCSSText());
}

CSSUnparsedValue* CSSUnparsedValue::FromCSSVariableData(
    const CSSVariableData& value) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return CSSUnparsedValue::Create(ParserTokenRangeToTokens(value.TokenRange()));
#endif
}

#if 0 // BKTODO:
CSSUnparsedSegment CSSUnparsedValue::AnonymousIndexedGetter(
    unsigned index,
    ExceptionState& exception_state) const {
  if (index < tokens_.size())
    return tokens_[index];
  return {};
}

bool CSSUnparsedValue::AnonymousIndexedSetter(unsigned index,
                                              const CSSUnparsedSegment& segment,
                                              ExceptionState& exception_state) {
  if (index < tokens_.size()) {
    tokens_[index] = segment;
    return true;
  }

  if (index == tokens_.size()) {
    tokens_.push_back(segment);
    return true;
  }

  exception_state.ThrowRangeError(
      ExceptionMessages::IndexOutsideRange<unsigned>(
          "index", index, 0, ExceptionMessages::kInclusiveBound, tokens_.size(),
          ExceptionMessages::kInclusiveBound));
  return false;
}
#endif

const CSSValue* CSSUnparsedValue::ToCSSValue() const {
  ASSERT(false); // BKTODO:
#if 0
  if (tokens_.IsEmpty()) {
    return CSSVariableReferenceValue::Create(CSSVariableData::Create());
  }
#endif

  CSSTokenizer tokenizer(ToString());
  const auto tokens = tokenizer.TokenizeToEOF();
  return CSSVariableReferenceValue::Create(CSSVariableData::Create(
      CSSParserTokenRange(tokens), false /* is_animation_tainted */,
      false /* needs_variable_resolution */, GURL(), WTF::TextEncoding()));
}

String CSSUnparsedValue::ToString() const {
  StringBuilder input;

  ASSERT(false); // BKTODO:
#if 0
  for (unsigned i = 0; i < tokens_.size(); i++) {
    if (i) {
      input.Append("/**/");
    }
    if (tokens_[i].IsString()) {
      input.Append(tokens_[i].GetAsString());
    } else if (tokens_[i].IsCSSVariableReferenceValue()) {
      const auto* reference_value = tokens_[i].GetAsCSSVariableReferenceValue();
      input.Append("var(");
      input.Append(reference_value->variable());
      if (reference_value->fallback()) {
        input.Append(",");
        input.Append(reference_value->fallback()->ToString());
      }
      input.Append(")");
    } else {
      NOTREACHED();
    }
  }
#endif

  return input.ToString();
}

}  // namespace blink
