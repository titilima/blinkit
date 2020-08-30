// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: animation_input_helpers.cc
// Description: AnimationInputHelpers Class
//      Author: Ziming Li
//     Created: 2020-08-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/animation/animation_input_helpers.h"

#include "third_party/blink/renderer/core/animation/property_handle.h"
#include "third_party/blink/renderer/core/css/css_value_list.h"
#include "third_party/blink/renderer/core/css/parser/css_parser.h"
#include "third_party/blink/renderer/core/css/parser/css_variable_parser.h"
#include "third_party/blink/renderer/core/css/resolver/css_to_style_map.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"

namespace blink {

const char kSVGPrefix[] = "svg-";
const unsigned kSVGPrefixLength = sizeof(kSVGPrefix) - 1;

static bool IsSVGPrefixed(const String& property) {
  return property.StartsWith(kSVGPrefix);
}

static String RemoveSVGPrefix(const String& property) {
  DCHECK(IsSVGPrefixed(property));
  return property.Substring(kSVGPrefixLength);
}

static String CSSPropertyToKeyframeAttribute(const CSSProperty& property) {
  DCHECK_NE(property.PropertyID(), CSSPropertyInvalid);
  DCHECK_NE(property.PropertyID(), CSSPropertyVariable);

  switch (property.PropertyID()) {
    case CSSPropertyFloat:
      return "cssFloat";
    case CSSPropertyOffset:
      return "cssOffset";
    default:
      return property.GetJSPropertyName();
  }
}

static String PresentationAttributeToKeyframeAttribute(
    const CSSProperty& presentation_attribute) {
  StringBuilder builder;
  builder.Append(kSVGPrefix, kSVGPrefixLength);
  builder.Append(presentation_attribute.GetPropertyName());
  return builder.ToString();
}

CSSPropertyID AnimationInputHelpers::KeyframeAttributeToCSSProperty(
    const String& property,
    const Document& document) {
  if (CSSVariableParser::IsValidVariableName(property))
    return CSSPropertyVariable;

  // Disallow prefixed properties.
  if (property[0] == '-')
    return CSSPropertyInvalid;
  if (IsASCIIUpper(property[0]))
    return CSSPropertyInvalid;
  if (property == "cssFloat")
    return CSSPropertyFloat;
  if (property == "cssOffset")
    return CSSPropertyOffset;

  StringBuilder builder;
  for (wtf_size_t i = 0; i < property.length(); ++i) {
    // Disallow hyphenated properties.
    if (property[i] == '-')
      return CSSPropertyInvalid;
    if (IsASCIIUpper(property[i]))
      builder.Append('-');
    builder.Append(property[i]);
  }
  return cssPropertyID(builder.ToString());
}

CSSPropertyID AnimationInputHelpers::KeyframeAttributeToPresentationAttribute(
    const String& property,
    const Element* element) {
  return CSSPropertyInvalid;
}

QualifiedName SvgAttributeName(const String& property) {
  DCHECK(!IsSVGPrefixed(property));
  return QualifiedName(g_null_atom, AtomicString(property), g_null_atom);
}

const QualifiedName* AnimationInputHelpers::KeyframeAttributeToSVGAttribute(
    const String& property,
    Element* element) {
  return nullptr;
}

scoped_refptr<TimingFunction> AnimationInputHelpers::ParseTimingFunction(
    const String& string,
    Document* document,
    ExceptionState& exception_state) {
  if (string.IsEmpty()) {
    exception_state.ThrowTypeError("Easing may not be the empty string");
    return nullptr;
  }

  // Fallback to an insecure parsing mode if we weren't provided with a
  // document.
  SecureContextMode secure_context_mode =
      document ? document->GetSecureContextMode()
               : SecureContextMode::kInsecureContext;
  const CSSValue* value =
      CSSParser::ParseSingleValue(CSSPropertyTransitionTimingFunction, string,
                                  StrictCSSParserContext(secure_context_mode));
  if (!value || !value->IsValueList()) {
    DCHECK(!value || value->IsCSSWideKeyword());
    exception_state.ThrowTypeError("'" + string +
                                   "' is not a valid value for easing");
    return nullptr;
  }
  const CSSValueList* value_list = ToCSSValueList(value);
  if (value_list->length() > 1) {
    exception_state.ThrowTypeError("Easing may not be set to a list of values");
    return nullptr;
  }
  return CSSToStyleMap::MapAnimationTimingFunction(value_list->Item(0), true,
                                                   document);
}

String AnimationInputHelpers::PropertyHandleToKeyframeAttribute(
    PropertyHandle property) {
  if (property.IsCSSProperty()) {
    return property.IsCSSCustomProperty()
               ? property.CustomPropertyName()
               : CSSPropertyToKeyframeAttribute(property.GetCSSProperty());
  }

  if (property.IsPresentationAttribute()) {
    return PresentationAttributeToKeyframeAttribute(
        property.PresentationAttribute());
  }

  DCHECK(property.IsSVGAttribute());
  return property.SvgAttribute().LocalName();
}

}  // namespace blink
