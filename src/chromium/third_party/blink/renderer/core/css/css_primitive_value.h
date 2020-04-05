// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_primitive_value.h
// Description: CSSPrimitiveValue Class
//      Author: Ziming Li
//     Created: 2020-03-28
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Alexey Proskuryakov <ap@webkit.org>
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

#ifndef BLINKIT_BLINK_CSS_PRIMITIVE_VALUE_H
#define BLINKIT_BLINK_CSS_PRIMITIVE_VALUE_H

#pragma once

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/math_extras.h"
#include "third_party/blink/renderer/platform/wtf/text/string_view.h"

namespace blink {

// CSSPrimitiveValue stores numeric data types (e.g. 1, 10px, 4%) and calc()
// values (e.g. calc(3px + 2em)).
class CSSPrimitiveValue
{
 public:
     // These units are iterated through, so be careful when adding or changing the
     // order.
     enum class UnitType {
         kUnknown,
         kNumber,
         kPercentage,
         // Length units
         kEms,
         kExs,
         kPixels,
         kCentimeters,
         kMillimeters,
         kInches,
         kPoints,
         kPicas,
         kQuarterMillimeters,
         kViewportWidth,
         kViewportHeight,
         kViewportMin,
         kViewportMax,
         kRems,
         kChs,
         kUserUnits,  // The SVG term for unitless lengths
         // Angle units
         kDegrees,
         kRadians,
         kGradians,
         kTurns,
         // Time units
         kMilliseconds,
         kSeconds,
         kHertz,
         kKilohertz,
         // Resolution
         kDotsPerPixel,
         kDotsPerInch,
         kDotsPerCentimeter,
         // Other units
         kFraction,
         kInteger,
         kCalc,
         kCalcPercentageWithNumber,
         kCalcPercentageWithLength,
         kCalcLengthWithNumber,
         kCalcPercentageWithLengthAndNumber,

         // This value is used to handle quirky margins in reflow roots (body, td,
         // and th) like WinIE. The basic idea is that a stylesheet can use the value
         // __qem (for quirky em) instead of em. When the quirky value is used, if
         // you're in quirks mode, the margin will collapse away inside a table cell.
         // This quirk is specified in the HTML spec but our impl is different.
         // TODO: Remove this. crbug.com/443952
         kQuirkyEms,
     };

     static UnitType StringToUnitType(StringView string)
     {
         if (string.Is8Bit())
             return StringToUnitType(string.Characters8(), string.length());
         return StringToUnitType(string.Characters16(), string.length());
     }
private:
    static UnitType StringToUnitType(const LChar *characters8, unsigned length);
    static UnitType StringToUnitType(const UChar *characters16, unsigned length);
};

} // namespace blink

#endif // BLINKIT_BLINK_CSS_PRIMITIVE_VALUE_H
