// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Generated from template:
//   core/css/templates/css_primitive_value_unit_trie.cc.tmpl
// and input files:
//   ../../third_party/blink/renderer/core/css/css_primitive_value_units.json5


#include "third_party/blink/renderer/core/css/css_primitive_value.h"

namespace blink {

namespace {

template<typename CharacterType>
CSSPrimitiveValue::UnitType cssPrimitiveValueUnitFromTrie(
    const CharacterType* data, unsigned length) {
  DCHECK(data);
  DCHECK(length);
  switch (length) {
    case 1:
        switch (ToASCIILower(data[0])) {
        case 'q':
            return CSSPrimitiveValue::UnitType::kQuarterMillimeters;

        case 's':
            return CSSPrimitiveValue::UnitType::kSeconds;

        case 'x':
            return CSSPrimitiveValue::UnitType::kDotsPerPixel;

        }
        break;

    case 2:
        switch (ToASCIILower(data[0])) {
        case 'c':
            switch (ToASCIILower(data[1])) {
            case 'h':
                return CSSPrimitiveValue::UnitType::kChs;

            case 'm':
                return CSSPrimitiveValue::UnitType::kCentimeters;

            }
            break;

        case 'e':
            switch (ToASCIILower(data[1])) {
            case 'm':
                return CSSPrimitiveValue::UnitType::kEms;

            case 'x':
                return CSSPrimitiveValue::UnitType::kExs;

            }
            break;

        case 'f':
            if (ToASCIILower(data[1]) == 'r')
                return CSSPrimitiveValue::UnitType::kFraction;
            break;

        case 'h':
            if (ToASCIILower(data[1]) == 'z')
                return CSSPrimitiveValue::UnitType::kHertz;
            break;

        case 'i':
            if (ToASCIILower(data[1]) == 'n')
                return CSSPrimitiveValue::UnitType::kInches;
            break;

        case 'm':
            switch (ToASCIILower(data[1])) {
            case 'm':
                return CSSPrimitiveValue::UnitType::kMillimeters;

            case 's':
                return CSSPrimitiveValue::UnitType::kMilliseconds;

            }
            break;

        case 'p':
            switch (ToASCIILower(data[1])) {
            case 'c':
                return CSSPrimitiveValue::UnitType::kPicas;

            case 't':
                return CSSPrimitiveValue::UnitType::kPoints;

            case 'x':
                return CSSPrimitiveValue::UnitType::kPixels;

            }
            break;

        case 'v':
            switch (ToASCIILower(data[1])) {
            case 'h':
                return CSSPrimitiveValue::UnitType::kViewportHeight;

            case 'w':
                return CSSPrimitiveValue::UnitType::kViewportWidth;

            }
            break;

        }
        break;

    case 3:
        switch (ToASCIILower(data[0])) {
        case 'd':
            switch (ToASCIILower(data[1])) {
            case 'e':
                if (ToASCIILower(data[2]) == 'g')
                    return CSSPrimitiveValue::UnitType::kDegrees;
                break;

            case 'p':
                if (ToASCIILower(data[2]) == 'i')
                    return CSSPrimitiveValue::UnitType::kDotsPerInch;
                break;

            }
            break;

        case 'k':
            if (ToASCIILower(data[1]) == 'h' && ToASCIILower(data[2]) == 'z')
                return CSSPrimitiveValue::UnitType::kKilohertz;
            break;

        case 'r':
            switch (ToASCIILower(data[1])) {
            case 'a':
                if (ToASCIILower(data[2]) == 'd')
                    return CSSPrimitiveValue::UnitType::kRadians;
                break;

            case 'e':
                if (ToASCIILower(data[2]) == 'm')
                    return CSSPrimitiveValue::UnitType::kRems;
                break;

            }
            break;

        }
        break;

    case 4:
        switch (ToASCIILower(data[0])) {
        case 'd':
            switch (ToASCIILower(data[1])) {
            case 'p':
                switch (ToASCIILower(data[2])) {
                case 'c':
                    if (ToASCIILower(data[3]) == 'm')
                        return CSSPrimitiveValue::UnitType::kDotsPerCentimeter;
                    break;

                case 'p':
                    if (ToASCIILower(data[3]) == 'x')
                        return CSSPrimitiveValue::UnitType::kDotsPerPixel;
                    break;

                }
                break;

            }
            break;

        case 'g':
            if (ToASCIILower(data[1]) == 'r' && ToASCIILower(data[2]) == 'a' && ToASCIILower(data[3]) == 'd')
                return CSSPrimitiveValue::UnitType::kGradians;
            break;

        case 't':
            if (ToASCIILower(data[1]) == 'u' && ToASCIILower(data[2]) == 'r' && ToASCIILower(data[3]) == 'n')
                return CSSPrimitiveValue::UnitType::kTurns;
            break;

        case 'v':
            switch (ToASCIILower(data[1])) {
            case 'm':
                switch (ToASCIILower(data[2])) {
                case 'a':
                    if (ToASCIILower(data[3]) == 'x')
                        return CSSPrimitiveValue::UnitType::kViewportMax;
                    break;

                case 'i':
                    if (ToASCIILower(data[3]) == 'n')
                        return CSSPrimitiveValue::UnitType::kViewportMin;
                    break;

                }
                break;

            }
            break;

        }
        break;

    case 5:
        switch (ToASCIILower(data[0])) {
        case '_':
            if (ToASCIILower(data[1]) == '_' && ToASCIILower(data[2]) == 'q' && ToASCIILower(data[3]) == 'e' && ToASCIILower(data[4]) == 'm')
                return CSSPrimitiveValue::UnitType::kQuirkyEms;
            break;

        }
        break;

    }

  return CSSPrimitiveValue::UnitType::kUnknown;
}

} // namespace

CSSPrimitiveValue::UnitType CSSPrimitiveValue::StringToUnitType(
    const LChar* characters8, unsigned length) {
  return cssPrimitiveValueUnitFromTrie(characters8, length);
}

CSSPrimitiveValue::UnitType CSSPrimitiveValue::StringToUnitType(
    const UChar* characters16, unsigned length) {
  return cssPrimitiveValueUnitFromTrie(characters16, length);
}

} // namespace blink
