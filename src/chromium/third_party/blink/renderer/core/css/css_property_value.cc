// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_property_value.cc
// Description: CSSPropertyValue Class
//      Author: Ziming Li
//     Created: 2020-12-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/**
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "third_party/blink/renderer/core/css/css_property_value.h"

#include "third_party/blink/renderer/core/style/computed_style_constants.h"
#include "third_party/blink/renderer/core/style_property_shorthand.h"

namespace blink {

struct SameSizeAsCSSPropertyValue {
  uint32_t bitfields;
  void* property;
  Member<void*> value;
};

static_assert(sizeof(CSSPropertyValue) == sizeof(SameSizeAsCSSPropertyValue),
              "CSSPropertyValue should stay small");

CSSPropertyID CSSPropertyValueMetadata::ShorthandID() const {
  ASSERT(valid_);
  if (!is_set_from_shorthand_)
    return CSSPropertyInvalid;

  Vector<StylePropertyShorthand, 4> shorthands;
  getMatchingShorthandsForLonghand(Property().PropertyID(), &shorthands);
  DCHECK(shorthands.size());
  DCHECK_GE(index_in_shorthands_vector_, 0u);
  DCHECK_LT(index_in_shorthands_vector_, shorthands.size());
  return shorthands.at(index_in_shorthands_vector_).id();
}

bool CSSPropertyValue::operator==(const CSSPropertyValue& other) const {
  return DataEquivalent(value_, other.value_) &&
         IsImportant() == other.IsImportant();
}

}  // namespace blink
