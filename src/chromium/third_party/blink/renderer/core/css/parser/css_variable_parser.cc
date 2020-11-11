// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_variable_parser.cc
// Description: CSSVariableParser Class
//      Author: Ziming Li
//     Created: 2020-11-10
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/parser/css_variable_parser.h"

#include "third_party/blink/renderer/core/css/css_custom_property_declaration.h"
#include "third_party/blink/renderer/core/css/css_variable_reference_value.h"
#include "third_party/blink/renderer/core/css/parser/css_parser_token_range.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"

namespace blink {

namespace {

bool IsValidVariableReference(CSSParserTokenRange, bool);
bool IsValidEnvVariableReference(CSSParserTokenRange, bool);

bool ClassifyBlock(CSSParserTokenRange range,
                   bool& has_references,
                   bool skip_variables) {
  size_t block_stack_size = 0;

  while (!range.AtEnd()) {
    // First check if this is a valid variable reference, then handle the next
    // token accordingly.
    if (range.Peek().GetBlockType() == CSSParserToken::kBlockStart) {
      const CSSParserToken& token = range.Peek();

      // A block may have both var and env references. They can also be nested
      // and used as fallbacks.
      switch (token.FunctionId()) {
        case CSSValueVar:
          if (!IsValidVariableReference(range.ConsumeBlock(), skip_variables))
            return false;  // Invalid reference.
          has_references = true;
          continue;
        case CSSValueEnv:
          if (!RuntimeEnabledFeatures::CSSEnvironmentVariablesEnabled())
            return false;
          if (!IsValidEnvVariableReference(range.ConsumeBlock(),
                                           skip_variables))
            return false;  // Invalid reference.
          has_references = true;
          continue;
        default:
          break;
      }
    }

    const CSSParserToken& token = range.Consume();
    if (token.GetBlockType() == CSSParserToken::kBlockStart) {
      // If we are an invalid function then we should skip over any variables
      // this function contains.
      if (token.GetType() == CSSParserTokenType::kFunctionToken &&
          token.FunctionId() == CSSValueInvalid) {
        skip_variables = true;
      }
      ++block_stack_size;
    } else if (token.GetBlockType() == CSSParserToken::kBlockEnd) {
      --block_stack_size;
    } else {
      switch (token.GetType()) {
        case kDelimiterToken: {
          if (token.Delimiter() == '!' && block_stack_size == 0)
            return false;
          break;
        }
        case kRightParenthesisToken:
        case kRightBraceToken:
        case kRightBracketToken:
        case kBadStringToken:
        case kBadUrlToken:
          return false;
        case kSemicolonToken:
          if (block_stack_size == 0)
            return false;
          break;
        default:
          break;
      }
    }
  }
  return true;
}

bool IsValidVariableReference(CSSParserTokenRange range, bool skip_variables) {
  range.ConsumeWhitespace();
  if (skip_variables)
    return false;
  if (!CSSVariableParser::IsValidVariableName(
          range.ConsumeIncludingWhitespace()))
    return false;
  if (range.AtEnd())
    return true;

  if (range.Consume().GetType() != kCommaToken)
    return false;
  if (range.AtEnd())
    return false;

  bool has_references = false;
  return ClassifyBlock(range, has_references, skip_variables);
}

bool IsValidEnvVariableReference(CSSParserTokenRange range,
                                 bool skip_variables) {
  range.ConsumeWhitespace();
  if (skip_variables)
    return false;
  if (range.ConsumeIncludingWhitespace().GetType() !=
      CSSParserTokenType::kIdentToken)
    return false;
  if (range.AtEnd())
    return true;

  if (range.Consume().GetType() != kCommaToken)
    return false;
  if (range.AtEnd())
    return false;

  bool has_references = false;
  return ClassifyBlock(range, has_references, skip_variables);
}

CSSValueID ClassifyVariableRange(CSSParserTokenRange range,
                                 bool& has_references) {
  has_references = false;

  range.ConsumeWhitespace();
  if (range.Peek().GetType() == kIdentToken) {
    CSSValueID id = range.ConsumeIncludingWhitespace().Id();
    if (range.AtEnd() &&
        (id == CSSValueInherit || id == CSSValueInitial || id == CSSValueUnset))
      return id;
  }

  if (ClassifyBlock(range, has_references, false /* skip_variables */))
    return CSSValueInternalVariableValue;
  return CSSValueInvalid;
}

}  // namespace

bool CSSVariableParser::IsValidVariableName(const CSSParserToken& token) {
  if (token.GetType() != kIdentToken)
    return false;

  StringView value = token.Value();
  return value.length() >= 2 && value[0] == '-' && value[1] == '-';
}

bool CSSVariableParser::IsValidVariableName(const String& string) {
  return string.length() >= 2 && string[0] == '-' && string[1] == '-';
}

bool CSSVariableParser::ContainsValidVariableReferences(
    CSSParserTokenRange range) {
  bool has_references;
  CSSValueID type = ClassifyVariableRange(range, has_references);
  return type == CSSValueInternalVariableValue && has_references;
}

CSSCustomPropertyDeclaration* CSSVariableParser::ParseDeclarationValue(
    const AtomicString& variable_name,
    CSSParserTokenRange range,
    bool is_animation_tainted,
    const CSSParserContext& context) {
  if (range.AtEnd())
    return nullptr;

  bool has_references;
  CSSValueID type = ClassifyVariableRange(range, has_references);

  if (type == CSSValueInvalid)
    return nullptr;
  if (type == CSSValueInternalVariableValue) {
    return CSSCustomPropertyDeclaration::Create(
        variable_name,
        CSSVariableData::Create(range, is_animation_tainted, has_references,
                                context.BaseURL(), context.Charset()));
  }
  return CSSCustomPropertyDeclaration::Create(variable_name, type);
}

CSSVariableReferenceValue* CSSVariableParser::ParseRegisteredPropertyValue(
    CSSParserTokenRange range,
    const CSSParserContext& context,
    bool require_var_reference,
    bool is_animation_tainted) {
  if (range.AtEnd())
    return nullptr;

  bool has_references;
  CSSValueID type = ClassifyVariableRange(range, has_references);

  if (type != CSSValueInternalVariableValue)
    return nullptr;  // Invalid or a css-wide keyword
  if (require_var_reference && !has_references)
    return nullptr;
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return CSSVariableReferenceValue::Create(
      CSSVariableData::Create(range, is_animation_tainted, has_references,
                              context.BaseURL(), context.Charset()),
      context);
#endif
}

}  // namespace blink
