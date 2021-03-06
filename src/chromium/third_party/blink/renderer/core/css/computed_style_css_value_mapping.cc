// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: computed_style_css_value_mapping.cc
// Description: ComputedStyleCSSValueMapping Class
//      Author: Ziming Li
//     Created: 2020-09-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004 Zack Rusin <zack@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012 Apple Inc.
 * All rights reserved.
 * Copyright (C) 2007 Alexey Proskuryakov <ap@webkit.org>
 * Copyright (C) 2007 Nicholas Shanks <webkit@nickshanks.com>
 * Copyright (C) 2011 Sencha, Inc. All rights reserved.
 * Copyright (C) 2015 Google Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include "third_party/blink/renderer/core/css/computed_style_css_value_mapping.h"

#include "third_party/blink/renderer/core/css/css_custom_property_declaration.h"
#include "third_party/blink/renderer/core/css/css_value.h"
#include "third_party/blink/renderer/core/css/property_registry.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {

const CSSValue* ComputedStyleCSSValueMapping::Get(
    const AtomicString custom_property_name,
    const ComputedStyle& style,
    const PropertyRegistry* registry) {
  if (registry) {
    const PropertyRegistration* registration =
        registry->Registration(custom_property_name);
    if (registration) {
      const CSSValue* result = style.GetRegisteredVariable(
          custom_property_name, registration->Inherits());
      if (result)
        return result;
      return registration->Initial();
    }
  }

  bool is_inherited_property = true;
  CSSVariableData* data =
      style.GetVariable(custom_property_name, is_inherited_property);
  if (!data)
    return nullptr;

  return CSSCustomPropertyDeclaration::Create(custom_property_name, data);
}

HeapHashMap<AtomicString, Member<const CSSValue>>
ComputedStyleCSSValueMapping::GetVariables(const ComputedStyle& style,
                                           const PropertyRegistry* registry) {
  HeapHashMap<AtomicString, Member<const CSSValue>> variables;

  StyleInheritedVariables* inherited = style.InheritedVariables();

  if (inherited) {
    for (const auto& name : inherited->GetCustomPropertyNames()) {
      const CSSValue* value =
          ComputedStyleCSSValueMapping::Get(name, style, registry);
      ASSERT(false); // BKTODO:
#if 0
      if (value)
        variables.Set(name, value);
#endif
    }
  }

  StyleNonInheritedVariables* non_inherited = style.NonInheritedVariables();

  if (non_inherited) {
    for (const auto& name : non_inherited->GetCustomPropertyNames()) {
      const CSSValue* value =
          ComputedStyleCSSValueMapping::Get(name, style, registry);
      ASSERT(false); // BKTODO:
#if 0
      if (value)
        variables.Set(name, value);
#endif
    }
  }

  // Registered properties with initial values are not stored explicitly on
  // each computed style. Their initialness is instead indicated by the
  // absence of that property on the computed style. This means that registered
  // properties with an implicit initial value will not appear in the result of
  // Style[Non]InheritedVariables::GetCustomPropertyNames, so we need to
  // iterate though all registrations and add the initial values, if necessary.
  if (registry) {
    for (const auto& entry : *registry) {
      if (variables.Contains(entry.first))
        continue;
      const CSSValue* initial = entry.second->Initial();
      ASSERT(false); // BKTODO:
#if 0
      if (initial)
        variables.Set(entry.key, initial);
#endif
    }
  }

  return variables;
}

}  // namespace blink
