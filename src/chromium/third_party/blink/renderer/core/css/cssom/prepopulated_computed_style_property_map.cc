// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: prepopulated_computed_style_property_map.cc
// Description: PrepopulatedComputedStylePropertyMap Class
//      Author: Ziming Li
//     Created: 2020-09-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 the Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/cssom/prepopulated_computed_style_property_map.h"

#include "third_party/blink/renderer/core/css/computed_style_css_value_mapping.h"
#include "third_party/blink/renderer/core/css/css_custom_property_declaration.h"
#include "third_party/blink/renderer/core/css/css_variable_data.h"
#include "third_party/blink/renderer/core/css/cssom/computed_style_property_map.h"
#include "third_party/blink/renderer/core/css/cssom/css_unparsed_value.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {

PrepopulatedComputedStylePropertyMap::PrepopulatedComputedStylePropertyMap(
    const Document& document,
    const ComputedStyle& style,
    Node* styled_node,
    const Vector<CSSPropertyID>& native_properties,
    const Vector<AtomicString>& custom_properties)
    : StylePropertyMapReadOnly(), styled_node_(styled_node) {
  // NOTE: This may over-reserve as shorthand properties will get dropped from
  // being in the map.
  native_values_.ReserveCapacityForSize(native_properties.size());
  custom_values_.ReserveCapacityForSize(custom_properties.size());

  for (const auto& property_id : native_properties) {
    // Silently drop shorthand properties.
    DCHECK_NE(property_id, CSSPropertyInvalid);
    if (CSSProperty::Get(property_id).IsShorthand())
      continue;

    UpdateNativeProperty(style, property_id);
  }

  for (const auto& property_name : custom_properties) {
    UpdateCustomProperty(document, style, property_name);
  }
}

unsigned PrepopulatedComputedStylePropertyMap::size() {
  return native_values_.size() + custom_values_.size();
}

void PrepopulatedComputedStylePropertyMap::UpdateStyle(
    const Document& document,
    const ComputedStyle& style) {
  ASSERT(false); // BKTODO:
#if 0
  for (const auto& property_id : native_values_.Keys()) {
    DCHECK_NE(property_id, CSSPropertyInvalid);
    UpdateNativeProperty(style, property_id);
  }

  for (const auto& property_name : custom_values_.Keys()) {
    UpdateCustomProperty(document, style, property_name);
  }
#endif
}

void PrepopulatedComputedStylePropertyMap::UpdateNativeProperty(
    const ComputedStyle& style,
    CSSPropertyID property_id) {
  ASSERT(false); // BKTODO:
#if 0
  native_values_.Set(property_id,
                     CSSProperty::Get(property_id)
                         .CSSValueFromComputedStyle(
                             style, /* layout_object */ nullptr, styled_node_,
                             /* allow_visited_style */ false));
#endif
}

void PrepopulatedComputedStylePropertyMap::UpdateCustomProperty(
    const Document& document,
    const ComputedStyle& style,
    const AtomicString& property_name) {
  ASSERT(false); // BKTODO:
#if 0
  const CSSValue* value = ComputedStyleCSSValueMapping::Get(
      property_name, style, document.GetPropertyRegistry());
  if (!value)
    value = CSSUnparsedValue::Create()->ToCSSValue();

  custom_values_.Set(property_name, value);
#endif
}

const CSSValue* PrepopulatedComputedStylePropertyMap::GetProperty(
    CSSPropertyID property_id) {
  return native_values_.at(property_id);
}

const CSSValue* PrepopulatedComputedStylePropertyMap::GetCustomProperty(
    AtomicString property_name) {
  return custom_values_.at(property_name);
}

void PrepopulatedComputedStylePropertyMap::ForEachProperty(
    const IterationCallback& callback) {
  // Have to sort by all properties by code point, so we have to store
  // them in a buffer first.
  HeapVector<std::pair<AtomicString, Member<const CSSValue>>> values;

  for (const auto& entry : native_values_) {
    DCHECK(entry.second);
    values.emplace_back(
        CSSProperty::Get(entry.first).GetPropertyNameAtomicString(), entry.second);
  }

  for (const auto& entry : custom_values_) {
    DCHECK(entry.second);
    values.emplace_back(entry.first, entry.second);
  }

  std::sort(values.begin(), values.end(), [](const auto& a, const auto& b) {
    return ComputedStylePropertyMap::ComparePropertyNames(a.first, b.first);
  });

  for (const auto& value : values)
    callback(value.first, *value.second);
}

String PrepopulatedComputedStylePropertyMap::SerializationForShorthand(
    const CSSProperty&) {
  // TODO(816722): Shorthands not yet supported for this style map.
  NOTREACHED();
  return "";
}

}  // namespace blink
