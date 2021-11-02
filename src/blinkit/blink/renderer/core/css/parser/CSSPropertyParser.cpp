// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSPropertyParser.cpp
// Description: CSSPropertyParser Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/css/parser/CSSPropertyParser.h"

#include "core/StylePropertyShorthand.h"
#include "core/css/CSSCalculationValue.h"
#include "core/css/CSSCounterValue.h"
#include "core/css/CSSCrossfadeValue.h"
#include "core/css/CSSCursorImageValue.h"
#include "core/css/CSSCustomIdentValue.h"
#include "core/css/CSSFontFaceSrcValue.h"
#include "core/css/CSSFontFeatureValue.h"
#include "core/css/CSSFunctionValue.h"
#include "core/css/CSSImageSetValue.h"
#include "core/css/CSSPathValue.h"
#include "core/css/CSSPrimitiveValueMappings.h"
#include "core/css/CSSQuadValue.h"
// BKTODO: #include "core/css/CSSSVGDocumentValue.h"
#include "core/css/CSSShadowValue.h"
#include "core/css/CSSStringValue.h"
#include "core/css/CSSTimingFunctionValue.h"
#include "core/css/CSSURIValue.h"
#include "core/css/CSSUnicodeRangeValue.h"
#include "core/css/CSSValuePair.h"
#include "core/css/CSSValuePool.h"
#include "core/css/CSSVariableReferenceValue.h"
#include "core/css/FontFace.h"
#include "core/css/HashTools.h"
#include "core/css/parser/CSSParserFastPaths.h"
#include "core/css/parser/CSSParserValues.h"
#include "core/css/parser/CSSVariableParser.h"
#include "core/frame/UseCounter.h"
#include "core/layout/LayoutTheme.h"
// BKTODO: #include "core/svg/SVGPathUtilities.h"
#include "wtf/text/StringBuilder.h"

namespace blink {

CSSPropertyParser::CSSPropertyParser(const CSSParserTokenRange& range,
    const CSSParserContext& context, std::vector<CSSProperty>& parsedProperties)
    : m_range(range)
    , m_context(context)
    , m_parsedProperties(parsedProperties)
    , m_inParseShorthand(0)
    , m_currentShorthand(CSSPropertyInvalid)
    , m_implicitShorthand(false)
{
    m_range.consumeWhitespace();
}

static bool hasInvalidNumericValues(const CSSParserTokenRange& range)
{
    for (const CSSParserToken& token : range) {
        CSSParserTokenType type = token.type();
        if ((type == NumberToken || type == DimensionToken || type == PercentageToken)
            && !CSSPropertyParser::isValidNumericValue(token.numericValue()))
            return true;
    }
    return false;
}

bool CSSPropertyParser::parseValue(CSSPropertyID unresolvedProperty, bool important,
    const CSSParserTokenRange& range, const CSSParserContext& context,
    std::vector<CSSProperty>& parsedProperties, StyleRule::Type ruleType)
{
    if (hasInvalidNumericValues(range))
        return false;
    int parsedPropertiesSize = parsedProperties.size();

    CSSPropertyParser parser(range, context, parsedProperties);
    CSSPropertyID resolvedProperty = resolveCSSPropertyID(unresolvedProperty);
    bool parseSuccess;

    if (ruleType == StyleRule::Viewport) {
        parseSuccess = (RuntimeEnabledFeatures::cssViewportEnabled() || isUASheetBehavior(context.mode()))
            && parser.parseViewportDescriptor(resolvedProperty, important);
    } else if (ruleType == StyleRule::FontFace) {
        parseSuccess = parser.parseFontFaceDescriptor(resolvedProperty);
    } else {
        parseSuccess = parser.parseValueStart(unresolvedProperty, important);
    }

#if 0 // BKTODO:
    // This doesn't count UA style sheets
    if (parseSuccess && context.useCounter())
        context.useCounter()->count(context.mode(), unresolvedProperty);
#endif

    if (!parseSuccess)
    {
        parsedProperties.resize(parsedPropertiesSize);
        parsedProperties.shrink_to_fit();
    }

    return parseSuccess;
}

bool CSSPropertyParser::isValidNumericValue(double value)
{
    return std::isfinite(value)
        && value >= -std::numeric_limits<float>::max()
        && value <= std::numeric_limits<float>::max();
}

bool CSSPropertyParser::parseValueStart(CSSPropertyID unresolvedProperty, bool important)
{
    if (consumeCSSWideKeyword(unresolvedProperty, important))
        return true;

    CSSParserTokenRange originalRange = m_range;
    CSSPropertyID propertyId = resolveCSSPropertyID(unresolvedProperty);

    if (GCRefPtr<CSSValue> parsedValue = parseSingleValue(unresolvedProperty)) {
        if (!m_range.atEnd())
            return false;
        addProperty(propertyId, parsedValue, important);
        return true;
    }

    if (parseShorthand(unresolvedProperty, important))
        return true;

    CSSParserValueList valueList(m_range);
    if (valueList.size()) {
        m_valueList = &valueList;
        if (parseValue(unresolvedProperty, important))
            return true;
    }

    if (RuntimeEnabledFeatures::cssVariablesEnabled() && CSSVariableParser::containsValidVariableReferences(originalRange)) {
        // We don't expand the shorthand here because crazypants.
        GCRefPtr<CSSVariableReferenceValue> variable = CSSVariableReferenceValue::create(CSSVariableData::create(originalRange));
        addProperty(propertyId, variable, important);
        return true;
    }

    return false;
}

bool CSSPropertyParser::isColorKeyword(CSSValueID id)
{
    // Named colors and color keywords:
    //
    // <named-color>
    //   'aqua', 'black', 'blue', ..., 'yellow' (CSS3: "basic color keywords")
    //   'aliceblue', ..., 'yellowgreen'        (CSS3: "extended color keywords")
    //   'transparent'
    //
    // 'currentcolor'
    //
    // <deprecated-system-color>
    //   'ActiveBorder', ..., 'WindowText'
    //
    // WebKit proprietary/internal:
    //   '-webkit-link'
    //   '-webkit-activelink'
    //   '-internal-active-list-box-selection'
    //   '-internal-active-list-box-selection-text'
    //   '-internal-inactive-list-box-selection'
    //   '-internal-inactive-list-box-selection-text'
    //   '-webkit-focus-ring-color'
    //   '-webkit-text'
    //
    return (id >= CSSValueAqua && id <= CSSValueWebkitText)
        || (id >= CSSValueAliceblue && id <= CSSValueYellowgreen)
        || id == CSSValueMenu;
}

bool CSSPropertyParser::isSystemColor(CSSValueID id)
{
    return (id >= CSSValueActiveborder && id <= CSSValueWindowtext) || id == CSSValueMenu;
}

template <typename CharacterType>
static CSSPropertyID unresolvedCSSPropertyID(const CharacterType* propertyName, unsigned length)
{
    char buffer[maxCSSPropertyNameLength + 1]; // 1 for null character

    for (unsigned i = 0; i != length; ++i) {
        CharacterType c = propertyName[i];
        if (c == 0 || c >= 0x7F)
            return CSSPropertyInvalid; // illegal character
        buffer[i] = toASCIILower(c);
    }
    buffer[length] = '\0';

    const char* name = buffer;
    const Property* hashTableEntry = findProperty(name, length);
    if (!hashTableEntry)
        return CSSPropertyInvalid;
    CSSPropertyID property = static_cast<CSSPropertyID>(hashTableEntry->id);
    if (!CSSPropertyMetadata::isEnabledProperty(property))
        return CSSPropertyInvalid;
    return property;
}

CSSPropertyID unresolvedCSSPropertyID(const String& string)
{
    unsigned length = string.length();

    if (!length)
        return CSSPropertyInvalid;
    if (length > maxCSSPropertyNameLength)
        return CSSPropertyInvalid;

    return string.is8Bit() ? unresolvedCSSPropertyID(string.characters8(), length) : unresolvedCSSPropertyID(string.characters16(), length);
}

CSSPropertyID unresolvedCSSPropertyID(const CSSParserString& string)
{
    unsigned length = string.length();

    if (!length)
        return CSSPropertyInvalid;
    if (length > maxCSSPropertyNameLength)
        return CSSPropertyInvalid;

    return string.is8Bit() ? unresolvedCSSPropertyID(string.characters8(), length) : unresolvedCSSPropertyID(string.characters16(), length);
}

template <typename CharacterType>
static CSSValueID cssValueKeywordID(const CharacterType* valueKeyword, unsigned length)
{
    char buffer[maxCSSValueKeywordLength + 1]; // 1 for null character

    for (unsigned i = 0; i != length; ++i) {
        CharacterType c = valueKeyword[i];
        if (c == 0 || c >= 0x7F)
            return CSSValueInvalid; // illegal character
        buffer[i] = WTF::toASCIILower(c);
    }
    buffer[length] = '\0';

    const Value* hashTableEntry = findValue(buffer, length);
    return hashTableEntry ? static_cast<CSSValueID>(hashTableEntry->id) : CSSValueInvalid;
}

CSSValueID cssValueKeywordID(const CSSParserString& string)
{
    unsigned length = string.length();
    if (!length)
        return CSSValueInvalid;
    if (length > maxCSSValueKeywordLength)
        return CSSValueInvalid;

    return string.is8Bit() ? cssValueKeywordID(string.characters8(), length) : cssValueKeywordID(string.characters16(), length);
}

bool CSSPropertyParser::consumeCSSWideKeyword(CSSPropertyID unresolvedProperty, bool important)
{
    CSSParserTokenRange rangeCopy = m_range;
    CSSValueID id = rangeCopy.consumeIncludingWhitespace().id();
    if (!rangeCopy.atEnd())
        return false;

    GCRefPtr<CSSValue> value;
    if (id == CSSValueInitial)
        value = cssValuePool().createExplicitInitialValue();
    else if (id == CSSValueInherit)
        value = cssValuePool().createInheritedValue();
    else if (id == CSSValueUnset)
        value = cssValuePool().createUnsetValue();
    else
        return false;

    addExpandedPropertyForValue(resolveCSSPropertyID(unresolvedProperty), value, important);
    m_range = rangeCopy;
    return true;
}

// Helper methods for consuming tokens starts here.
static bool consumeCommaIncludingWhitespace(CSSParserTokenRange& valueList)
{
    CSSParserToken value = valueList.peek();
    if (value.type() != CommaToken)
        return false;
    valueList.consumeIncludingWhitespace();
    return true;
}

static GCRefPtr<CSSPrimitiveValue> consumeIdent(CSSParserTokenRange& range)
{
    if (range.peek().type() != IdentToken)
        return nullptr;
    return cssValuePool().createIdentifierValue(range.consumeIncludingWhitespace().id());
}

template<typename... emptyBaseCase> inline bool identMatches(CSSValueID id) { return false; }
template<CSSValueID head, CSSValueID... tail> inline bool identMatches(CSSValueID id)
{
    return id == head || identMatches<tail...>(id);
}

template<CSSValueID... names> GCRefPtr<CSSPrimitiveValue> consumeIdent(CSSParserTokenRange& range)
{
    if (range.peek().type() != IdentToken || !identMatches<names...>(range.peek().id()))
        return nullptr;
    return cssValuePool().createIdentifierValue(range.consumeIncludingWhitespace().id());
}

static GCRefPtr<CSSCustomIdentValue> consumeCustomIdent(CSSParserTokenRange& range)
{
    if (range.peek().type() != IdentToken)
        return nullptr;
    return CSSCustomIdentValue::create(range.consumeIncludingWhitespace().value());
}

static GCRefPtr<CSSStringValue> consumeString(CSSParserTokenRange& range)
{
    if (range.peek().type() != StringToken)
        return nullptr;
    return CSSStringValue::create(range.consumeIncludingWhitespace().value());
}

static String consumeUrl(CSSParserTokenRange& range)
{
    const CSSParserToken& token = range.peek();
    if (token.type() == UrlToken) {
        range.consumeIncludingWhitespace();
        return token.value();
    }
    if (token.functionId() == CSSValueUrl) {
        CSSParserTokenRange urlRange = range;
        CSSParserTokenRange urlArgs = urlRange.consumeBlock();
        const CSSParserToken& next = urlArgs.consumeIncludingWhitespace();
        if (next.type() == BadStringToken || !urlArgs.atEnd())
            return String();
        ASSERT(next.type() == StringToken);
        range = urlRange;
        range.consumeWhitespace();
        return next.value();
    }

    return String();
}

static CSSParserTokenRange consumeFunction(CSSParserTokenRange& range)
{
    ASSERT(range.peek().type() == FunctionToken);
    CSSParserTokenRange contents = range.consumeBlock();
    range.consumeWhitespace();
    contents.consumeWhitespace();
    return contents;
}

// TODO(rwlbuis): consider pulling in the parsing logic from CSSCalculationValue.cpp.
class CalcParser {
    STACK_ALLOCATED();

public:
    explicit CalcParser(CSSParserTokenRange& range, ValueRange valueRange = ValueRangeAll)
        : m_sourceRange(range)
        , m_range(range)
    {
        const CSSParserToken& token = range.peek();
        if (token.functionId() == CSSValueCalc || token.functionId() == CSSValueWebkitCalc)
            m_calcValue = CSSCalcValue::create(consumeFunction(m_range), valueRange);
    }

    const CSSCalcValue* value() const { return m_calcValue.get(); }
    GCRefPtr<CSSPrimitiveValue> consumeValue()
    {
        if (!m_calcValue)
            return nullptr;
        m_sourceRange = m_range;
        return CSSPrimitiveValue::create(m_calcValue.release());
    }
    GCRefPtr<CSSPrimitiveValue> consumeNumber()
    {
        if (!m_calcValue)
            return nullptr;
        m_sourceRange = m_range;
        CSSPrimitiveValue::UnitType unitType = m_calcValue->isInt() ? CSSPrimitiveValue::UnitType::Integer : CSSPrimitiveValue::UnitType::Number;
        return cssValuePool().createValue(m_calcValue->doubleValue(), unitType);
    }

    bool consumeNumberRaw(double& result)
    {
        if (!m_calcValue || m_calcValue->category() != CalcNumber)
            return false;
        m_sourceRange = m_range;
        result = m_calcValue->doubleValue();
        return true;
    }

private:
    CSSParserTokenRange& m_sourceRange;
    CSSParserTokenRange m_range;
    GCRefPtr<CSSCalcValue> m_calcValue;
};

static GCRefPtr<CSSPrimitiveValue> consumeInteger(CSSParserTokenRange& range, double minimumValue = -std::numeric_limits<double>::max())
{
    const CSSParserToken& token = range.peek();
    if (token.type() == NumberToken) {
        if (token.numericValueType() == NumberValueType || token.numericValue() < minimumValue)
            return nullptr;
        return cssValuePool().createValue(range.consumeIncludingWhitespace().numericValue(), CSSPrimitiveValue::UnitType::Integer);
    }
    CalcParser calcParser(range);
    if (const CSSCalcValue* calculation = calcParser.value()) {
        if (calculation->category() != CalcNumber || !calculation->isInt())
            return nullptr;
        double value = calculation->doubleValue();
        if (value < minimumValue)
            return nullptr;
        return calcParser.consumeNumber();
    }
    return nullptr;
}

static GCRefPtr<CSSPrimitiveValue> consumePositiveInteger(CSSParserTokenRange& range)
{
    return consumeInteger(range, 1);
}

static bool consumeNumberRaw(CSSParserTokenRange& range, double& result)
{
    if (range.peek().type() == NumberToken) {
        result = range.consumeIncludingWhitespace().numericValue();
        return true;
    }
    CalcParser calcParser(range, ValueRangeAll);
    return calcParser.consumeNumberRaw(result);
}

// TODO(timloh): Work out if this can just call consumeNumberRaw
static GCRefPtr<CSSPrimitiveValue> consumeNumber(CSSParserTokenRange& range, ValueRange valueRange)
{
    const CSSParserToken& token = range.peek();
    if (token.type() == NumberToken) {
        if (valueRange == ValueRangeNonNegative && token.numericValue() < 0)
            return nullptr;
        return cssValuePool().createValue(range.consumeIncludingWhitespace().numericValue(), token.unitType());
    }
    CalcParser calcParser(range, ValueRangeAll);
    if (const CSSCalcValue* calculation = calcParser.value()) {
        // TODO(rwlbuis) Calcs should not be subject to parse time range checks.
        // spec: https://drafts.csswg.org/css-values-3/#calc-range
        if (calculation->category() != CalcNumber || (valueRange == ValueRangeNonNegative && calculation->isNegative()))
            return nullptr;
        return calcParser.consumeNumber();
    }
    return nullptr;
}

inline bool shouldAcceptUnitlessValues(double fValue, CSSParserMode cssParserMode, UnitlessQuirk unitless)
{
    // Quirks mode for certain properties and presentation attributes accept unit-less values for certain units.
    return !fValue // 0 can always be unitless.
        || isUnitLessLengthParsingEnabledForMode(cssParserMode) // HTML and SVG attribute values can always be unitless.
        || (cssParserMode == HTMLQuirksMode && (unitless == UnitlessQuirk::Allow));
}

static GCRefPtr<CSSPrimitiveValue> consumeLength(CSSParserTokenRange& range, CSSParserMode cssParserMode, ValueRange valueRange, UnitlessQuirk unitless = UnitlessQuirk::Forbid)
{
    const CSSParserToken& token = range.peek();
    if (token.type() == DimensionToken) {
        switch (token.unitType()) {
        case CSSPrimitiveValue::UnitType::QuirkyEms:
            if (cssParserMode != UASheetMode)
                return nullptr;
        /* fallthrough intentional */
        case CSSPrimitiveValue::UnitType::Ems:
        case CSSPrimitiveValue::UnitType::Rems:
        case CSSPrimitiveValue::UnitType::Chs:
        case CSSPrimitiveValue::UnitType::Exs:
        case CSSPrimitiveValue::UnitType::Pixels:
        case CSSPrimitiveValue::UnitType::Centimeters:
        case CSSPrimitiveValue::UnitType::Millimeters:
        case CSSPrimitiveValue::UnitType::Inches:
        case CSSPrimitiveValue::UnitType::Points:
        case CSSPrimitiveValue::UnitType::Picas:
        case CSSPrimitiveValue::UnitType::UserUnits:
        case CSSPrimitiveValue::UnitType::ViewportWidth:
        case CSSPrimitiveValue::UnitType::ViewportHeight:
        case CSSPrimitiveValue::UnitType::ViewportMin:
        case CSSPrimitiveValue::UnitType::ViewportMax:
            break;
        default:
            return nullptr;
        }
        if (valueRange == ValueRangeNonNegative && token.numericValue() < 0)
            return nullptr;
        return cssValuePool().createValue(range.consumeIncludingWhitespace().numericValue(), token.unitType());
    }
    if (token.type() == NumberToken) {
        if (!shouldAcceptUnitlessValues(token.numericValue(), cssParserMode, unitless)
            || (valueRange == ValueRangeNonNegative && token.numericValue() < 0))
            return nullptr;
        CSSPrimitiveValue::UnitType unitType = CSSPrimitiveValue::UnitType::Pixels;
        if (cssParserMode == SVGAttributeMode)
            unitType = CSSPrimitiveValue::UnitType::UserUnits;
        return cssValuePool().createValue(range.consumeIncludingWhitespace().numericValue(), unitType);
    }
    if (cssParserMode == SVGAttributeMode)
        return nullptr;
    CalcParser calcParser(range, valueRange);
    if (calcParser.value() && calcParser.value()->category() == CalcLength)
        return calcParser.consumeValue();
    return nullptr;
}

static GCRefPtr<CSSPrimitiveValue> consumePercent(CSSParserTokenRange& range, ValueRange valueRange)
{
    const CSSParserToken& token = range.peek();
    if (token.type() == PercentageToken) {
        if (valueRange == ValueRangeNonNegative && token.numericValue() < 0)
            return nullptr;
        return cssValuePool().createValue(range.consumeIncludingWhitespace().numericValue(), CSSPrimitiveValue::UnitType::Percentage);
    }
    CalcParser calcParser(range, valueRange);
    if (const CSSCalcValue* calculation = calcParser.value()) {
        if (calculation->category() == CalcPercent)
            return calcParser.consumeValue();
    }
    return nullptr;
}

static GCRefPtr<CSSPrimitiveValue> consumeLengthOrPercent(CSSParserTokenRange& range, CSSParserMode cssParserMode, ValueRange valueRange, UnitlessQuirk unitless = UnitlessQuirk::Forbid)
{
    const CSSParserToken& token = range.peek();
    if (token.type() == DimensionToken || token.type() == NumberToken)
        return consumeLength(range, cssParserMode, valueRange, unitless);
    if (token.type() == PercentageToken)
        return consumePercent(range, valueRange);
    CalcParser calcParser(range, valueRange);
    if (const CSSCalcValue* calculation = calcParser.value()) {
        if (calculation->category() == CalcLength || calculation->category() == CalcPercent || calculation->category() == CalcPercentLength)
            return calcParser.consumeValue();
    }
    return nullptr;
}

static GCRefPtr<CSSPrimitiveValue> consumeAngle(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    const CSSParserToken& token = range.peek();
    if (token.type() == DimensionToken) {
        switch (token.unitType()) {
        case CSSPrimitiveValue::UnitType::Degrees:
        case CSSPrimitiveValue::UnitType::Radians:
        case CSSPrimitiveValue::UnitType::Gradians:
        case CSSPrimitiveValue::UnitType::Turns:
            return cssValuePool().createValue(range.consumeIncludingWhitespace().numericValue(), token.unitType());
        default:
            return nullptr;
        }
    }
    if (token.type() == NumberToken) {
        if (!shouldAcceptUnitlessValues(token.numericValue(), cssParserMode, UnitlessQuirk::Forbid))
            return nullptr;
        return cssValuePool().createValue(range.consumeIncludingWhitespace().numericValue(), CSSPrimitiveValue::UnitType::Degrees);
    }
    CalcParser calcParser(range, ValueRangeAll);
    if (const CSSCalcValue* calculation = calcParser.value()) {
        if (calculation->category() == CalcAngle)
            return calcParser.consumeValue();
    }
    return nullptr;
}

static GCRefPtr<CSSPrimitiveValue> consumeTime(CSSParserTokenRange& range, ValueRange valueRange)
{
    const CSSParserToken& token = range.peek();
    if (token.type() == DimensionToken) {
        if (valueRange == ValueRangeNonNegative && token.numericValue() < 0)
            return nullptr;
        CSSPrimitiveValue::UnitType unit = token.unitType();
        if (unit == CSSPrimitiveValue::UnitType::Milliseconds || unit == CSSPrimitiveValue::UnitType::Seconds)
            return cssValuePool().createValue(range.consumeIncludingWhitespace().numericValue(), token.unitType());
        return nullptr;
    }
    CalcParser calcParser(range, valueRange);
    if (const CSSCalcValue* calculation = calcParser.value()) {
        if (calculation->category() == CalcTime)
            return calcParser.consumeValue();
    }
    return nullptr;
}

static int clampRGBComponent(const CSSPrimitiveValue& value)
{
    double result = value.getDoubleValue();
    // TODO(timloh): Multiply by 2.55 and round instead of floor.
    if (value.isPercentage())
        result *= 2.56;
    return clampTo<int>(result, 0, 255);
}

static bool parseRGBParameters(CSSParserTokenRange& range, RGBA32& result, bool parseAlpha)
{
    ASSERT(range.peek().functionId() == CSSValueRgb || range.peek().functionId() == CSSValueRgba);
    CSSParserTokenRange args = consumeFunction(range);
    GCRefPtr<CSSPrimitiveValue> colorParameter = consumeInteger(args);
    if (!colorParameter)
        colorParameter = consumePercent(args, ValueRangeAll);
    if (!colorParameter)
        return false;
    const bool isPercent = colorParameter->isPercentage();
    int colorArray[3];
    colorArray[0] = clampRGBComponent(*colorParameter);
    for (int i = 1; i < 3; i++) {
        if (!consumeCommaIncludingWhitespace(args))
            return false;
        colorParameter = isPercent ? consumePercent(args, ValueRangeAll) : consumeInteger(args);
        if (!colorParameter)
            return false;
        colorArray[i] = clampRGBComponent(*colorParameter);
    }
    if (parseAlpha) {
        if (!consumeCommaIncludingWhitespace(args))
            return false;
        double alpha;
        if (!consumeNumberRaw(args, alpha))
            return false;
        // Convert the floating pointer number of alpha to an integer in the range [0, 256),
        // with an equal distribution across all 256 values.
        int alphaComponent = static_cast<int>(clampTo<double>(alpha, 0.0, 1.0) * nextafter(256.0, 0.0));
        result = makeRGBA(colorArray[0], colorArray[1], colorArray[2], alphaComponent);
    } else {
        result = makeRGB(colorArray[0], colorArray[1], colorArray[2]);
    }
    return args.atEnd();
}

static bool parseHSLParameters(CSSParserTokenRange& range, RGBA32& result, bool parseAlpha)
{
    ASSERT(range.peek().functionId() == CSSValueHsl || range.peek().functionId() == CSSValueHsla);
    CSSParserTokenRange args = consumeFunction(range);
    GCRefPtr<CSSPrimitiveValue> hslValue = consumeNumber(args, ValueRangeAll);
    if (!hslValue)
        return false;
    double colorArray[3];
    colorArray[0] = (((hslValue->getIntValue() % 360) + 360) % 360) / 360.0;
    for (int i = 1; i < 3; i++) {
        if (!consumeCommaIncludingWhitespace(args))
            return false;
        hslValue = consumePercent(args, ValueRangeAll);
        if (!hslValue)
            return false;
        double doubleValue = hslValue->getDoubleValue();
        colorArray[i] = clampTo<double>(doubleValue, 0.0, 100.0) / 100.0; // Needs to be value between 0 and 1.0.
    }
    double alpha = 1.0;
    if (parseAlpha) {
        if (!consumeCommaIncludingWhitespace(args))
            return false;
        if (!consumeNumberRaw(args, alpha))
            return false;
        alpha = clampTo<double>(alpha, 0.0, 1.0);
    }
    result = makeRGBAFromHSLA(colorArray[0], colorArray[1], colorArray[2], alpha);
    return args.atEnd();
}

static bool parseHexColor(CSSParserTokenRange& range, RGBA32& result, bool acceptQuirkyColors)
{
    const CSSParserToken& token = range.peek();
    String color;
    if (acceptQuirkyColors) {
        if (token.type() == NumberToken && token.numericValueType() == IntegerValueType
            && token.numericValue() >= 0. && token.numericValue() < 1000000.) { // e.g. 112233
            color = String::format("%06d", static_cast<int>(token.numericValue()));
        } else if (token.type() == DimensionToken) { // e.g. 0001FF
            color = String::number(static_cast<int>(token.numericValue())) + String(token.value());
            if (color.length() > 6)
                return false;
            while (color.length() < 6)
                color = "0" + color;
        } else if (token.type() == IdentToken) { // e.g. FF0000
            color = token.value();
        }
    }
    if (token.type() == HashToken)
        color = token.value();
    if (!Color::parseHexColor(color, result))
        return false;
    range.consumeIncludingWhitespace();
    return true;
}

static bool parseColorFunction(CSSParserTokenRange& range, RGBA32& result)
{
    CSSValueID functionId = range.peek().functionId();
    if (functionId < CSSValueRgb || functionId > CSSValueHsla)
        return false;
    CSSParserTokenRange colorRange = range;
    if ((functionId <= CSSValueRgba && !parseRGBParameters(colorRange, result, functionId == CSSValueRgba))
        || (functionId >= CSSValueHsl && !parseHSLParameters(colorRange, result, functionId == CSSValueHsla)))
        return false;
    range = colorRange;
    return true;
}

static GCRefPtr<CSSValue> consumeColor(CSSParserTokenRange& range, CSSParserMode cssParserMode, bool acceptQuirkyColors = false)
{
    CSSValueID id = range.peek().id();
    if (CSSPropertyParser::isColorKeyword(id)) {
        if (!isValueAllowedInMode(id, cssParserMode))
            return nullptr;
        return consumeIdent(range);
    }
    RGBA32 color = Color::transparent;
    if (!parseHexColor(range, color, acceptQuirkyColors) && !parseColorFunction(range, color))
        return nullptr;
    return cssValuePool().createColorValue(color);
}

static GCRefPtr<CSSPrimitiveValue> consumePositionComponent(CSSParserTokenRange& range, CSSParserMode cssParserMode, UnitlessQuirk unitless)
{
    if (range.peek().type() == IdentToken)
        return consumeIdent<CSSValueLeft, CSSValueTop, CSSValueBottom, CSSValueRight, CSSValueCenter>(range);
    return consumeLengthOrPercent(range, cssParserMode, ValueRangeAll, unitless);
}

static bool isHorizontalPositionKeywordOnly(const CSSPrimitiveValue& value)
{
    return value.isValueID() && (value.getValueID() == CSSValueLeft || value.getValueID() == CSSValueRight);
}

static bool isVerticalPositionKeywordOnly(const CSSPrimitiveValue& value)
{
    return value.isValueID() && (value.getValueID() == CSSValueTop || value.getValueID() == CSSValueBottom);
}

static void positionFromOneValue(const GCRefPtr<CSSPrimitiveValue> &value, GCRefPtr<CSSValue> &resultX, GCRefPtr<CSSValue> &resultY)
{
    bool valueAppliesToYAxisOnly = isVerticalPositionKeywordOnly(*value);
    resultX = value;
    resultY = cssValuePool().createIdentifierValue(CSSValueCenter);
    if (valueAppliesToYAxisOnly)
        resultX.swap(resultY);
}

static bool positionFromTwoValues(const GCRefPtr<CSSPrimitiveValue> &value1, const GCRefPtr<CSSPrimitiveValue> &value2,
    GCRefPtr<CSSValue> &resultX, GCRefPtr<CSSValue> &resultY)
{
    bool mustOrderAsXY = isHorizontalPositionKeywordOnly(*value1) || isVerticalPositionKeywordOnly(*value2)
        || !value1->isValueID() || !value2->isValueID();
    bool mustOrderAsYX = isVerticalPositionKeywordOnly(*value1) || isHorizontalPositionKeywordOnly(*value2);
    if (mustOrderAsXY && mustOrderAsYX)
        return false;
    resultX = value1;
    resultY = value2;
    if (mustOrderAsYX)
        resultX.swap(resultY);
    return true;
}

static bool positionFromThreeOrFourValues(CSSPrimitiveValue** values, GCRefPtr<CSSValue>& resultX, GCRefPtr<CSSValue>& resultY)
{
    CSSPrimitiveValue* center = nullptr;
    for (int i = 0; values[i]; i++) {
        CSSPrimitiveValue* currentValue = values[i];
        if (!currentValue->isValueID())
            return false;
        CSSValueID id = currentValue->getValueID();

        if (id == CSSValueCenter) {
            if (center)
                return false;
            center = currentValue;
            continue;
        }

        GCRefPtr<CSSValue> result;
        if (values[i + 1] && !values[i + 1]->isValueID()) {
            result = CSSValuePair::create(currentValue, values[++i], CSSValuePair::KeepIdenticalValues);
        } else {
            result = currentValue;
        }

        if (id == CSSValueLeft || id == CSSValueRight) {
            if (resultX)
                return false;
            resultX = result.release();
        } else {
            ASSERT(id == CSSValueTop || id == CSSValueBottom);
            if (resultY)
                return false;
            resultY = result.release();
        }
    }

    if (center) {
        ASSERT(resultX || resultY);
        if (resultX && resultY)
            return false;
        if (!resultX)
            resultX = center;
        else
            resultY = center;
    }

    ASSERT(resultX && resultY);
    return true;
}

// This may consume from the range upon failure since no caller needs the stricter behaviour.
static bool consumePosition(CSSParserTokenRange& range, CSSParserMode cssParserMode, UnitlessQuirk unitless, GCRefPtr<CSSValue>& resultX, GCRefPtr<CSSValue>& resultY)
{
    GCRefPtr<CSSPrimitiveValue> value1 = consumePositionComponent(range, cssParserMode, unitless);
    if (!value1)
        return false;

    GCRefPtr<CSSPrimitiveValue> value2 = consumePositionComponent(range, cssParserMode, unitless);
    if (!value2) {
        positionFromOneValue(value1, resultX, resultY);
        return true;
    }

    GCRefPtr<CSSPrimitiveValue> value3 = consumePositionComponent(range, cssParserMode, unitless);
    if (!value3)
        return positionFromTwoValues(value1, value2, resultX, resultY);

    GCRefPtr<CSSPrimitiveValue> value4 = consumePositionComponent(range, cssParserMode, unitless);
    CSSPrimitiveValue* values[5];
    values[0] = value1.get();
    values[1] = value2.get();
    values[2] = value3.get();
    values[3] = value4.get();
    values[4] = nullptr;
    return positionFromThreeOrFourValues(values, resultX, resultY);
}

static GCRefPtr<CSSValue> consumePosition(CSSParserTokenRange& range, CSSParserMode cssParserMode, UnitlessQuirk unitless)
{
    GCRefPtr<CSSValue> resultX, resultY;
    if (consumePosition(range, cssParserMode, unitless, resultX, resultY))
        return CSSValuePair::create(resultX, resultY, CSSValuePair::KeepIdenticalValues);
    return nullptr;
}

static bool consumeOneOrTwoValuedPosition(CSSParserTokenRange& range, CSSParserMode cssParserMode, UnitlessQuirk unitless, GCRefPtr<CSSValue>& resultX, GCRefPtr<CSSValue>& resultY)
{
    GCRefPtr<CSSPrimitiveValue> value1 = consumePositionComponent(range, cssParserMode, unitless);
    if (!value1)
        return false;
    GCRefPtr<CSSPrimitiveValue> value2 = consumePositionComponent(range, cssParserMode, unitless);
    if (!value2) {
        positionFromOneValue(value1, resultX, resultY);
        return true;
    }
    return positionFromTwoValues(value1, value2, resultX, resultY);
}

static GCRefPtr<CSSValueList> consumeTransformOrigin(CSSParserTokenRange& range, CSSParserMode cssParserMode, UnitlessQuirk unitless)
{
    GCRefPtr<CSSValue> resultX, resultY;
    if (consumeOneOrTwoValuedPosition(range, cssParserMode, unitless, resultX, resultY)) {
        GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
        list->append(resultX.release());
        list->append(resultY.release());
        GCRefPtr<CSSValue> resultZ = consumeLength(range, cssParserMode, ValueRangeAll);
        if (!resultZ)
            resultZ = cssValuePool().createValue(0, CSSPrimitiveValue::UnitType::Pixels);
        list->append(resultZ);
        return list;
    }
    return nullptr;
}

static inline bool isCSSWideKeyword(const CSSValueID& id)
{
    return id == CSSValueInitial || id == CSSValueInherit || id == CSSValueUnset || id == CSSValueDefault;
}

// Methods for consuming non-shorthand properties starts here.
static GCRefPtr<CSSValue> consumeWillChange(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);

    GCRefPtr<CSSValueList> values = CSSValueList::createCommaSeparated();
    // Every comma-separated list of identifiers is a valid will-change value,
    // unless the list includes an explicitly disallowed identifier.
    while (true) {
        if (range.peek().type() != IdentToken)
            return nullptr;
        CSSPropertyID unresolvedProperty = unresolvedCSSPropertyID(range.peek().value());
        if (unresolvedProperty) {
            ASSERT(CSSPropertyMetadata::isEnabledProperty(unresolvedProperty));
            // Now "all" is used by both CSSValue and CSSPropertyValue.
            // Need to return nullptr when currentValue is CSSPropertyAll.
            if (unresolvedProperty == CSSPropertyWillChange || unresolvedProperty == CSSPropertyAll)
                return nullptr;
            values->append(cssValuePool().createIdentifierValue(unresolvedProperty));
            range.consumeIncludingWhitespace();
        } else {
            switch (range.peek().id()) {
            case CSSValueNone:
            case CSSValueAll:
            case CSSValueAuto:
            case CSSValueDefault:
            case CSSValueInitial:
            case CSSValueInherit:
                return nullptr;
            case CSSValueContents:
            case CSSValueScrollPosition:
                values->append(consumeIdent(range));
                break;
            default:
                range.consumeIncludingWhitespace();
                break;
            }
        }

        if (range.atEnd())
            break;
        if (!consumeCommaIncludingWhitespace(range))
            return nullptr;
    }

    return values;
}

static GCRefPtr<CSSFontFeatureValue> consumeFontFeatureTag(CSSParserTokenRange& range)
{
    // Feature tag name consists of 4-letter characters.
    static const unsigned tagNameLength = 4;

    const CSSParserToken& token = range.consumeIncludingWhitespace();
    // Feature tag name comes first
    if (token.type() != StringToken)
        return nullptr;
    if (token.value().length() != tagNameLength)
        return nullptr;
    AtomicString tag = token.value();
    for (unsigned i = 0; i < tagNameLength; ++i) {
        // Limits the range of characters to 0x20-0x7E, following the tag name rules defiend in the OpenType specification.
        UChar character = tag[i];
        if (character < 0x20 || character > 0x7E)
            return nullptr;
    }

    int tagValue = 1;
    // Feature tag values could follow: <integer> | on | off
    if (range.peek().type() == NumberToken && range.peek().numericValueType() == IntegerValueType && range.peek().numericValue() >= 0) {
        tagValue = clampTo<int>(range.consumeIncludingWhitespace().numericValue());
        if (tagValue < 0)
            return nullptr;
    } else if (range.peek().id() == CSSValueOn || range.peek().id() == CSSValueOff) {
        tagValue = range.consumeIncludingWhitespace().id() == CSSValueOn;
    }
    return CSSFontFeatureValue::create(tag, tagValue);
}

static GCRefPtr<CSSValue> consumeFontFeatureSettings(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueNormal)
        return consumeIdent(range);
    GCRefPtr<CSSValueList> settings = CSSValueList::createCommaSeparated();
    do {
        GCRefPtr<CSSFontFeatureValue> fontFeatureValue = consumeFontFeatureTag(range);
        if (!fontFeatureValue)
            return nullptr;
        settings->append(fontFeatureValue);
    } while (consumeCommaIncludingWhitespace(range));
    return settings;
}

static GCRefPtr<CSSValue> consumePage(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);
    return consumeCustomIdent(range);
}

static GCRefPtr<CSSValue> consumeQuotes(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);
    GCRefPtr<CSSValueList> values = CSSValueList::createSpaceSeparated();
    while (!range.atEnd()) {
        GCRefPtr<CSSStringValue> parsedValue = consumeString(range);
        if (!parsedValue)
            return nullptr;
        values->append(parsedValue);
    }
    if (values->length() && values->length() % 2 == 0)
        return values;
    return nullptr;
}

static GCRefPtr<CSSValue> consumeWebkitHighlight(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);
    return consumeString(range);
}

static GCRefPtr<CSSValue> consumeFontVariantLigatures(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueNormal)
        return consumeIdent(range);
    GCRefPtr<CSSValueList> ligatureValues = CSSValueList::createSpaceSeparated();
    bool sawCommonLigaturesValue = false;
    bool sawDiscretionaryLigaturesValue = false;
    bool sawHistoricalLigaturesValue = false;
    bool sawContextualLigaturesValue = false;
    do {
        CSSValueID id = range.peek().id();
        switch (id) {
        case CSSValueNoCommonLigatures:
        case CSSValueCommonLigatures:
            if (sawCommonLigaturesValue)
                return nullptr;
            sawCommonLigaturesValue = true;
            break;
        case CSSValueNoDiscretionaryLigatures:
        case CSSValueDiscretionaryLigatures:
            if (sawDiscretionaryLigaturesValue)
                return nullptr;
            sawDiscretionaryLigaturesValue = true;
            break;
        case CSSValueNoHistoricalLigatures:
        case CSSValueHistoricalLigatures:
            if (sawHistoricalLigaturesValue)
                return nullptr;
            sawHistoricalLigaturesValue = true;
            break;
        case CSSValueNoContextual:
        case CSSValueContextual:
            if (sawContextualLigaturesValue)
                return nullptr;
            sawContextualLigaturesValue = true;
            break;
        default:
            return nullptr;
        }
        ligatureValues->append(consumeIdent(range));
    } while (!range.atEnd());

    return ligatureValues;
}

static GCRefPtr<CSSPrimitiveValue> consumeFontVariant(CSSParserTokenRange& range)
{
    return consumeIdent<CSSValueNormal, CSSValueSmallCaps>(range);
}

static GCRefPtr<CSSValue> consumeFontVariantList(CSSParserTokenRange& range)
{
    GCRefPtr<CSSValueList> values = CSSValueList::createCommaSeparated();
    do {
        if (range.peek().id() == CSSValueAll) {
            // FIXME: CSSPropertyParser::parseFontVariant() implements
            // the old css3 draft:
            // http://www.w3.org/TR/2002/WD-css3-webfonts-20020802/#font-variant
            // 'all' is only allowed in @font-face and with no other values.
            if (values->length())
                return nullptr;
            return consumeIdent(range);
        }
        GCRefPtr<CSSPrimitiveValue> fontVariant = consumeFontVariant(range);
        if (fontVariant)
            values->append(fontVariant);
    } while (consumeCommaIncludingWhitespace(range));

    if (values->length())
        return values;

    return nullptr;
}

static GCRefPtr<CSSPrimitiveValue> consumeFontWeight(CSSParserTokenRange& range)
{
    const CSSParserToken& token = range.peek();
    if (token.id() >= CSSValueNormal && token.id() <= CSSValueLighter)
        return consumeIdent(range);
    if (token.type() != NumberToken || token.numericValueType() != IntegerValueType)
        return nullptr;
    int weight = static_cast<int>(token.numericValue());
    if ((weight % 100) || weight < 100 || weight > 900)
        return nullptr;
    range.consumeIncludingWhitespace();
    return cssValuePool().createIdentifierValue(static_cast<CSSValueID>(CSSValue100 + weight / 100 - 1));
}

static String concatenateFamilyName(CSSParserTokenRange& range)
{
    StringBuilder builder;
    bool addedSpace = false;
    const CSSParserToken& firstToken = range.peek();
    while (range.peek().type() == IdentToken) {
        if (!builder.isEmpty()) {
            builder.append(' ');
            addedSpace = true;
        }
        builder.append(range.consumeIncludingWhitespace().value());
    }
    if (!addedSpace && isCSSWideKeyword(firstToken.id()))
        return String();
    return builder.toString();
}

static GCRefPtr<CSSValue> consumeFamilyName(CSSParserTokenRange& range)
{
    if (range.peek().type() == StringToken)
        return cssValuePool().createFontFamilyValue(range.consumeIncludingWhitespace().value());
    if (range.peek().type() != IdentToken)
        return nullptr;
    String familyName = concatenateFamilyName(range);
    if (familyName.isNull())
        return nullptr;
    return cssValuePool().createFontFamilyValue(familyName);
}

static GCRefPtr<CSSValue> consumeGenericFamily(CSSParserTokenRange& range)
{
    return consumeIdent<CSSValueSerif, CSSValueSansSerif, CSSValueCursive, CSSValueFantasy, CSSValueMonospace, CSSValueWebkitBody>(range);
}

static GCRefPtr<CSSValueList> consumeFontFamily(CSSParserTokenRange& range)
{
    GCRefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
    do {
        GCRefPtr<CSSValue> parsedValue;
        if ((parsedValue = consumeGenericFamily(range))) {
            list->append(parsedValue);
        } else if ((parsedValue = consumeFamilyName(range))) {
            list->append(parsedValue);
        } else {
            return nullptr;
        }
    } while (consumeCommaIncludingWhitespace(range));
    return list;
}

static GCRefPtr<CSSValue> consumeSpacing(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().id() == CSSValueNormal)
        return consumeIdent(range);
    // TODO(timloh): Don't allow unitless values, and allow <percentage>s in word-spacing.
    return consumeLength(range, cssParserMode, ValueRangeAll, UnitlessQuirk::Allow);
}

static GCRefPtr<CSSValue> consumeTabSize(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    GCRefPtr<CSSPrimitiveValue> parsedValue = consumeInteger(range, 0);
    if (parsedValue)
        return parsedValue;
    return consumeLength(range, cssParserMode, ValueRangeNonNegative);
}

static GCRefPtr<CSSValue> consumeFontSize(CSSParserTokenRange& range, CSSParserMode cssParserMode, UnitlessQuirk unitless = UnitlessQuirk::Forbid)
{
    if (range.peek().id() >= CSSValueXxSmall && range.peek().id() <= CSSValueLarger)
        return consumeIdent(range);
    return consumeLengthOrPercent(range, cssParserMode, ValueRangeNonNegative, unitless);
}

static GCRefPtr<CSSPrimitiveValue> consumeLineHeight(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().id() == CSSValueNormal)
        return consumeIdent(range);

    GCRefPtr<CSSPrimitiveValue> lineHeight = consumeNumber(range, ValueRangeNonNegative);
    if (lineHeight)
        return lineHeight;
    return consumeLengthOrPercent(range, cssParserMode, ValueRangeNonNegative);
}

static GCRefPtr<CSSValueList> consumeRotation(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    ASSERT(RuntimeEnabledFeatures::cssIndependentTransformPropertiesEnabled());
    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();

    GCRefPtr<CSSValue> rotation = consumeAngle(range, cssParserMode);
    if (!rotation)
        return nullptr;
    list->append(rotation);

    if (range.atEnd())
        return list;

    for (unsigned i = 0; i < 3; i++) { // 3 dimensions of rotation
        GCRefPtr<CSSValue> dimension = consumeNumber(range, ValueRangeAll);
        if (!dimension)
            return nullptr;
        list->append(dimension);
    }

    return list;
}

static GCRefPtr<CSSValueList> consumeScale(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    ASSERT(RuntimeEnabledFeatures::cssIndependentTransformPropertiesEnabled());

    GCRefPtr<CSSValue> scale = consumeNumber(range, ValueRangeAll);
    if (!scale)
        return nullptr;
    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    list->append(scale);
    if ((scale = consumeNumber(range, ValueRangeAll))) {
        list->append(scale);
        if ((scale = consumeNumber(range, ValueRangeAll)))
            list->append(scale);
    }

    return list;
}

static GCRefPtr<CSSValueList> consumeTranslate(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    ASSERT(RuntimeEnabledFeatures::cssIndependentTransformPropertiesEnabled());
    GCRefPtr<CSSValue> translate = consumeLengthOrPercent(range, cssParserMode, ValueRangeAll);
    if (!translate)
        return nullptr;
    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    list->append(translate);
    if ((translate = consumeLengthOrPercent(range, cssParserMode, ValueRangeAll))) {
        list->append(translate);
        if ((translate = consumeLength(range, cssParserMode, ValueRangeAll)))
            list->append(translate);
    }

    return list;
}

static GCRefPtr<CSSValue> consumeCounter(CSSParserTokenRange& range, CSSParserMode cssParserMode, int defaultValue)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);

    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    do {
        GCRefPtr<CSSCustomIdentValue> counterName = consumeCustomIdent(range);
        if (!counterName)
            return nullptr;
        int i = defaultValue;
        if (GCRefPtr<CSSPrimitiveValue> counterValue = consumeInteger(range))
            i = clampTo<int>(counterValue->getDoubleValue());
        list->append(CSSValuePair::create(counterName,
            cssValuePool().createValue(i, CSSPrimitiveValue::UnitType::Integer),
            CSSValuePair::DropIdenticalValues));
    } while (!range.atEnd());
    return list;
}

static GCRefPtr<CSSValue> consumePageSize(CSSParserTokenRange& range)
{
    return consumeIdent<CSSValueA3, CSSValueA4, CSSValueA5, CSSValueB4, CSSValueB5, CSSValueLedger, CSSValueLegal, CSSValueLetter>(range);
}

static GCRefPtr<CSSValueList> consumeSize(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    GCRefPtr<CSSValueList> result = CSSValueList::createSpaceSeparated();

    if (range.peek().id() == CSSValueAuto) {
        result->append(consumeIdent(range));
        return result;
    }

    if (GCRefPtr<CSSValue> width = consumeLength(range, cssParserMode, ValueRangeNonNegative)) {
        GCRefPtr<CSSValue> height = consumeLength(range, cssParserMode, ValueRangeNonNegative);
        result->append(width);
        if (height)
            result->append(height);
        return result;
    }

    GCRefPtr<CSSValue> pageSize = consumePageSize(range);
    GCRefPtr<CSSValue> orientation = consumeIdent<CSSValuePortrait, CSSValueLandscape>(range);
    if (!pageSize)
        pageSize = consumePageSize(range);

    if (!orientation && !pageSize)
        return nullptr;
    if (pageSize)
        result->append(pageSize);
    if (orientation)
        result->append(orientation);
    return result;
}

static GCRefPtr<CSSValue> consumeTextIndent(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    // [ <length> | <percentage> ] && hanging? && each-line?
    // Keywords only allowed when css3Text is enabled.
    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();

    bool hasLengthOrPercentage = false;
    bool hasEachLine = false;
    bool hasHanging = false;

    do {
        if (!hasLengthOrPercentage) {
            if (GCRefPtr<CSSValue> textIndent = consumeLengthOrPercent(range, cssParserMode, ValueRangeAll, UnitlessQuirk::Allow)) {
                list->append(textIndent);
                hasLengthOrPercentage = true;
                continue;
            }
        }

        if (RuntimeEnabledFeatures::css3TextEnabled()) {
            CSSValueID id = range.peek().id();
            if (!hasEachLine && id == CSSValueEachLine) {
                list->append(consumeIdent(range));
                hasEachLine = true;
                continue;
            }
            if (!hasHanging && id == CSSValueHanging) {
                list->append(consumeIdent(range));
                hasHanging = true;
                continue;
            }
        }
        return nullptr;
    } while (!range.atEnd());

    if (!hasLengthOrPercentage)
        return nullptr;

    return list;
}

static bool validWidthOrHeightKeyword(CSSValueID id, const CSSParserContext& context)
{
    if (id == CSSValueWebkitMinContent || id == CSSValueWebkitMaxContent || id == CSSValueWebkitFillAvailable || id == CSSValueWebkitFitContent
        || id == CSSValueMinContent || id == CSSValueMaxContent || id == CSSValueFitContent) {
#if 0 // BKTODO:
        if (context.useCounter()) {
            switch (id) {
            case CSSValueWebkitMinContent:
                context.useCounter()->count(UseCounter::CSSValuePrefixedMinContent);
                break;
            case CSSValueWebkitMaxContent:
                context.useCounter()->count(UseCounter::CSSValuePrefixedMaxContent);
                break;
            case CSSValueWebkitFillAvailable:
                context.useCounter()->count(UseCounter::CSSValuePrefixedFillAvailable);
                break;
            case CSSValueWebkitFitContent:
                context.useCounter()->count(UseCounter::CSSValuePrefixedFitContent);
                break;
            default:
                break;
            }
        }
#endif
        return true;
    }
    return false;
}

static GCRefPtr<CSSValue> consumeMaxWidthOrHeight(CSSParserTokenRange& range, const CSSParserContext& context, UnitlessQuirk unitless = UnitlessQuirk::Forbid)
{
    if (range.peek().id() == CSSValueNone || validWidthOrHeightKeyword(range.peek().id(), context))
        return consumeIdent(range);
    return consumeLengthOrPercent(range, context.mode(), ValueRangeNonNegative, unitless);
}

static GCRefPtr<CSSValue> consumeWidthOrHeight(CSSParserTokenRange& range, const CSSParserContext& context, UnitlessQuirk unitless = UnitlessQuirk::Forbid)
{
    if (range.peek().id() == CSSValueAuto || validWidthOrHeightKeyword(range.peek().id(), context))
        return consumeIdent(range);
    return consumeLengthOrPercent(range, context.mode(), ValueRangeNonNegative, unitless);
}

static GCRefPtr<CSSValue> consumeMarginWidth(CSSParserTokenRange& range, CSSParserMode cssParserMode, UnitlessQuirk unitless)
{
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);
    return consumeLengthOrPercent(range, cssParserMode, ValueRangeAll, unitless);
}

static GCRefPtr<CSSPrimitiveValue> consumeClipComponent(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);
    return consumeLength(range, cssParserMode, ValueRangeAll, UnitlessQuirk::Allow);
}

static GCRefPtr<CSSValue> consumeClip(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);

    if (range.peek().functionId() != CSSValueRect)
        return nullptr;

    CSSParserTokenRange args = consumeFunction(range);
    // rect(t, r, b, l) || rect(t r b l)
    GCRefPtr<CSSPrimitiveValue> top = consumeClipComponent(args, cssParserMode);
    if (!top)
        return nullptr;
    bool needsComma = consumeCommaIncludingWhitespace(args);
    GCRefPtr<CSSPrimitiveValue> right = consumeClipComponent(args, cssParserMode);
    if (!right || (needsComma && !consumeCommaIncludingWhitespace(args)))
        return nullptr;
    GCRefPtr<CSSPrimitiveValue> bottom = consumeClipComponent(args, cssParserMode);
    if (!bottom || (needsComma && !consumeCommaIncludingWhitespace(args)))
        return nullptr;
    GCRefPtr<CSSPrimitiveValue> left = consumeClipComponent(args, cssParserMode);
    if (!left || !args.atEnd())
        return nullptr;
    return CSSQuadValue::create(top, right, bottom, left, CSSQuadValue::SerializeAsRect);
}

static bool consumePan(CSSParserTokenRange& range, GCRefPtr<CSSValue>& panX, GCRefPtr<CSSValue>& panY)
{
    CSSValueID id = range.peek().id();
    if ((id == CSSValuePanX || id == CSSValuePanRight || id == CSSValuePanLeft) && !panX) {
        if (id != CSSValuePanX && !RuntimeEnabledFeatures::cssTouchActionPanDirectionsEnabled())
            return false;
        panX = consumeIdent(range);
    } else if ((id == CSSValuePanY || id == CSSValuePanDown || id == CSSValuePanUp) && !panY) {
        if (id != CSSValuePanY && !RuntimeEnabledFeatures::cssTouchActionPanDirectionsEnabled())
            return false;
        panY = consumeIdent(range);
    } else {
        return false;
    }
    return true;
}

static GCRefPtr<CSSValue> consumeTouchAction(CSSParserTokenRange& range)
{
    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    CSSValueID id = range.peek().id();
    if (id == CSSValueAuto || id == CSSValueNone || id == CSSValueManipulation) {
        list->append(consumeIdent(range));
        return list;
    }

    GCRefPtr<CSSValue> panX, panY;
    if (!consumePan(range, panX, panY))
        return nullptr;
    if (!range.atEnd() && !consumePan(range, panX, panY))
        return nullptr;

    if (panX)
        list->append(panX);
    if (panY)
        list->append(panY);
    return list;
}

static GCRefPtr<CSSPrimitiveValue> consumeLineClamp(CSSParserTokenRange& range)
{
    if (range.peek().type() != PercentageToken && range.peek().type() != NumberToken)
        return nullptr;
    GCRefPtr<CSSPrimitiveValue> clampValue = consumePercent(range, ValueRangeNonNegative);
    if (clampValue)
        return clampValue;
    // When specifying number of lines, don't allow 0 as a valid value.
    return consumePositiveInteger(range);
}

static GCRefPtr<CSSValue> consumeLocale(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);
    return consumeString(range);
}

static GCRefPtr<CSSValue> consumeColumnWidth(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);
    // Always parse lengths in strict mode here, since it would be ambiguous otherwise when used in
    // the 'columns' shorthand property.
    GCRefPtr<CSSPrimitiveValue> columnWidth = consumeLength(range, HTMLStandardMode, ValueRangeNonNegative);
    if (!columnWidth || (!columnWidth->isCalculated() && columnWidth->getDoubleValue() == 0))
        return nullptr;
    return columnWidth;
}

static GCRefPtr<CSSValue> consumeColumnCount(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);
    return consumePositiveInteger(range);
}

static GCRefPtr<CSSValue> consumeColumnGap(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().id() == CSSValueNormal)
        return consumeIdent(range);
    return consumeLength(range, cssParserMode, ValueRangeNonNegative);
}

static GCRefPtr<CSSValue> consumeColumnSpan(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    CSSValueID id = range.peek().id();
    if (id == CSSValueAll || id == CSSValueNone)
        return consumeIdent(range);
    if (range.peek().type() != NumberToken)
        return nullptr;
    if (GCRefPtr<CSSPrimitiveValue> spanValue = consumeInteger(range)) {
        // 1 (will be dropped in the unprefixed property).
        if (spanValue->getIntValue() == 1)
            return spanValue;
    }
    return nullptr;
}

static GCRefPtr<CSSValue> consumeZoom(CSSParserTokenRange& range, const CSSParserContext& context)
{
    const CSSParserToken& token = range.peek();
    GCRefPtr<CSSPrimitiveValue> zoom;
    if (token.type() == IdentToken) {
        zoom = consumeIdent<CSSValueNormal, CSSValueReset, CSSValueDocument>(range);
    } else {
        zoom = consumePercent(range, ValueRangeNonNegative);
        if (!zoom)
            zoom = consumeNumber(range, ValueRangeNonNegative);
    }
#if 0 // BKTODO:
    if (zoom && context.useCounter()
        && !(token.id() == CSSValueNormal
            || (token.type() == NumberToken && zoom->getDoubleValue() == 1)
            || (token.type() == PercentageToken && zoom->getDoubleValue() == 100)))
        context.useCounter()->count(UseCounter::CSSZoomNotEqualToOne);
#endif
    return zoom;
}

static GCRefPtr<CSSValue> consumeAnimationIterationCount(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueInfinite)
        return consumeIdent(range);
    return consumeNumber(range, ValueRangeNonNegative);
}

static GCRefPtr<CSSValue> consumeAnimationName(CSSParserTokenRange& range, const CSSParserContext& context, bool allowQuotedName)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);

    if (allowQuotedName && range.peek().type() == StringToken) {
#if 0 // BKTODO:
        // Legacy support for strings in prefixed animations.
        if (context.useCounter())
            context.useCounter()->count(UseCounter::QuotedAnimationName);
#endif

        const CSSParserToken& token = range.consumeIncludingWhitespace();
        if (token.valueEqualsIgnoringCase("none"))
            return cssValuePool().createIdentifierValue(CSSValueNone);
        return CSSCustomIdentValue::create(token.value());
    }

    return consumeCustomIdent(range);
}

static GCRefPtr<CSSValue> consumeTransitionProperty(CSSParserTokenRange& range)
{
    const CSSParserToken& token = range.peek();
    if (token.type() != IdentToken)
        return nullptr;
    // TODO(timloh): This should check isCSSWideKeyword
    if (token.id() == CSSValueInitial || token.id() == CSSValueInherit)
        return nullptr;
    if (token.id() == CSSValueNone)
        return consumeIdent(range);

    if (CSSPropertyID property = token.parseAsUnresolvedCSSPropertyID()) {
        ASSERT(CSSPropertyMetadata::isEnabledProperty(property));
        range.consumeIncludingWhitespace();
        return cssValuePool().createIdentifierValue(property);
    }
    return consumeCustomIdent(range);
}

static GCRefPtr<CSSValue> consumeSteps(CSSParserTokenRange& range)
{
    ASSERT(range.peek().functionId() == CSSValueSteps);
    CSSParserTokenRange rangeCopy = range;
    CSSParserTokenRange args = consumeFunction(rangeCopy);

    GCRefPtr<CSSPrimitiveValue> steps = consumePositiveInteger(args);
    if (!steps)
        return nullptr;

    StepsTimingFunction::StepAtPosition position = StepsTimingFunction::End;
    if (consumeCommaIncludingWhitespace(args)) {
        switch (args.consumeIncludingWhitespace().id()) {
        case CSSValueMiddle:
            if (!RuntimeEnabledFeatures::webAnimationsAPIEnabled())
                return nullptr;
            position = StepsTimingFunction::Middle;
            break;
        case CSSValueStart:
            position = StepsTimingFunction::Start;
            break;
        case CSSValueEnd:
            position = StepsTimingFunction::End;
            break;
        default:
            return nullptr;
        }
    }

    if (!args.atEnd())
        return nullptr;

    range = rangeCopy;
    return CSSStepsTimingFunctionValue::create(steps->getIntValue(), position);
}

static GCRefPtr<CSSValue> consumeCubicBezier(CSSParserTokenRange& range)
{
    ASSERT(range.peek().functionId() == CSSValueCubicBezier);
    CSSParserTokenRange rangeCopy = range;
    CSSParserTokenRange args = consumeFunction(rangeCopy);

    double x1, y1, x2, y2;
    if (consumeNumberRaw(args, x1)
        && x1 >= 0 && x1 <= 1
        && consumeCommaIncludingWhitespace(args)
        && consumeNumberRaw(args, y1)
        && consumeCommaIncludingWhitespace(args)
        && consumeNumberRaw(args, x2)
        && x2 >= 0 && x2 <= 1
        && consumeCommaIncludingWhitespace(args)
        && consumeNumberRaw(args, y2)
        && args.atEnd()) {
        range = rangeCopy;
        return CSSCubicBezierTimingFunctionValue::create(x1, y1, x2, y2);
    }

    return nullptr;
}

static GCRefPtr<CSSValue> consumeAnimationTimingFunction(CSSParserTokenRange& range)
{
    CSSValueID id = range.peek().id();
    if (id == CSSValueEase || id == CSSValueLinear || id == CSSValueEaseIn
        || id == CSSValueEaseOut || id == CSSValueEaseInOut || id == CSSValueStepStart
        || id == CSSValueStepEnd || id == CSSValueStepMiddle)
        return consumeIdent(range);

    CSSValueID function = range.peek().functionId();
    if (function == CSSValueSteps)
        return consumeSteps(range);
    if (function == CSSValueCubicBezier)
        return consumeCubicBezier(range);
    return nullptr;
}

static GCRefPtr<CSSValue> consumeAnimationValue(CSSPropertyID property, CSSParserTokenRange& range, const CSSParserContext& context, bool useLegacyParsing)
{
    switch (property) {
    case CSSPropertyAnimationDelay:
    case CSSPropertyTransitionDelay:
        return consumeTime(range, ValueRangeAll);
    case CSSPropertyAnimationDirection:
        return consumeIdent<CSSValueNormal, CSSValueAlternate, CSSValueReverse, CSSValueAlternateReverse>(range);
    case CSSPropertyAnimationDuration:
    case CSSPropertyTransitionDuration:
        return consumeTime(range, ValueRangeNonNegative);
    case CSSPropertyAnimationFillMode:
        return consumeIdent<CSSValueNone, CSSValueForwards, CSSValueBackwards, CSSValueBoth>(range);
    case CSSPropertyAnimationIterationCount:
        return consumeAnimationIterationCount(range);
    case CSSPropertyAnimationName:
        return consumeAnimationName(range, context, useLegacyParsing);
    case CSSPropertyAnimationPlayState:
        return consumeIdent<CSSValueRunning, CSSValuePaused>(range);
    case CSSPropertyTransitionProperty:
        return consumeTransitionProperty(range);
    case CSSPropertyAnimationTimingFunction:
    case CSSPropertyTransitionTimingFunction:
        return consumeAnimationTimingFunction(range);
    default:
        ASSERT_NOT_REACHED();
        return nullptr;
    }
}

static bool isValidAnimationPropertyList(CSSPropertyID property, const CSSValueList& valueList)
{
    if (property != CSSPropertyTransitionProperty || valueList.length() < 2)
        return true;
    for (auto& value : valueList) {
        if (value->isPrimitiveValue() && toCSSPrimitiveValue(*value).isValueID()
            && toCSSPrimitiveValue(*value).getValueID() == CSSValueNone)
            return false;
    }
    return true;
}

static GCRefPtr<CSSValueList> consumeAnimationPropertyList(CSSPropertyID property, CSSParserTokenRange& range, const CSSParserContext& context, bool useLegacyParsing)
{
    GCRefPtr<CSSValueList> list = CSSValueList::createCommaSeparated();
    do {
        GCRefPtr<CSSValue> value = consumeAnimationValue(property, range, context, useLegacyParsing);
        if (!value)
            return nullptr;
        list->append(value);
    } while (consumeCommaIncludingWhitespace(range));
    if (!isValidAnimationPropertyList(property, *list))
        return nullptr;
    ASSERT(list->length());
    return list;
}

bool CSSPropertyParser::consumeAnimationShorthand(const StylePropertyShorthand& shorthand, bool useLegacyParsing, bool important)
{
    const unsigned longhandCount = shorthand.length();
    GCRefPtr<CSSValueList> longhands[8];
    ASSERT(longhandCount <= 8);
    for (size_t i = 0; i < longhandCount; ++i)
        longhands[i] = CSSValueList::createCommaSeparated();

    do {
        bool parsedLonghand[8] = { false };
        do {
            bool foundProperty = false;
            for (size_t i = 0; i < longhandCount; ++i) {
                if (parsedLonghand[i])
                    continue;

                if (GCRefPtr<CSSValue> value = consumeAnimationValue(shorthand.properties()[i], m_range, m_context, useLegacyParsing)) {
                    parsedLonghand[i] = true;
                    foundProperty = true;
                    longhands[i]->append(value);
                    break;
                }
            }
            if (!foundProperty)
                return false;
        } while (!m_range.atEnd() && m_range.peek().type() != CommaToken);

        // TODO(timloh): This will make invalid longhands, see crbug.com/386459
        for (size_t i = 0; i < longhandCount; ++i) {
            if (!parsedLonghand[i])
                longhands[i]->append(cssValuePool().createImplicitInitialValue());
            parsedLonghand[i] = false;
        }
    } while (consumeCommaIncludingWhitespace(m_range));

    for (size_t i = 0; i < longhandCount; ++i) {
        if (!isValidAnimationPropertyList(shorthand.properties()[i], *longhands[i]))
            return false;
    }

    for (size_t i = 0; i < longhandCount; ++i)
        addProperty(shorthand.properties()[i], longhands[i], important);

    return m_range.atEnd();
}

static GCRefPtr<CSSValue> consumeWidowsOrOrphans(CSSParserTokenRange& range)
{
    // Support for auto is non-standard and for backwards compatibility.
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);
    return consumePositiveInteger(range);
}

static GCRefPtr<CSSValue> consumeZIndex(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);
    return consumeInteger(range);
}

static GCRefPtr<CSSShadowValue> parseSingleShadow(CSSParserTokenRange& range, CSSParserMode cssParserMode, bool allowInset, bool allowSpread)
{
    GCRefPtr<CSSPrimitiveValue> style;
    GCRefPtr<CSSValue> color;

    if (range.atEnd())
        return nullptr;
    if (range.peek().id() == CSSValueInset) {
        if (!allowInset)
            return nullptr;
        style = consumeIdent(range);
    }
    color = consumeColor(range, cssParserMode);

    GCRefPtr<CSSPrimitiveValue> horizontalOffset = consumeLength(range, cssParserMode, ValueRangeAll);
    if (!horizontalOffset)
        return nullptr;

    GCRefPtr<CSSPrimitiveValue> verticalOffset = consumeLength(range, cssParserMode, ValueRangeAll);
    if (!verticalOffset)
        return nullptr;

    GCRefPtr<CSSPrimitiveValue> blurRadius = consumeLength(range, cssParserMode, ValueRangeAll);
    GCRefPtr<CSSPrimitiveValue> spreadDistance;
    if (blurRadius) {
        // Blur radius must be non-negative.
        if (blurRadius->getDoubleValue() < 0)
            return nullptr;
        if (allowSpread)
            spreadDistance = consumeLength(range, cssParserMode, ValueRangeAll);
    }

    if (!range.atEnd()) {
        if (!color)
            color = consumeColor(range, cssParserMode);
        if (range.peek().id() == CSSValueInset) {
            if (!allowInset || style)
                return nullptr;
            style = consumeIdent(range);
        }
    }
    return CSSShadowValue::create(horizontalOffset, verticalOffset, blurRadius,
        spreadDistance, style, color);
}

static GCRefPtr<CSSValue> consumeShadow(CSSParserTokenRange& range, CSSParserMode cssParserMode, bool isBoxShadowProperty)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);

    GCRefPtr<CSSValueList> shadowValueList = CSSValueList::createCommaSeparated();
    do {
        if (GCRefPtr<CSSShadowValue> shadowValue = parseSingleShadow(range, cssParserMode, isBoxShadowProperty, isBoxShadowProperty))
            shadowValueList->append(shadowValue);
        else
            return nullptr;
    } while (consumeCommaIncludingWhitespace(range));
    return shadowValueList;
}

static GCRefPtr<CSSFunctionValue> consumeFilterFunction(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    CSSValueID filterType = range.peek().functionId();
    if (filterType < CSSValueInvert || filterType > CSSValueDropShadow)
        return nullptr;
    CSSParserTokenRange args = consumeFunction(range);
    GCRefPtr<CSSFunctionValue> filterValue = CSSFunctionValue::create(filterType);
    GCRefPtr<CSSValue> parsedValue;

    if (filterType == CSSValueDropShadow) {
        parsedValue = parseSingleShadow(args, cssParserMode, false, false);
    } else {
        // TODO(timloh): Add UseCounters for empty filter arguments.
        if (args.atEnd())
            return filterValue;
        if (filterType == CSSValueBrightness) {
            // FIXME (crbug.com/397061): Support calc expressions like calc(10% + 0.5)
            parsedValue = consumePercent(args, ValueRangeAll);
            if (!parsedValue)
                parsedValue = consumeNumber(args, ValueRangeAll);
        } else if (filterType == CSSValueHueRotate) {
            parsedValue = consumeAngle(args, cssParserMode);
        } else if (filterType == CSSValueBlur) {
            parsedValue = consumeLength(args, HTMLStandardMode, ValueRangeNonNegative);
        } else {
            // FIXME (crbug.com/397061): Support calc expressions like calc(10% + 0.5)
            parsedValue = consumePercent(args, ValueRangeNonNegative);
            if (!parsedValue)
                parsedValue = consumeNumber(args, ValueRangeNonNegative);
            if (parsedValue && filterType != CSSValueSaturate && filterType != CSSValueContrast) {
                double maxAllowed = toCSSPrimitiveValue(parsedValue.get())->isPercentage() ? 100.0 : 1.0;
                if (toCSSPrimitiveValue(parsedValue.get())->getDoubleValue() > maxAllowed)
                    return nullptr;
            }
        }
    }
    if (!parsedValue || !args.atEnd())
        return nullptr;
    filterValue->append(parsedValue);
    return filterValue;
}

static GCRefPtr<CSSValue> consumeFilter(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);

    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    do {
        String url = consumeUrl(range);
        GCRefPtr<CSSFunctionValue> filterValue;
        if (!url.isNull()) {
            filterValue = CSSFunctionValue::create(CSSValueUrl);
            ASSERT(false); // BKTODO: filterValue->append(CSSSVGDocumentValue::create(url));
        } else {
            filterValue = consumeFilterFunction(range, cssParserMode);
            if (!filterValue)
                return nullptr;
        }
        list->append(filterValue);
    } while (!range.atEnd());
    return list;
}

static GCRefPtr<CSSValue> consumeTextDecorationLine(CSSParserTokenRange& range)
{
    CSSValueID id = range.peek().id();
    if (id == CSSValueNone)
        return consumeIdent(range);

    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    GCRefPtr<CSSPrimitiveValue> ident;
    while ((ident = consumeIdent<CSSValueBlink, CSSValueUnderline, CSSValueOverline, CSSValueLineThrough>(range))) {
        if (list->hasValue(ident.get()))
            return nullptr;
        list->append(ident);
    }

    if (!list->length())
        return nullptr;
    return list;
}

// none | strict | [ layout || style || paint ]
static GCRefPtr<CSSValue> consumeContain(CSSParserTokenRange& range)
{
    CSSValueID id = range.peek().id();
    if (id == CSSValueNone)
        return consumeIdent(range);

    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    if (id == CSSValueStrict) {
        list->append(consumeIdent(range));
        return list;
    }
    GCRefPtr<CSSPrimitiveValue> ident;
    while ((ident = consumeIdent<CSSValuePaint, CSSValueLayout, CSSValueStyle>(range))) {
        if (list->hasValue(ident.get()))
            return nullptr;
        list->append(ident);
    }

    if (!list->length())
        return nullptr;
    return list;
}

static GCRefPtr<CSSValue> consumePath(CSSParserTokenRange& range)
{
    // FIXME: Add support for <url>, <basic-shape>, <geometry-box>.
    if (range.peek().functionId() != CSSValuePath)
        return nullptr;

    // FIXME: Add support for <fill-rule>.
    CSSParserTokenRange functionRange = range;
    CSSParserTokenRange functionArgs = consumeFunction(functionRange);

    if (functionArgs.peek().type() != StringToken)
        return nullptr;
    String pathString = functionArgs.consumeIncludingWhitespace().value();

    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    RefPtr<SVGPathByteStream> byteStream = SVGPathByteStream::create();
    if (!buildByteStreamFromString(pathString, *byteStream) || !functionArgs.atEnd())
        return nullptr;

    range = functionRange;
    return CSSPathValue::create(byteStream.release());
#endif
}

static GCRefPtr<CSSValue> consumePathOrNone(CSSParserTokenRange& range)
{
    CSSValueID id = range.peek().id();
    if (id == CSSValueNone)
        return consumeIdent(range);

    return consumePath(range);
}

static GCRefPtr<CSSValue> consumeMotionRotation(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    GCRefPtr<CSSValue> angle = consumeAngle(range, cssParserMode);
    GCRefPtr<CSSValue> keyword = consumeIdent<CSSValueAuto, CSSValueReverse>(range);
    if (!angle && !keyword)
        return nullptr;

    if (!angle)
        angle = consumeAngle(range, cssParserMode);

    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    if (keyword)
        list->append(keyword);
    if (angle)
        list->append(angle);
    return list;
}

static GCRefPtr<CSSValue> consumeTextEmphasisStyle(CSSParserTokenRange& range)
{
    CSSValueID id = range.peek().id();
    if (id == CSSValueNone)
        return consumeIdent(range);

    if (GCRefPtr<CSSValue> textEmphasisStyle = consumeString(range))
        return textEmphasisStyle;

    GCRefPtr<CSSPrimitiveValue> fill = consumeIdent<CSSValueFilled, CSSValueOpen>(range);
    GCRefPtr<CSSPrimitiveValue> shape = consumeIdent<CSSValueDot, CSSValueCircle, CSSValueDoubleCircle, CSSValueTriangle, CSSValueSesame>(range);
    if (!fill)
        fill = consumeIdent<CSSValueFilled, CSSValueOpen>(range);
    if (fill && shape) {
        GCRefPtr<CSSValueList> parsedValues = CSSValueList::createSpaceSeparated();
        parsedValues->append(fill.release());
        parsedValues->append(shape.release());
        return parsedValues;
    }
    if (fill)
        return fill;
    if (shape)
        return shape;
    return nullptr;
}

static GCRefPtr<CSSValue> consumeOutlineColor(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    // Outline color has "invert" as additional keyword.
    // Also, we want to allow the special focus color even in HTML Standard parsing mode.
    if (range.peek().id() == CSSValueInvert || range.peek().id() == CSSValueWebkitFocusRingColor)
        return consumeIdent(range);
    return consumeColor(range, cssParserMode);
}

static GCRefPtr<CSSPrimitiveValue> consumeLineWidth(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    CSSValueID id = range.peek().id();
    if (id == CSSValueThin || id == CSSValueMedium || id == CSSValueThick)
        return consumeIdent(range);
    return consumeLength(range, cssParserMode, ValueRangeNonNegative);
}

static GCRefPtr<CSSPrimitiveValue> consumeBorderWidth(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    return consumeLineWidth(range, cssParserMode);
}

static GCRefPtr<CSSPrimitiveValue> consumeTextStrokeWidth(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    return consumeLineWidth(range, cssParserMode);
}

static GCRefPtr<CSSPrimitiveValue> consumeColumnRuleWidth(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    return consumeLineWidth(range, cssParserMode);
}

static bool consumeTranslate3d(CSSParserTokenRange& args, CSSParserMode cssParserMode, GCRefPtr<CSSFunctionValue>& transformValue)
{
    unsigned numberOfArguments = 2;
    GCRefPtr<CSSValue> parsedValue;
    do {
        parsedValue = consumeLengthOrPercent(args, cssParserMode, ValueRangeAll);
        if (!parsedValue)
            return false;
        transformValue->append(parsedValue);
        if (!consumeCommaIncludingWhitespace(args))
            return false;
    } while (--numberOfArguments);
    parsedValue = consumeLength(args, cssParserMode, ValueRangeAll);
    if (!parsedValue)
        return false;
    transformValue->append(parsedValue);
    return true;
}

static bool consumeNumbers(CSSParserTokenRange& args, GCRefPtr<CSSFunctionValue>& transformValue, unsigned numberOfArguments)
{
    do {
        GCRefPtr<CSSValue> parsedValue = consumeNumber(args, ValueRangeAll);
        if (!parsedValue)
            return false;
        transformValue->append(parsedValue);
        if (--numberOfArguments && !consumeCommaIncludingWhitespace(args))
            return false;
    } while (numberOfArguments);
    return true;
}

static bool consumePerspective(CSSParserTokenRange& args, CSSParserMode cssParserMode, GCRefPtr<CSSFunctionValue>& transformValue, bool useLegacyParsing)
{
    GCRefPtr<CSSPrimitiveValue> parsedValue = consumeLength(args, cssParserMode, ValueRangeNonNegative);
    if (!parsedValue && useLegacyParsing) {
        double perspective;
        if (!consumeNumberRaw(args, perspective) || perspective < 0)
            return false;
        parsedValue = cssValuePool().createValue(perspective, CSSPrimitiveValue::UnitType::Pixels);
    }
    if (!parsedValue)
        return false;
    transformValue->append(parsedValue);
    return true;
}

static GCRefPtr<CSSValue> consumeTransformValue(CSSParserTokenRange& range, CSSParserMode cssParserMode, bool useLegacyParsing)
{
    CSSValueID functionId = range.peek().functionId();
    if (functionId == CSSValueInvalid)
        return nullptr;
    CSSParserTokenRange args = consumeFunction(range);
    if (args.atEnd())
        return nullptr;
    GCRefPtr<CSSFunctionValue> transformValue = CSSFunctionValue::create(functionId);
    GCRefPtr<CSSValue> parsedValue;
    switch (functionId) {
    case CSSValueRotate:
    case CSSValueRotateX:
    case CSSValueRotateY:
    case CSSValueRotateZ:
    case CSSValueSkewX:
    case CSSValueSkewY:
    case CSSValueSkew:
        parsedValue = consumeAngle(args, cssParserMode);
        if (!parsedValue)
            return nullptr;
        if (functionId == CSSValueSkew && consumeCommaIncludingWhitespace(args)) {
            transformValue->append(parsedValue);
            parsedValue = consumeAngle(args, cssParserMode);
        }
        break;
    case CSSValueScaleX:
    case CSSValueScaleY:
    case CSSValueScaleZ:
    case CSSValueScale:
        parsedValue = consumeNumber(args, ValueRangeAll);
        if (!parsedValue)
            return nullptr;
        if (functionId == CSSValueScale && consumeCommaIncludingWhitespace(args)) {
            transformValue->append(parsedValue);
            parsedValue = consumeNumber(args, ValueRangeAll);
        }
        break;
    case CSSValuePerspective:
        if (!consumePerspective(args, cssParserMode, transformValue, useLegacyParsing))
            return nullptr;
        break;
    case CSSValueTranslateX:
    case CSSValueTranslateY:
    case CSSValueTranslate:
        parsedValue = consumeLengthOrPercent(args, cssParserMode, ValueRangeAll);
        if (!parsedValue)
            return nullptr;
        if (functionId == CSSValueTranslate && consumeCommaIncludingWhitespace(args)) {
            transformValue->append(parsedValue);
            parsedValue = consumeLengthOrPercent(args, cssParserMode, ValueRangeAll);
        }
        break;
    case CSSValueTranslateZ:
        parsedValue = consumeLength(args, cssParserMode, ValueRangeAll);
        break;
    case CSSValueMatrix:
    case CSSValueMatrix3d:
        if (!consumeNumbers(args, transformValue, (functionId == CSSValueMatrix3d) ? 16 : 6))
            return nullptr;
        break;
    case CSSValueScale3d:
        if (!consumeNumbers(args, transformValue, 3))
            return nullptr;
        break;
    case CSSValueRotate3d:
        if (!consumeNumbers(args, transformValue, 3) || !consumeCommaIncludingWhitespace(args))
            return nullptr;
        parsedValue = consumeAngle(args, cssParserMode);
        break;
    case CSSValueTranslate3d:
        if (!consumeTranslate3d(args, cssParserMode, transformValue))
            return nullptr;
        break;
    default:
        return nullptr;
    }
    if (parsedValue)
        transformValue->append(parsedValue);
    if (!args.atEnd())
        return nullptr;
    return transformValue;
}

static GCRefPtr<CSSValue> consumeTransform(CSSParserTokenRange& range, CSSParserMode cssParserMode, bool useLegacyParsing)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);

    GCRefPtr<CSSValueList> list = CSSValueList::createSpaceSeparated();
    do {
        GCRefPtr<CSSValue> parsedTransformValue = consumeTransformValue(range, cssParserMode, useLegacyParsing);
        if (!parsedTransformValue)
            return nullptr;
        list->append(parsedTransformValue);
    } while (!range.atEnd());

    return list;
}

template <CSSValueID start, CSSValueID end>
static GCRefPtr<CSSValue> consumePositionLonghand(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().type() == IdentToken) {
        CSSValueID id = range.peek().id();
        int percent;
        if (id == start)
            percent = 0;
        else if (id == CSSValueCenter)
            percent = 50;
        else if (id == end)
            percent = 100;
        else
            return nullptr;
        range.consumeIncludingWhitespace();
        return cssValuePool().createValue(percent, CSSPrimitiveValue::UnitType::Percentage);
    }
    return consumeLengthOrPercent(range, cssParserMode, ValueRangeAll, UnitlessQuirk::Forbid);
}

static GCRefPtr<CSSValue> consumePositionX(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    return consumePositionLonghand<CSSValueLeft, CSSValueRight>(range, cssParserMode);
}

static GCRefPtr<CSSValue> consumePositionY(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    return consumePositionLonghand<CSSValueTop, CSSValueBottom>(range, cssParserMode);
}

static GCRefPtr<CSSValue> consumePaint(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);
    String url = consumeUrl(range);
    if (!url.isNull()) {
        GCRefPtr<CSSValue> parsedValue;
        if (range.peek().id() == CSSValueNone)
            parsedValue = consumeIdent(range);
        else
            parsedValue = consumeColor(range, cssParserMode);
        if (parsedValue) {
            GCRefPtr<CSSValueList> values = CSSValueList::createSpaceSeparated();
            values->append(CSSURIValue::create(url));
            values->append(parsedValue);
            return values;
        }
        return CSSURIValue::create(url);
    }
    return consumeColor(range, cssParserMode);
}

static GCRefPtr<CSSValue> consumePaintOrder(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueNormal)
        return consumeIdent(range);

    std::vector<CSSValueID> paintTypeList;
    GCRefPtr<CSSPrimitiveValue> fill, stroke, markers;
    do {
        CSSValueID id = range.peek().id();
        if (id == CSSValueFill && !fill)
            fill = consumeIdent(range);
        else if (id == CSSValueStroke && !stroke)
            stroke = consumeIdent(range);
        else if (id == CSSValueMarkers && !markers)
            markers = consumeIdent(range);
        else
            return nullptr;
        paintTypeList.emplace_back(id);
    } while (!range.atEnd());

    // After parsing we serialize the paint-order list. Since it is not possible to
    // pop a last list items from CSSValueList without bigger cost, we create the
    // list after parsing.
    CSSValueID firstPaintOrderType = paintTypeList.at(0);
    GCRefPtr<CSSValueList> paintOrderList = CSSValueList::createSpaceSeparated();
    switch (firstPaintOrderType) {
    case CSSValueFill:
    case CSSValueStroke:
        paintOrderList->append(firstPaintOrderType == CSSValueFill ? fill : stroke);
        if (paintTypeList.size() > 1) {
            if (paintTypeList.at(1) == CSSValueMarkers)
                paintOrderList->append(markers);
        }
        break;
    case CSSValueMarkers:
        paintOrderList->append(markers.release());
        if (paintTypeList.size() > 1) {
            if (paintTypeList.at(1) == CSSValueStroke)
                paintOrderList->append(stroke);
        }
        break;
    default:
        ASSERT_NOT_REACHED();
    }

    return paintOrderList;
}

static GCRefPtr<CSSValue> consumeNoneOrURI(CSSParserTokenRange& range)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);

    String url = consumeUrl(range);
    if (url.isNull())
        return nullptr;
    return CSSURIValue::create(url);
}

static GCRefPtr<CSSValue> consumeFlexBasis(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    // FIXME: Support intrinsic dimensions too.
    if (range.peek().id() == CSSValueAuto)
        return consumeIdent(range);
    return consumeLengthOrPercent(range, cssParserMode, ValueRangeNonNegative);
}

static GCRefPtr<CSSValue> consumeStrokeDasharray(CSSParserTokenRange& range)
{
    CSSValueID id = range.peek().id();
    if (id == CSSValueNone)
        return consumeIdent(range);

    GCRefPtr<CSSValueList> dashes = CSSValueList::createCommaSeparated();
    do {
        GCRefPtr<CSSPrimitiveValue> dash = consumeLengthOrPercent(range, SVGAttributeMode, ValueRangeNonNegative, UnitlessQuirk::Allow);
        if (!dash || (consumeCommaIncludingWhitespace(range) && range.atEnd()))
            return nullptr;
        dashes->append(dash);
    } while (!range.atEnd());
    return dashes;
}

static GCRefPtr<CSSPrimitiveValue> consumeBaselineShift(CSSParserTokenRange& range)
{
    CSSValueID id = range.peek().id();
    if (id == CSSValueBaseline || id == CSSValueSub || id == CSSValueSuper)
        return consumeIdent(range);
    return consumeLengthOrPercent(range, SVGAttributeMode, ValueRangeAll, UnitlessQuirk::Forbid);
}

static GCRefPtr<CSSValue> consumeImageSet(CSSParserTokenRange& range, const CSSParserContext& context)
{
    CSSParserTokenRange rangeCopy = range;
    CSSParserTokenRange args = consumeFunction(rangeCopy);
    GCRefPtr<CSSImageSetValue> imageSet = CSSImageSetValue::create();
    do {
        AtomicString urlValue(consumeUrl(args));
        if (urlValue.isNull())
            return nullptr;

        GCRefPtr<CSSValue> image = CSSPropertyParser::createCSSImageValueWithReferrer(urlValue, context);
        imageSet->append(image);

        const CSSParserToken& token = args.consumeIncludingWhitespace();
        if (token.type() != DimensionToken)
            return nullptr;
        if (String(token.value()) != "x")
            return nullptr;
        ASSERT(token.unitType() == CSSPrimitiveValue::UnitType::Unknown);
        double imageScaleFactor = token.numericValue();
        if (imageScaleFactor <= 0)
            return nullptr;
        imageSet->append(cssValuePool().createValue(imageScaleFactor, CSSPrimitiveValue::UnitType::Number));
    } while (consumeCommaIncludingWhitespace(args));
    if (!args.atEnd())
        return nullptr;
    range = rangeCopy;
    return imageSet;
}

static GCRefPtr<CSSValue> consumeCursor(CSSParserTokenRange& range, const CSSParserContext& context, bool inQuirksMode)
{
    GCRefPtr<CSSValueList> list;
    while (true) {
        GCRefPtr<CSSValue> image;
        AtomicString uri(consumeUrl(range));
        if (!uri.isNull()) {
            image = CSSPropertyParser::createCSSImageValueWithReferrer(uri, context);
        } else if (range.peek().type() == FunctionToken && range.peek().functionId() == CSSValueWebkitImageSet) {
            image = consumeImageSet(range, context);
            if (!image)
                return nullptr;
        } else {
            break;
        }

        double num;
        IntPoint hotSpot(-1, -1);
        bool hotSpotSpecified = false;
        if (consumeNumberRaw(range, num)) {
            hotSpot.setX(int(num));
            if (!consumeNumberRaw(range, num))
                return nullptr;
            hotSpot.setY(int(num));
            hotSpotSpecified = true;
        }

        if (!list)
            list = CSSValueList::createCommaSeparated();

        list->append(CSSCursorImageValue::create(image, hotSpotSpecified, hotSpot));
        if (!consumeCommaIncludingWhitespace(range))
            return nullptr;
    }

    CSSValueID id = range.peek().id();
#if 0 // BKTODO:
    if (!range.atEnd() && context.useCounter()) {
        if (id == CSSValueWebkitZoomIn)
            context.useCounter()->count(UseCounter::PrefixedCursorZoomIn);
        else if (id == CSSValueWebkitZoomOut)
            context.useCounter()->count(UseCounter::PrefixedCursorZoomOut);
    }
#endif
    GCRefPtr<CSSValue> cursorType;
    if (id == CSSValueHand) {
        if (!inQuirksMode) // Non-standard behavior
            return nullptr;
        cursorType = cssValuePool().createIdentifierValue(CSSValuePointer);
        range.consumeIncludingWhitespace();
    } else if ((id >= CSSValueAuto && id <= CSSValueWebkitZoomOut) || id == CSSValueCopy || id == CSSValueNone) {
        cursorType = consumeIdent(range);
    } else {
        return nullptr;
    }

    if (!list)
        return cursorType;
    list->append(cursorType.release());
    return list;
}

// This should go away once we drop support for -webkit-gradient
static GCRefPtr<CSSPrimitiveValue> consumeDeprecatedGradientPoint(CSSParserTokenRange& args, bool horizontal)
{
    if (args.peek().type() == IdentToken) {
        if ((horizontal && consumeIdent<CSSValueLeft>(args)) || (!horizontal && consumeIdent<CSSValueTop>(args)))
            return cssValuePool().createValue(0., CSSPrimitiveValue::UnitType::Percentage);
        if ((horizontal && consumeIdent<CSSValueRight>(args)) || (!horizontal && consumeIdent<CSSValueBottom>(args)))
            return cssValuePool().createValue(100., CSSPrimitiveValue::UnitType::Percentage);
        if (consumeIdent<CSSValueCenter>(args))
            return cssValuePool().createValue(50., CSSPrimitiveValue::UnitType::Percentage);
        return nullptr;
    }
    GCRefPtr<CSSPrimitiveValue> result = consumePercent(args, ValueRangeAll);
    if (!result)
        result = consumeNumber(args, ValueRangeAll);
    return result;
}

// Used to parse colors for -webkit-gradient(...).
static GCRefPtr<CSSValue> consumeDeprecatedGradientStopColor(CSSParserTokenRange& args, CSSParserMode cssParserMode)
{
    if (args.peek().id() == CSSValueCurrentcolor)
        return nullptr;
    return consumeColor(args, cssParserMode);
}

static bool consumeDeprecatedGradientColorStop(CSSParserTokenRange& range, CSSGradientColorStop& stop, CSSParserMode cssParserMode)
{
    CSSValueID id = range.peek().functionId();
    if (id != CSSValueFrom && id != CSSValueTo && id != CSSValueColorStop)
        return false;

    CSSParserTokenRange args = consumeFunction(range);
    double position;
    if (id == CSSValueFrom || id == CSSValueTo) {
        position = (id == CSSValueFrom) ? 0 : 1;
    } else {
        ASSERT(id == CSSValueColorStop);
        const CSSParserToken& arg = args.consumeIncludingWhitespace();
        if (arg.type() == PercentageToken)
            position = arg.numericValue() / 100.0;
        else if (arg.type() == NumberToken)
            position = arg.numericValue();
        else
            return false;

        if (!consumeCommaIncludingWhitespace(args))
            return false;
    }

    stop.m_position = cssValuePool().createValue(position, CSSPrimitiveValue::UnitType::Number);
    stop.m_color = consumeDeprecatedGradientStopColor(args, cssParserMode);
    return stop.m_color && args.atEnd();
}

static GCRefPtr<CSSValue> consumeDeprecatedGradient(CSSParserTokenRange& args, CSSParserMode cssParserMode)
{
    GCRefPtr<CSSGradientValue> result;
    CSSValueID id = args.consumeIncludingWhitespace().id();
    bool isDeprecatedRadialGradient = (id == CSSValueRadial);
    if (isDeprecatedRadialGradient)
        result = CSSRadialGradientValue::create(NonRepeating, CSSDeprecatedRadialGradient);
    else if (id == CSSValueLinear)
        result = CSSLinearGradientValue::create(NonRepeating, CSSDeprecatedLinearGradient);
    if (!result || !consumeCommaIncludingWhitespace(args))
        return nullptr;

    GCRefPtr<CSSPrimitiveValue> point = consumeDeprecatedGradientPoint(args, true);
    if (!point)
        return nullptr;
    result->setFirstX(point);
    point = consumeDeprecatedGradientPoint(args, false);
    if (!point)
        return nullptr;
    result->setFirstY(point.release());

    if (!consumeCommaIncludingWhitespace(args))
        return nullptr;

    // For radial gradients only, we now expect a numeric radius.
    if (isDeprecatedRadialGradient) {
        GCRefPtr<CSSPrimitiveValue> radius = consumeNumber(args, ValueRangeAll);
        if (!radius || !consumeCommaIncludingWhitespace(args))
            return nullptr;
        toCSSRadialGradientValue(result.get())->setFirstRadius(radius);
    }

    point = consumeDeprecatedGradientPoint(args, true);
    if (!point)
        return nullptr;
    result->setSecondX(point);
    point = consumeDeprecatedGradientPoint(args, false);
    if (!point)
        return nullptr;
    result->setSecondY(point);

    // For radial gradients only, we now expect the second radius.
    if (isDeprecatedRadialGradient) {
        if (!consumeCommaIncludingWhitespace(args))
            return nullptr;
        GCRefPtr<CSSPrimitiveValue> radius = consumeNumber(args, ValueRangeAll);
        if (!radius)
            return nullptr;
        toCSSRadialGradientValue(result.get())->setSecondRadius(radius);
    }

    CSSGradientColorStop stop;
    while (consumeCommaIncludingWhitespace(args)) {
        if (!consumeDeprecatedGradientColorStop(args, stop, cssParserMode))
            return nullptr;
        result->addStop(stop);
    }

    return result;
}

static bool consumeGradientColorStops(CSSParserTokenRange& range, CSSParserMode cssParserMode, CSSGradientValue* gradient)
{
    bool supportsColorHints = gradient->gradientType() == CSSLinearGradient || gradient->gradientType() == CSSRadialGradient;

    // The first color stop cannot be a color hint.
    bool previousStopWasColorHint = true;
    do {
        CSSGradientColorStop stop;
        stop.m_color = consumeColor(range, cssParserMode);
        // Two hints in a row are not allowed.
        if (!stop.m_color && (!supportsColorHints || previousStopWasColorHint))
            return false;
        previousStopWasColorHint = !stop.m_color;
        stop.m_position = consumeLengthOrPercent(range, cssParserMode, ValueRangeAll);
        if (!stop.m_color && !stop.m_position)
            return false;
        gradient->addStop(stop);
    } while (consumeCommaIncludingWhitespace(range));

    // The last color stop cannot be a color hint.
    if (previousStopWasColorHint)
        return false;

    // Must have 2 or more stops to be valid.
    return gradient->stopCount() >= 2;
}

static GCRefPtr<CSSValue> consumeDeprecatedRadialGradient(CSSParserTokenRange& args, CSSParserMode cssParserMode, CSSGradientRepeat repeating)
{
    GCRefPtr<CSSRadialGradientValue> result = CSSRadialGradientValue::create(repeating, CSSPrefixedRadialGradient);
    GCRefPtr<CSSValue> centerX, centerY;
    consumeOneOrTwoValuedPosition(args, cssParserMode, UnitlessQuirk::Forbid, centerX, centerY);
    if ((centerX || centerY) && !consumeCommaIncludingWhitespace(args))
        return nullptr;

    result->setFirstX(toCSSPrimitiveValue(centerX.get()));
    result->setSecondX(toCSSPrimitiveValue(centerX.get()));
    result->setFirstY(toCSSPrimitiveValue(centerY.get()));
    result->setSecondY(toCSSPrimitiveValue(centerY.get()));

    GCRefPtr<CSSPrimitiveValue> shape = consumeIdent<CSSValueCircle, CSSValueEllipse>(args);
    GCRefPtr<CSSPrimitiveValue> sizeKeyword = consumeIdent<CSSValueClosestSide, CSSValueClosestCorner, CSSValueFarthestSide, CSSValueFarthestCorner, CSSValueContain, CSSValueCover>(args);
    if (!shape)
        shape = consumeIdent<CSSValueCircle, CSSValueEllipse>(args);
    result->setShape(shape);
    result->setSizingBehavior(sizeKeyword);

    // Or, two lengths or percentages
    if (!shape && !sizeKeyword) {
        GCRefPtr<CSSPrimitiveValue> horizontalSize, verticalSize;
        if ((horizontalSize = consumeLengthOrPercent(args, cssParserMode, ValueRangeAll))) {
            verticalSize = consumeLengthOrPercent(args, cssParserMode, ValueRangeAll);
            if (!verticalSize)
                return nullptr;
            consumeCommaIncludingWhitespace(args);
            result->setEndHorizontalSize(horizontalSize);
            result->setEndVerticalSize(verticalSize);
        }
    } else {
        consumeCommaIncludingWhitespace(args);
    }
    if (!consumeGradientColorStops(args, cssParserMode, result.get()))
        return nullptr;

    return result;
}

static GCRefPtr<CSSValue> consumeRadialGradient(CSSParserTokenRange& args, CSSParserMode cssParserMode, CSSGradientRepeat repeating)
{
    GCRefPtr<CSSRadialGradientValue> result = CSSRadialGradientValue::create(repeating, CSSRadialGradient);

    GCRefPtr<CSSPrimitiveValue> shape, sizeKeyword, horizontalSize, verticalSize;

    // First part of grammar, the size/shape clause:
    // [ circle || <length> ] |
    // [ ellipse || [ <length> | <percentage> ]{2} ] |
    // [ [ circle | ellipse] || <size-keyword> ]
    for (int i = 0; i < 3; ++i) {
        if (args.peek().type() == IdentToken) {
            CSSValueID id = args.peek().id();
            if (id == CSSValueCircle || id == CSSValueEllipse) {
                if (shape)
                    return nullptr;
                shape = consumeIdent(args);
            } else if (id == CSSValueClosestSide || id == CSSValueClosestCorner || id == CSSValueFarthestSide || id == CSSValueFarthestCorner) {
                if (sizeKeyword)
                    return nullptr;
                sizeKeyword = consumeIdent(args);
            } else {
                break;
            }
        } else {
            GCRefPtr<CSSPrimitiveValue> center = consumeLengthOrPercent(args, cssParserMode, ValueRangeAll);
            if (!center)
                break;
            if (horizontalSize)
                return nullptr;
            horizontalSize = center;
            if ((center = consumeLengthOrPercent(args, cssParserMode, ValueRangeAll))) {
                verticalSize = center;
                ++i;
            }
        }
    }

    // You can specify size as a keyword or a length/percentage, not both.
    if (sizeKeyword && horizontalSize)
        return nullptr;
    // Circles must have 0 or 1 lengths.
    if (shape && shape->getValueID() == CSSValueCircle && verticalSize)
        return nullptr;
    // Ellipses must have 0 or 2 length/percentages.
    if (shape && shape->getValueID() == CSSValueEllipse && horizontalSize && !verticalSize)
        return nullptr;
    // If there's only one size, it must be a length.
    // TODO(timloh): Calcs with both lengths and percentages should be rejected.
    if (!verticalSize && horizontalSize && horizontalSize->isPercentage())
        return nullptr;

    result->setShape(shape);
    result->setSizingBehavior(sizeKeyword);
    result->setEndHorizontalSize(horizontalSize);
    result->setEndVerticalSize(verticalSize);

    GCRefPtr<CSSValue> centerX, centerY;
    if (args.peek().id() == CSSValueAt) {
        args.consumeIncludingWhitespace();
        consumePosition(args, cssParserMode, UnitlessQuirk::Forbid, centerX, centerY);
        if (!(centerX && centerY))
            return nullptr;
        result->setFirstX(centerX);
        result->setFirstY(centerY);
        // Right now, CSS radial gradients have the same start and end centers.
        result->setSecondX(centerX);
        result->setSecondY(centerY);
    }

    if ((shape || sizeKeyword || horizontalSize || centerX || centerY) && !consumeCommaIncludingWhitespace(args))
        return nullptr;
    if (!consumeGradientColorStops(args, cssParserMode, result.get()))
        return nullptr;
    return result;
}

static GCRefPtr<CSSValue> consumeLinearGradient(CSSParserTokenRange& args, CSSParserMode cssParserMode, CSSGradientRepeat repeating, CSSGradientType gradientType)
{
    GCRefPtr<CSSLinearGradientValue> result = CSSLinearGradientValue::create(repeating, gradientType);

    bool expectComma = true;
    GCRefPtr<CSSPrimitiveValue> angle = consumeAngle(args, cssParserMode);
    if (angle) {
        result->setAngle(angle);
    } else if (gradientType == CSSPrefixedLinearGradient || consumeIdent<CSSValueTo>(args)) {
        GCRefPtr<CSSPrimitiveValue> endX = consumeIdent<CSSValueLeft, CSSValueRight>(args);
        GCRefPtr<CSSPrimitiveValue> endY = consumeIdent<CSSValueBottom, CSSValueTop>(args);
        if (!endX && !endY) {
            if (gradientType == CSSLinearGradient)
                return nullptr;
            endY = cssValuePool().createIdentifierValue(CSSValueTop);
            expectComma = false;
        } else if (!endX) {
            endX = consumeIdent<CSSValueLeft, CSSValueRight>(args);
        }

        result->setFirstX(endX);
        result->setFirstY(endY);
    } else {
        expectComma = false;
    }

    if (expectComma && !consumeCommaIncludingWhitespace(args))
        return nullptr;
    if (!consumeGradientColorStops(args, cssParserMode, result.get()))
        return nullptr;
    return result;
}

static GCRefPtr<CSSValue> consumeImage(CSSParserTokenRange&, CSSParserContext);

static GCRefPtr<CSSValue> consumeCrossFade(CSSParserTokenRange& args, CSSParserContext context)
{
    GCRefPtr<CSSValue> fromImageValue = consumeImage(args, context);
    if (!fromImageValue || !consumeCommaIncludingWhitespace(args))
        return nullptr;
    GCRefPtr<CSSValue> toImageValue = consumeImage(args, context);
    if (!toImageValue || !consumeCommaIncludingWhitespace(args))
        return nullptr;

    GCRefPtr<CSSPrimitiveValue> percentage;
    const CSSParserToken& percentageArg = args.consumeIncludingWhitespace();
    if (percentageArg.type() == PercentageToken)
        percentage = cssValuePool().createValue(clampTo<double>(percentageArg.numericValue() / 100, 0, 1), CSSPrimitiveValue::UnitType::Number);
    else if (percentageArg.type() == NumberToken)
        percentage = cssValuePool().createValue(clampTo<double>(percentageArg.numericValue(), 0, 1), CSSPrimitiveValue::UnitType::Number);

    if (!percentage)
        return nullptr;
    return CSSCrossfadeValue::create(fromImageValue, toImageValue, percentage);
}

static GCRefPtr<CSSValue> consumeGeneratedImage(CSSParserTokenRange& range, CSSParserContext context)
{
    CSSValueID id = range.peek().functionId();
    CSSParserTokenRange rangeCopy = range;
    CSSParserTokenRange args = consumeFunction(rangeCopy);
    GCRefPtr<CSSValue> result;
    if (id == CSSValueRadialGradient) {
        result = consumeRadialGradient(args, context.mode(), NonRepeating);
    } else if (id == CSSValueWebkitLinearGradient) {
#if 0 // BKTODO:
        // FIXME: This should send a deprecation message.
        if (context.useCounter())
            context.useCounter()->count(UseCounter::DeprecatedWebKitLinearGradient);
#endif
        result = consumeLinearGradient(args, context.mode(), NonRepeating, CSSPrefixedLinearGradient);
    } else if (id == CSSValueWebkitRepeatingLinearGradient) {
#if 0 // BKTODO:
        // FIXME: This should send a deprecation message.
        if (context.useCounter())
            context.useCounter()->count(UseCounter::DeprecatedWebKitRepeatingLinearGradient);
#endif
        result = consumeLinearGradient(args, context.mode(), Repeating, CSSPrefixedLinearGradient);
    } else if (id == CSSValueLinearGradient) {
        result = consumeLinearGradient(args, context.mode(), NonRepeating, CSSLinearGradient);
    } else if (id == CSSValueWebkitGradient) {
#if 0 // BKTODO:
        // FIXME: This should send a deprecation message.
        if (context.useCounter())
            context.useCounter()->count(UseCounter::DeprecatedWebKitGradient);
#endif
        result = consumeDeprecatedGradient(args, context.mode());
    } else if (id == CSSValueWebkitRadialGradient) {
#if 0 // BKTODO:
        // FIXME: This should send a deprecation message.
        if (context.useCounter())
            context.useCounter()->count(UseCounter::DeprecatedWebKitRadialGradient);
#endif
        result = consumeDeprecatedRadialGradient(args, context.mode(), NonRepeating);
    } else if (id == CSSValueWebkitRepeatingRadialGradient) {
#if 0 // BKTODO:
        if (context.useCounter())
            context.useCounter()->count(UseCounter::DeprecatedWebKitRepeatingRadialGradient);
#endif
        result = consumeDeprecatedRadialGradient(args, context.mode(), Repeating);
    } else if (id == CSSValueWebkitCrossFade) {
        result = consumeCrossFade(args, context);
    }
    if (!result || !args.atEnd())
        return nullptr;
    range = rangeCopy;
    return result;
}

static GCRefPtr<CSSValue> consumeImage(CSSParserTokenRange& range, CSSParserContext context)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);

    AtomicString uri(consumeUrl(range));
    if (!uri.isNull())
        return CSSPropertyParser::createCSSImageValueWithReferrer(uri, context);
    if (range.peek().type() == FunctionToken) {
        CSSValueID id = range.peek().functionId();
        if (id == CSSValueWebkitImageSet)
            return consumeImageSet(range, context);
        if (CSSPropertyParser::isGeneratedImage(id))
            return consumeGeneratedImage(range, context);
    }
    return nullptr;
}

static GCRefPtr<CSSValue> consumeAttr(CSSParserTokenRange args, CSSParserContext context)
{
    if (args.peek().type() != IdentToken)
        return nullptr;

    String attrName = args.consumeIncludingWhitespace().value();
    // CSS allows identifiers with "-" at the start, like "-webkit-mask-image".
    // But HTML attribute names can't have those characters, and we should not
    // even parse them inside attr().
    // TODO(timloh): We should allow any <ident-token> here.
    if (attrName[0] == '-' || !args.atEnd())
        return nullptr;

    if (context.isHTMLDocument())
        attrName = attrName.lower();

    GCRefPtr<CSSFunctionValue> attrValue = CSSFunctionValue::create(CSSValueAttr);
    attrValue->append(CSSCustomIdentValue::create(attrName));
    return attrValue;
}

static GCRefPtr<CSSValue> consumeCounterContent(CSSParserTokenRange args, bool counters)
{
    GCRefPtr<CSSCustomIdentValue> identifier = consumeCustomIdent(args);
    if (!identifier)
        return nullptr;

    // TODO(timloh): Make this a CSSStringValue.
    GCRefPtr<CSSCustomIdentValue> separator;
    if (!counters) {
        separator = CSSCustomIdentValue::create(String());
    } else {
        if (!consumeCommaIncludingWhitespace(args))
            return nullptr;
        if (args.peek().type() != StringToken)
            return nullptr;
        separator = CSSCustomIdentValue::create(args.consumeIncludingWhitespace().value());
    }

    GCRefPtr<CSSPrimitiveValue> listStyle;
    if (consumeCommaIncludingWhitespace(args)) {
        CSSValueID id = args.peek().id();
        if ((id != CSSValueNone && (id < CSSValueDisc || id > CSSValueKatakanaIroha)))
            return nullptr;
        listStyle = consumeIdent(args);
    } else {
        listStyle = cssValuePool().createIdentifierValue(CSSValueDecimal);
    }

    if (!args.atEnd())
        return nullptr;
    return CSSCounterValue::create(identifier, listStyle, separator);
}

static GCRefPtr<CSSValueList> consumeContent(CSSParserTokenRange& range, CSSParserContext context)
{
    GCRefPtr<CSSValueList> values = CSSValueList::createSpaceSeparated();

    do {
        GCRefPtr<CSSValue> parsedValue = consumeImage(range, context);
        if (!parsedValue)
            parsedValue = consumeIdent<CSSValueOpenQuote, CSSValueCloseQuote, CSSValueNoOpenQuote, CSSValueNoCloseQuote, CSSValueNormal>(range);
        if (!parsedValue)
            parsedValue = consumeString(range);
        if (!parsedValue) {
            if (range.peek().functionId() == CSSValueAttr)
                parsedValue = consumeAttr(consumeFunction(range), context);
            else if (range.peek().functionId() == CSSValueCounter)
                parsedValue = consumeCounterContent(consumeFunction(range), false);
            else if (range.peek().functionId() == CSSValueCounters)
                parsedValue = consumeCounterContent(consumeFunction(range), true);
            if (!parsedValue)
                return nullptr;
        }
        values->append(parsedValue);
    } while (!range.atEnd());

    return values;
}

static GCRefPtr<CSSPrimitiveValue> consumePerspective(CSSParserTokenRange& range, CSSParserMode cssParserMode, CSSPropertyID unresolvedProperty)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);
    GCRefPtr<CSSPrimitiveValue> parsedValue = consumeLength(range, cssParserMode, ValueRangeAll);
    if (!parsedValue && (unresolvedProperty == CSSPropertyAliasWebkitPerspective)) {
        double perspective;
        if (!consumeNumberRaw(range, perspective))
            return nullptr;
        parsedValue = cssValuePool().createValue(perspective, CSSPrimitiveValue::UnitType::Pixels);
    }
    if (parsedValue && (parsedValue->isCalculated() || parsedValue->getDoubleValue() > 0))
        return parsedValue;
    return nullptr;
}

static GCRefPtr<CSSValueList> consumePositionList(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    GCRefPtr<CSSValueList> positions = CSSValueList::createCommaSeparated();
    do {
        GCRefPtr<CSSValue> position = consumePosition(range, cssParserMode, UnitlessQuirk::Forbid);
        if (!position)
            return nullptr;
        positions->append(position);
    } while (consumeCommaIncludingWhitespace(range));
    return positions;
}

static GCRefPtr<CSSValue> consumeScrollSnapCoordinate(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);
    return consumePositionList(range, cssParserMode);
}

static GCRefPtr<CSSValue> consumeScrollSnapPoints(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    if (range.peek().id() == CSSValueNone)
        return consumeIdent(range);
    if (range.peek().functionId() == CSSValueRepeat) {
        CSSParserTokenRange args = consumeFunction(range);
        GCRefPtr<CSSPrimitiveValue> parsedValue = consumeLengthOrPercent(args, cssParserMode, ValueRangeNonNegative);
        if (args.atEnd() && parsedValue && (parsedValue->isCalculated() || parsedValue->getDoubleValue() > 0)) {
            GCRefPtr<CSSFunctionValue> result = CSSFunctionValue::create(CSSValueRepeat);
            result->append(parsedValue);
            return result;
        }
    }
    return nullptr;
}

static GCRefPtr<CSSValue> consumeBorderRadiusCorner(CSSParserTokenRange& range, CSSParserMode cssParserMode)
{
    GCRefPtr<CSSValue> parsedValue1 = consumeLengthOrPercent(range, cssParserMode, ValueRangeNonNegative);
    if (!parsedValue1)
        return nullptr;
    GCRefPtr<CSSValue> parsedValue2 = consumeLengthOrPercent(range, cssParserMode, ValueRangeNonNegative);
    if (!parsedValue2)
        parsedValue2 = parsedValue1;
    return CSSValuePair::create(parsedValue1, parsedValue2, CSSValuePair::DropIdenticalValues);
}

GCRefPtr<CSSValue> CSSPropertyParser::parseSingleValue(CSSPropertyID unresolvedProperty)
{
    CSSPropertyID property = resolveCSSPropertyID(unresolvedProperty);
    switch (property) {
    case CSSPropertyWillChange:
        return consumeWillChange(m_range);
    case CSSPropertyPage:
        return consumePage(m_range);
    case CSSPropertyQuotes:
        return consumeQuotes(m_range);
    case CSSPropertyWebkitHighlight:
        return consumeWebkitHighlight(m_range);
    case CSSPropertyFontVariantLigatures:
        return consumeFontVariantLigatures(m_range);
    case CSSPropertyFontFeatureSettings:
        return consumeFontFeatureSettings(m_range);
    case CSSPropertyFontVariant:
        return consumeFontVariant(m_range);
    case CSSPropertyFontFamily:
        return consumeFontFamily(m_range);
    case CSSPropertyFontWeight:
        return consumeFontWeight(m_range);
    case CSSPropertyLetterSpacing:
    case CSSPropertyWordSpacing:
        return consumeSpacing(m_range, m_context.mode());
    case CSSPropertyTabSize:
        return consumeTabSize(m_range, m_context.mode());
    case CSSPropertyFontSize:
        return consumeFontSize(m_range, m_context.mode(), UnitlessQuirk::Allow);
    case CSSPropertyLineHeight:
        return consumeLineHeight(m_range, m_context.mode());
    case CSSPropertyRotate:
        return consumeRotation(m_range, m_context.mode());
    case CSSPropertyScale:
        return consumeScale(m_range, m_context.mode());
    case CSSPropertyTranslate:
        return consumeTranslate(m_range, m_context.mode());
    case CSSPropertyWebkitBorderHorizontalSpacing:
    case CSSPropertyWebkitBorderVerticalSpacing:
        return consumeLength(m_range, m_context.mode(), ValueRangeNonNegative);
    case CSSPropertyCounterIncrement:
    case CSSPropertyCounterReset:
        return consumeCounter(m_range, m_context.mode(), property == CSSPropertyCounterIncrement ? 1 : 0);
    case CSSPropertySize:
        return consumeSize(m_range, m_context.mode());
    case CSSPropertyTextIndent:
        return consumeTextIndent(m_range, m_context.mode());
    case CSSPropertyMaxWidth:
    case CSSPropertyMaxHeight:
        return consumeMaxWidthOrHeight(m_range, m_context, UnitlessQuirk::Allow);
    case CSSPropertyWebkitMaxLogicalWidth:
    case CSSPropertyWebkitMaxLogicalHeight:
        return consumeMaxWidthOrHeight(m_range, m_context);
    case CSSPropertyMinWidth:
    case CSSPropertyMinHeight:
    case CSSPropertyWidth:
    case CSSPropertyHeight:
        return consumeWidthOrHeight(m_range, m_context, UnitlessQuirk::Allow);
    case CSSPropertyWebkitMinLogicalWidth:
    case CSSPropertyWebkitMinLogicalHeight:
    case CSSPropertyWebkitLogicalWidth:
    case CSSPropertyWebkitLogicalHeight:
        return consumeWidthOrHeight(m_range, m_context);
    case CSSPropertyMarginTop:
    case CSSPropertyMarginRight:
    case CSSPropertyMarginBottom:
    case CSSPropertyMarginLeft:
        return consumeMarginWidth(m_range, m_context.mode(), UnitlessQuirk::Allow);
    case CSSPropertyWebkitMarginStart:
    case CSSPropertyWebkitMarginEnd:
    case CSSPropertyWebkitMarginBefore:
    case CSSPropertyWebkitMarginAfter:
        return consumeMarginWidth(m_range, m_context.mode(), UnitlessQuirk::Forbid);
    case CSSPropertyPaddingTop:
    case CSSPropertyPaddingRight:
    case CSSPropertyPaddingBottom:
    case CSSPropertyPaddingLeft:
        return consumeLengthOrPercent(m_range, m_context.mode(), ValueRangeNonNegative, UnitlessQuirk::Allow);
    case CSSPropertyWebkitPaddingStart:
    case CSSPropertyWebkitPaddingEnd:
    case CSSPropertyWebkitPaddingBefore:
    case CSSPropertyWebkitPaddingAfter:
        return consumeLengthOrPercent(m_range, m_context.mode(), ValueRangeNonNegative, UnitlessQuirk::Forbid);
    case CSSPropertyClip:
        return consumeClip(m_range, m_context.mode());
    case CSSPropertyTouchAction:
        return consumeTouchAction(m_range);
    case CSSPropertyScrollSnapDestination:
    case CSSPropertyObjectPosition:
    case CSSPropertyPerspectiveOrigin:
        return consumePosition(m_range, m_context.mode(), UnitlessQuirk::Forbid);
    case CSSPropertyWebkitLineClamp:
        return consumeLineClamp(m_range);
    case CSSPropertyWebkitFontSizeDelta:
        return consumeLength(m_range, m_context.mode(), ValueRangeAll, UnitlessQuirk::Allow);
    case CSSPropertyWebkitHyphenateCharacter:
    case CSSPropertyWebkitLocale:
        return consumeLocale(m_range);
    case CSSPropertyWebkitColumnWidth:
        return consumeColumnWidth(m_range);
    case CSSPropertyWebkitColumnCount:
        return consumeColumnCount(m_range);
    case CSSPropertyWebkitColumnGap:
        return consumeColumnGap(m_range, m_context.mode());
    case CSSPropertyWebkitColumnSpan:
        return consumeColumnSpan(m_range, m_context.mode());
    case CSSPropertyZoom:
        return consumeZoom(m_range, m_context);
    case CSSPropertyAnimationDelay:
    case CSSPropertyTransitionDelay:
    case CSSPropertyAnimationDirection:
    case CSSPropertyAnimationDuration:
    case CSSPropertyTransitionDuration:
    case CSSPropertyAnimationFillMode:
    case CSSPropertyAnimationIterationCount:
    case CSSPropertyAnimationName:
    case CSSPropertyAnimationPlayState:
    case CSSPropertyTransitionProperty:
    case CSSPropertyAnimationTimingFunction:
    case CSSPropertyTransitionTimingFunction:
        return consumeAnimationPropertyList(property, m_range, m_context, unresolvedProperty == CSSPropertyAliasWebkitAnimationName);
    case CSSPropertyGridColumnGap:
    case CSSPropertyGridRowGap:
        return consumeLength(m_range, m_context.mode(), ValueRangeNonNegative);
    case CSSPropertyShapeMargin:
        return consumeLengthOrPercent(m_range, m_context.mode(), ValueRangeNonNegative);
    case CSSPropertyShapeImageThreshold:
        return consumeNumber(m_range, ValueRangeAll);
    case CSSPropertyWebkitBoxOrdinalGroup:
        return consumePositiveInteger(m_range);
    case CSSPropertyOrphans:
    case CSSPropertyWidows:
        return consumeWidowsOrOrphans(m_range);
    case CSSPropertyTextDecorationColor:
        ASSERT(RuntimeEnabledFeatures::css3TextDecorationsEnabled());
        return consumeColor(m_range, m_context.mode());
    case CSSPropertyWebkitTextStrokeWidth:
        return consumeTextStrokeWidth(m_range, m_context.mode());
    case CSSPropertyWebkitTextFillColor:
    case CSSPropertyWebkitTapHighlightColor:
    case CSSPropertyWebkitTextEmphasisColor:
    case CSSPropertyWebkitBorderStartColor:
    case CSSPropertyWebkitBorderEndColor:
    case CSSPropertyWebkitBorderBeforeColor:
    case CSSPropertyWebkitBorderAfterColor:
    case CSSPropertyWebkitTextStrokeColor:
#if 0 // BKTODO:
    case CSSPropertyStopColor:
    case CSSPropertyFloodColor:
    case CSSPropertyLightingColor:
#endif
    case CSSPropertyWebkitColumnRuleColor:
        return consumeColor(m_range, m_context.mode());
    case CSSPropertyColor:
        return consumeColor(m_range, m_context.mode(), inQuirksMode());
    case CSSPropertyWebkitBorderStartWidth:
    case CSSPropertyWebkitBorderEndWidth:
    case CSSPropertyWebkitBorderBeforeWidth:
    case CSSPropertyWebkitBorderAfterWidth:
        return consumeBorderWidth(m_range, m_context.mode());
    case CSSPropertyZIndex:
        return consumeZIndex(m_range);
    case CSSPropertyTextShadow: // CSS2 property, dropped in CSS2.1, back in CSS3, so treat as CSS3
    case CSSPropertyBoxShadow:
        return consumeShadow(m_range, m_context.mode(), property == CSSPropertyBoxShadow);
    case CSSPropertyWebkitFilter:
    case CSSPropertyBackdropFilter:
        return consumeFilter(m_range, m_context.mode());
    case CSSPropertyWebkitTextDecorationsInEffect:
    case CSSPropertyTextDecorationLine:
        return consumeTextDecorationLine(m_range);
#if 0 // BKTODO:
    case CSSPropertyD:
        return consumePath(m_range);
#endif
    case CSSPropertyMotionPath:
        return consumePathOrNone(m_range);
    case CSSPropertyMotionOffset:
        return consumeLengthOrPercent(m_range, m_context.mode(), ValueRangeAll);
    case CSSPropertyMotionRotation:
        return consumeMotionRotation(m_range, m_context.mode());
    case CSSPropertyWebkitTextEmphasisStyle:
        return consumeTextEmphasisStyle(m_range);
    case CSSPropertyOutlineColor:
        return consumeOutlineColor(m_range, m_context.mode());
    case CSSPropertyOutlineOffset:
        return consumeLength(m_range, m_context.mode(), ValueRangeAll);
    case CSSPropertyOutlineWidth:
        return consumeLineWidth(m_range, m_context.mode());
    case CSSPropertyTransform:
        return consumeTransform(m_range, m_context.mode(), unresolvedProperty == CSSPropertyAliasWebkitTransform);
    case CSSPropertyWebkitTransformOriginX:
    case CSSPropertyWebkitPerspectiveOriginX:
        return consumePositionX(m_range, m_context.mode());
    case CSSPropertyWebkitTransformOriginY:
    case CSSPropertyWebkitPerspectiveOriginY:
        return consumePositionY(m_range, m_context.mode());
    case CSSPropertyWebkitTransformOriginZ:
        return consumeLength(m_range, m_context.mode(), ValueRangeAll);
#if 0 // BKTODO:
    case CSSPropertyFill:
    case CSSPropertyStroke:
        return consumePaint(m_range, m_context.mode());
    case CSSPropertyPaintOrder:
        return consumePaintOrder(m_range);
    case CSSPropertyMarkerStart:
    case CSSPropertyMarkerMid:
    case CSSPropertyMarkerEnd:
    case CSSPropertyClipPath:
    case CSSPropertyFilter:
    case CSSPropertyMask:
        return consumeNoneOrURI(m_range);
#endif
    case CSSPropertyFlexBasis:
        return consumeFlexBasis(m_range, m_context.mode());
    case CSSPropertyFlexGrow:
    case CSSPropertyFlexShrink:
        return consumeNumber(m_range, ValueRangeNonNegative);
#if 0 // BKTODO:
    case CSSPropertyStrokeDasharray:
        return consumeStrokeDasharray(m_range);
#endif
    case CSSPropertyWebkitColumnRuleWidth:
        return consumeColumnRuleWidth(m_range, m_context.mode());
#if 0 // BKTODO:
    case CSSPropertyStrokeOpacity:
    case CSSPropertyFillOpacity:
    case CSSPropertyStopOpacity:
    case CSSPropertyFloodOpacity:
        return consumeNumber(m_range, ValueRangeAll);
    case CSSPropertyBaselineShift:
        return consumeBaselineShift(m_range);
    case CSSPropertyStrokeMiterlimit:
        return consumeNumber(m_range, ValueRangeNonNegative);
    case CSSPropertyStrokeWidth:
    case CSSPropertyStrokeDashoffset:
    case CSSPropertyCx:
    case CSSPropertyCy:
    case CSSPropertyX:
    case CSSPropertyY:
    case CSSPropertyR:
    case CSSPropertyRx:
    case CSSPropertyRy:
        return consumeLengthOrPercent(m_range, SVGAttributeMode, ValueRangeAll, UnitlessQuirk::Forbid);
#endif
    case CSSPropertyCursor:
        return consumeCursor(m_range, m_context, inQuirksMode());
    case CSSPropertyContain:
        return consumeContain(m_range);
    case CSSPropertyTransformOrigin:
        return consumeTransformOrigin(m_range, m_context.mode(), UnitlessQuirk::Forbid);
    case CSSPropertyContent:
        return consumeContent(m_range, m_context);
    case CSSPropertyListStyleImage:
        return consumeImage(m_range, m_context);
    case CSSPropertyPerspective:
        return consumePerspective(m_range, m_context.mode(), unresolvedProperty);
    case CSSPropertyScrollSnapCoordinate:
        return consumeScrollSnapCoordinate(m_range, m_context.mode());
    case CSSPropertyScrollSnapPointsX:
    case CSSPropertyScrollSnapPointsY:
        return consumeScrollSnapPoints(m_range, m_context.mode());
    case CSSPropertyBorderTopRightRadius:
    case CSSPropertyBorderTopLeftRadius:
    case CSSPropertyBorderBottomLeftRadius:
    case CSSPropertyBorderBottomRightRadius:
        return consumeBorderRadiusCorner(m_range, m_context.mode());
    default:
        return nullptr;
    }
}

static GCRefPtr<CSSValueList> consumeFontFaceUnicodeRange(CSSParserTokenRange& range)
{
    GCRefPtr<CSSValueList> values = CSSValueList::createCommaSeparated();

    do {
        const CSSParserToken& token = range.consumeIncludingWhitespace();
        if (token.type() != UnicodeRangeToken)
            return nullptr;

        UChar32 start = token.unicodeRangeStart();
        UChar32 end = token.unicodeRangeEnd();
        if (start > end)
            return nullptr;
        values->append(CSSUnicodeRangeValue::create(start, end));
    } while (consumeCommaIncludingWhitespace(range));

    return values;
}

static GCRefPtr<CSSValue> consumeFontFaceSrcURI(CSSParserTokenRange& range, const CSSParserContext& context)
{
    String url = consumeUrl(range);
    if (url.isNull())
        return nullptr;
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    RefPtrWillBeRawPtr<CSSFontFaceSrcValue> uriValue(CSSFontFaceSrcValue::create(context.completeURL(url), context.shouldCheckContentSecurityPolicy()));
    uriValue->setReferrer(context.referrer());

    if (range.peek().functionId() != CSSValueFormat)
        return uriValue.release();

    // FIXME: https://drafts.csswg.org/css-fonts says that format() contains a comma-separated list of strings,
    // but CSSFontFaceSrcValue stores only one format. Allowing one format for now.
    // FIXME: IdentToken should not be supported here.
    CSSParserTokenRange args = consumeFunction(range);
    const CSSParserToken& arg = args.consumeIncludingWhitespace();
    if ((arg.type() != StringToken && arg.type() != IdentToken) || !args.atEnd())
        return nullptr;
    uriValue->setFormat(arg.value());
    return uriValue.release();
#endif
}

static GCRefPtr<CSSValue> consumeFontFaceSrcLocal(CSSParserTokenRange& range, const CSSParserContext& context)
{
    CSSParserTokenRange args = consumeFunction(range);
    ASSERT(false); // BKTODO:
#if 0
    ContentSecurityPolicyDisposition shouldCheckContentSecurityPolicy = context.shouldCheckContentSecurityPolicy();
    if (args.peek().type() == StringToken) {
        const CSSParserToken& arg = args.consumeIncludingWhitespace();
        if (!args.atEnd())
            return nullptr;
        return CSSFontFaceSrcValue::createLocal(arg.value(), shouldCheckContentSecurityPolicy);
    }
    if (args.peek().type() == IdentToken) {
        String familyName = concatenateFamilyName(args);
        if (!args.atEnd())
            return nullptr;
        return CSSFontFaceSrcValue::createLocal(familyName, shouldCheckContentSecurityPolicy);
    }
#endif
    return nullptr;
}

static GCRefPtr<CSSValueList> consumeFontFaceSrc(CSSParserTokenRange& range, const CSSParserContext& context)
{
    GCRefPtr<CSSValueList> values(CSSValueList::createCommaSeparated());

    do {
        const CSSParserToken& token = range.peek();
        GCRefPtr<CSSValue> parsedValue;
        if (token.functionId() == CSSValueLocal)
            parsedValue = consumeFontFaceSrcLocal(range, context);
        else
            parsedValue = consumeFontFaceSrcURI(range, context);
        if (!parsedValue)
            return nullptr;
        values->append(parsedValue);
    } while (consumeCommaIncludingWhitespace(range));
    return values;
}

bool CSSPropertyParser::parseFontFaceDescriptor(CSSPropertyID propId)
{
    GCRefPtr<CSSValue> parsedValue;
    switch (propId) {
    case CSSPropertyFontFamily:
        if (consumeGenericFamily(m_range))
            return false;
        parsedValue = consumeFamilyName(m_range);
        break;
    case CSSPropertySrc: // This is a list of urls or local references.
        parsedValue = consumeFontFaceSrc(m_range, m_context);
        break;
    case CSSPropertyUnicodeRange:
        parsedValue = consumeFontFaceUnicodeRange(m_range);
        break;
    case CSSPropertyFontDisplay:
    case CSSPropertyFontStretch:
    case CSSPropertyFontStyle: {
        CSSValueID id = m_range.consumeIncludingWhitespace().id();
        if (!CSSParserFastPaths::isValidKeywordPropertyAndValue(propId, id))
            return false;
        parsedValue = cssValuePool().createIdentifierValue(id);
        break;
    }
    case CSSPropertyFontVariant:
        parsedValue = consumeFontVariantList(m_range);
        break;
    case CSSPropertyFontWeight:
        parsedValue = consumeFontWeight(m_range);
        break;
    case CSSPropertyFontFeatureSettings:
        parsedValue = consumeFontFeatureSettings(m_range);
        break;
    default:
        break;
    }

    if (!parsedValue || !m_range.atEnd())
        return false;

    addProperty(propId, parsedValue.release(), false);
    return true;
}

bool CSSPropertyParser::consumeSystemFont(bool important)
{
    CSSValueID systemFontID = m_range.consumeIncludingWhitespace().id();
    ASSERT(systemFontID >= CSSValueCaption && systemFontID <= CSSValueStatusBar);
    if (!m_range.atEnd())
        return false;

    FontStyle fontStyle = FontStyleNormal;
    FontWeight fontWeight = FontWeightNormal;
    float fontSize = 0;
    AtomicString fontFamily;
    LayoutTheme::theme().systemFont(systemFontID, fontStyle, fontWeight, fontSize, fontFamily);

    addProperty(CSSPropertyFontStyle, cssValuePool().createIdentifierValue(fontStyle == FontStyleItalic ? CSSValueItalic : CSSValueNormal), important);
    addProperty(CSSPropertyFontWeight, cssValuePool().createValue(fontWeight), important);
    addProperty(CSSPropertyFontSize, cssValuePool().createValue(fontSize, CSSPrimitiveValue::UnitType::Pixels), important);
    GCRefPtr<CSSValueList> fontFamilyList = CSSValueList::createCommaSeparated();
    fontFamilyList->append(cssValuePool().createFontFamilyValue(fontFamily));
    addProperty(CSSPropertyFontFamily, fontFamilyList.release(), important);

    addProperty(CSSPropertyFontStretch, cssValuePool().createIdentifierValue(CSSValueNormal), important);
    addProperty(CSSPropertyFontVariant, cssValuePool().createIdentifierValue(CSSValueNormal), important);
    addProperty(CSSPropertyLineHeight, cssValuePool().createIdentifierValue(CSSValueNormal), important);
    return true;
}

bool CSSPropertyParser::consumeFont(bool important)
{
    // Let's check if there is an inherit or initial somewhere in the shorthand.
    CSSParserTokenRange range = m_range;
    while (!range.atEnd()) {
        CSSValueID id = range.consumeIncludingWhitespace().id();
        if (id == CSSValueInherit || id == CSSValueInitial)
            return false;
    }
    // Optional font-style, font-variant, font-stretch and font-weight.
    GCRefPtr<CSSPrimitiveValue> fontStyle, fontVariant, fontWeight, fontStretch;
    while (!m_range.atEnd()) {
        CSSValueID id = m_range.peek().id();
        if (!fontStyle && CSSParserFastPaths::isValidKeywordPropertyAndValue(CSSPropertyFontStyle, id)) {
            fontStyle = consumeIdent(m_range);
            continue;
        }
        if (!fontVariant) {
            // Font variant in the shorthand is particular, it only accepts normal or small-caps.
            fontVariant = consumeFontVariant(m_range);
            if (fontVariant)
                continue;
        }
        if (!fontWeight) {
            fontWeight = consumeFontWeight(m_range);
            if (fontWeight)
                continue;
        }
        if (!fontStretch && CSSParserFastPaths::isValidKeywordPropertyAndValue(CSSPropertyFontStretch, id))
            fontStretch = consumeIdent(m_range);
        else
            break;
    }

    if (m_range.atEnd())
        return false;

    addProperty(CSSPropertyFontStyle, fontStyle ? fontStyle : cssValuePool().createIdentifierValue(CSSValueNormal), important);
    addProperty(CSSPropertyFontVariant, fontVariant ? fontVariant : cssValuePool().createIdentifierValue(CSSValueNormal), important);
    addProperty(CSSPropertyFontWeight, fontWeight ? fontWeight : cssValuePool().createIdentifierValue(CSSValueNormal), important);
    addProperty(CSSPropertyFontStretch, fontStretch ? fontStretch : cssValuePool().createIdentifierValue(CSSValueNormal), important);

    // Now a font size _must_ come.
    GCRefPtr<CSSValue> fontSize = consumeFontSize(m_range, m_context.mode());
    if (!fontSize || m_range.atEnd())
        return false;

    addProperty(CSSPropertyFontSize, fontSize, important);

    if (m_range.peek().type() == DelimiterToken && m_range.peek().delimiter() == '/') {
        m_range.consumeIncludingWhitespace();
        GCRefPtr<CSSPrimitiveValue> lineHeight = consumeLineHeight(m_range, m_context.mode());
        if (!lineHeight)
            return false;
        addProperty(CSSPropertyLineHeight, lineHeight, important);
    } else {
        addProperty(CSSPropertyLineHeight, cssValuePool().createIdentifierValue(CSSValueNormal), important);
    }

    // Font family must come now.
    GCRefPtr<CSSValue> parsedFamilyValue = consumeFontFamily(m_range);
    if (!parsedFamilyValue)
        return false;

    addProperty(CSSPropertyFontFamily, parsedFamilyValue, important);

    // FIXME: http://www.w3.org/TR/2011/WD-css3-fonts-20110324/#font-prop requires that
    // "font-stretch", "font-size-adjust", and "font-kerning" be reset to their initial values
    // but we don't seem to support them at the moment. They should also be added here once implemented.
    return m_range.atEnd();
}

bool CSSPropertyParser::consumeBorderSpacing(bool important)
{
    GCRefPtr<CSSValue> horizontalSpacing = consumeLength(m_range, m_context.mode(), ValueRangeNonNegative, UnitlessQuirk::Allow);
    if (!horizontalSpacing)
        return false;
    GCRefPtr<CSSValue> verticalSpacing = horizontalSpacing;
    if (!m_range.atEnd())
        verticalSpacing = consumeLength(m_range, m_context.mode(), ValueRangeNonNegative, UnitlessQuirk::Allow);
    if (!verticalSpacing || !m_range.atEnd())
        return false;
    addProperty(CSSPropertyWebkitBorderHorizontalSpacing, horizontalSpacing, important);
    addProperty(CSSPropertyWebkitBorderVerticalSpacing, verticalSpacing, important);
    return true;
}

static GCRefPtr<CSSValue> consumeSingleViewportDescriptor(CSSParserTokenRange& range, CSSPropertyID propId, CSSParserMode cssParserMode)
{
    CSSValueID id = range.peek().id();
    switch (propId) {
    case CSSPropertyMinWidth:
    case CSSPropertyMaxWidth:
    case CSSPropertyMinHeight:
    case CSSPropertyMaxHeight:
        if (id == CSSValueAuto || id == CSSValueInternalExtendToZoom)
            return consumeIdent(range);
        return consumeLengthOrPercent(range, cssParserMode, ValueRangeNonNegative);
    case CSSPropertyMinZoom:
    case CSSPropertyMaxZoom:
    case CSSPropertyZoom: {
        if (id == CSSValueAuto)
            return consumeIdent(range);
        GCRefPtr<CSSValue> parsedValue = consumeNumber(range, ValueRangeNonNegative);
        if (parsedValue)
            return parsedValue;
        return consumePercent(range, ValueRangeNonNegative);
    }
    case CSSPropertyUserZoom:
        return consumeIdent<CSSValueZoom, CSSValueFixed>(range);
    case CSSPropertyOrientation:
        return consumeIdent<CSSValueAuto, CSSValuePortrait, CSSValueLandscape>(range);
    default:
        ASSERT_NOT_REACHED();
        break;
    }

    ASSERT_NOT_REACHED();
    return nullptr;
}

bool CSSPropertyParser::parseViewportDescriptor(CSSPropertyID propId, bool important)
{
    ASSERT(RuntimeEnabledFeatures::cssViewportEnabled() || isUASheetBehavior(m_context.mode()));

    switch (propId) {
    case CSSPropertyWidth: {
        GCRefPtr<CSSValue> minWidth = consumeSingleViewportDescriptor(m_range, CSSPropertyMinWidth, m_context.mode());
        if (!minWidth)
            return false;
        GCRefPtr<CSSValue> maxWidth = minWidth;
        if (!m_range.atEnd())
            maxWidth = consumeSingleViewportDescriptor(m_range, CSSPropertyMaxWidth, m_context.mode());
        if (!maxWidth || !m_range.atEnd())
            return false;
        addProperty(CSSPropertyMinWidth, minWidth, important);
        addProperty(CSSPropertyMaxWidth, maxWidth, important);
        return true;
    }
    case CSSPropertyHeight: {
        GCRefPtr<CSSValue> minHeight = consumeSingleViewportDescriptor(m_range, CSSPropertyMinHeight, m_context.mode());
        if (!minHeight)
            return false;
        GCRefPtr<CSSValue> maxHeight = minHeight;
        if (!m_range.atEnd())
            maxHeight = consumeSingleViewportDescriptor(m_range, CSSPropertyMaxHeight, m_context.mode());
        if (!maxHeight || !m_range.atEnd())
            return false;
        addProperty(CSSPropertyMinHeight, minHeight, important);
        addProperty(CSSPropertyMaxHeight, maxHeight, important);
        return true;
    }
    case CSSPropertyMinWidth:
    case CSSPropertyMaxWidth:
    case CSSPropertyMinHeight:
    case CSSPropertyMaxHeight:
    case CSSPropertyMinZoom:
    case CSSPropertyMaxZoom:
    case CSSPropertyZoom:
    case CSSPropertyUserZoom:
    case CSSPropertyOrientation: {
        GCRefPtr<CSSValue> parsedValue = consumeSingleViewportDescriptor(m_range, propId, m_context.mode());
        if (!parsedValue || !m_range.atEnd())
            return false;
        addProperty(propId, parsedValue, important);
        return true;
    }
    default:
        return false;
    }
}

static bool consumeColumnWidthOrCount(CSSParserTokenRange& range, CSSParserMode cssParserMode, GCRefPtr<CSSValue> &columnWidth, GCRefPtr<CSSValue> &columnCount)
{
    if (range.peek().id() == CSSValueAuto) {
        consumeIdent(range);
        return true;
    }
    if (!columnWidth) {
        if ((columnWidth = consumeColumnWidth(range)))
            return true;
    }
    if (!columnCount)
        columnCount = consumeColumnCount(range);
    return columnCount;
}

bool CSSPropertyParser::consumeColumns(bool important)
{
    GCRefPtr<CSSValue> columnWidth, columnCount;
    if (!consumeColumnWidthOrCount(m_range, m_context.mode(), columnWidth, columnCount))
        return false;
    consumeColumnWidthOrCount(m_range, m_context.mode(), columnWidth, columnCount);
    if (!m_range.atEnd())
        return false;
    if (!columnWidth)
        columnWidth = cssValuePool().createIdentifierValue(CSSValueAuto);
    if (!columnCount)
        columnCount = cssValuePool().createIdentifierValue(CSSValueAuto);
    addProperty(CSSPropertyWebkitColumnWidth, columnWidth, important);
    addProperty(CSSPropertyWebkitColumnCount, columnCount, important);
    return true;
}

bool CSSPropertyParser::consumeShorthandGreedily(const StylePropertyShorthand& shorthand, bool important)
{
    ASSERT(shorthand.length() <= 6); // Existing shorthands have at most 6 longhands.
    GCRefPtr<CSSValue> longhands[6];
    const CSSPropertyID* shorthandProperties = shorthand.properties();
    do {
        bool foundLonghand = false;
        for (size_t i = 0; !foundLonghand && i < shorthand.length(); ++i) {
            if (longhands[i])
                continue;
            // TODO: parseSingleValue needs to handle fastpath keywords.
            if (CSSParserFastPaths::isKeywordPropertyID(shorthandProperties[i])) {
                if (CSSParserFastPaths::isValidKeywordPropertyAndValue(shorthandProperties[i], m_range.peek().id()))
                    longhands[i] = consumeIdent(m_range);
            } else {
                longhands[i] = parseSingleValue(shorthandProperties[i]);
            }
            if (longhands[i])
                foundLonghand = true;
        }
        if (!foundLonghand)
            return false;
    } while (!m_range.atEnd());

    ImplicitScope implicitScope(this);
    for (size_t i = 0; i < shorthand.length(); ++i) {
        if (longhands[i])
            addProperty(shorthandProperties[i], longhands[i].release(), important);
        else
            addProperty(shorthandProperties[i], cssValuePool().createImplicitInitialValue(), important);
    }
    return true;
}

bool CSSPropertyParser::consumeFlex(bool important)
{
    ShorthandScope scope(this, CSSPropertyFlex);
    static const double unsetValue = -1;
    double flexGrow = unsetValue;
    double flexShrink = unsetValue;
    GCRefPtr<CSSPrimitiveValue> flexBasis;

    if (m_range.peek().id() == CSSValueNone) {
        flexGrow = 0;
        flexShrink = 0;
        flexBasis = cssValuePool().createIdentifierValue(CSSValueAuto);
        m_range.consumeIncludingWhitespace();
    } else {
        unsigned index = 0;
        while (!m_range.atEnd() && index++ < 3) {
            double num;
            if (consumeNumberRaw(m_range, num)) {
                if (num < 0)
                    return false;
                if (flexGrow == unsetValue)
                    flexGrow = num;
                else if (flexShrink == unsetValue)
                    flexShrink = num;
                else if (!num) // flex only allows a basis of 0 (sans units) if flex-grow and flex-shrink values have already been set.
                    flexBasis = cssValuePool().createValue(0, CSSPrimitiveValue::UnitType::Pixels);
                else
                    return false;
            } else if (!flexBasis) {
                if (m_range.peek().id() == CSSValueAuto)
                    flexBasis = consumeIdent(m_range);
                if (!flexBasis)
                    flexBasis = consumeLengthOrPercent(m_range, m_context.mode(), ValueRangeNonNegative);
                if (index == 2 && !m_range.atEnd())
                    return false;
            }
        }
        if (index == 0)
            return false;
        if (flexGrow == unsetValue)
            flexGrow = 1;
        if (flexShrink == unsetValue)
            flexShrink = 1;
        if (!flexBasis)
            flexBasis = cssValuePool().createValue(0, CSSPrimitiveValue::UnitType::Percentage);
    }

    if (!m_range.atEnd())
        return false;
    addProperty(CSSPropertyFlexGrow, cssValuePool().createValue(clampTo<float>(flexGrow), CSSPrimitiveValue::UnitType::Number), important);
    addProperty(CSSPropertyFlexShrink, cssValuePool().createValue(clampTo<float>(flexShrink), CSSPrimitiveValue::UnitType::Number), important);
    addProperty(CSSPropertyFlexBasis, flexBasis, important);
    return true;
}

bool CSSPropertyParser::consume4Values(const StylePropertyShorthand& shorthand, bool important)
{
    ASSERT(shorthand.length() == 4);
    const CSSPropertyID* longhands = shorthand.properties();
    GCRefPtr<CSSValue> top = parseSingleValue(longhands[0]);
    if (!top)
        return false;

    GCRefPtr<CSSValue> right, bottom, left;
    if ((right = parseSingleValue(longhands[1]))) {
        if ((bottom = parseSingleValue(longhands[2])))
            left = parseSingleValue(longhands[3]);
    }

    if (!right)
        right = top;
    if (!bottom)
        bottom = top;
    if (!left)
        left = right;

    addProperty(longhands[0], top, important);
    addProperty(longhands[1], right, important);
    addProperty(longhands[2], bottom, important);
    addProperty(longhands[3], left, important);

    return m_range.atEnd();
}

static bool consumeRadii(GCRefPtr<CSSPrimitiveValue> horizontalRadii[4], GCRefPtr<CSSPrimitiveValue> verticalRadii[4], CSSParserTokenRange& range, CSSParserMode cssParserMode, bool useLegacyParsing)
{
    unsigned i = 0;
    for (; i < 4 && !range.atEnd() && range.peek().type() != DelimiterToken; ++i) {
        horizontalRadii[i] = consumeLengthOrPercent(range, cssParserMode, ValueRangeNonNegative);
        if (!horizontalRadii[i])
            return false;
    }
    if (!horizontalRadii[0])
        return false;
    if (range.atEnd()) {
        // Legacy syntax: -webkit-border-radius: l1 l2; is equivalent to border-radius: l1 / l2;
        if (useLegacyParsing && i == 2) {
            verticalRadii[0] = horizontalRadii[1];
            horizontalRadii[1] = nullptr;
        } else {
            completeBorderRadii(horizontalRadii);
            for (unsigned i = 0; i < 4; ++i)
                verticalRadii[i] = horizontalRadii[i];
            return true;
        }
    } else {
        if (range.peek().type() != DelimiterToken || range.peek().delimiter() != '/')
            return false;
        range.consumeIncludingWhitespace();
        for (i = 0; i < 4 && !range.atEnd(); ++i) {
            verticalRadii[i] = consumeLengthOrPercent(range, cssParserMode, ValueRangeNonNegative);
            if (!verticalRadii[i])
                return false;
        }
        if (!verticalRadii[0] || !range.atEnd())
            return false;
    }
    completeBorderRadii(horizontalRadii);
    completeBorderRadii(verticalRadii);
    return true;
}

bool CSSPropertyParser::parseShorthand(CSSPropertyID unresolvedProperty, bool important)
{
    CSSPropertyID property = resolveCSSPropertyID(unresolvedProperty);

    CSSPropertyID oldShorthand = m_currentShorthand;
    // TODO(rob.buis): Remove this when the legacy property parser is gone
    m_currentShorthand = property;
    switch (property) {
    case CSSPropertyWebkitMarginCollapse: {
        CSSValueID id = m_range.consumeIncludingWhitespace().id();
        if (!CSSParserFastPaths::isValidKeywordPropertyAndValue(CSSPropertyWebkitMarginBeforeCollapse, id))
            return false;
        GCRefPtr<CSSValue> beforeCollapse = cssValuePool().createIdentifierValue(id);
        addProperty(CSSPropertyWebkitMarginBeforeCollapse, beforeCollapse, important);
        if (m_range.atEnd()) {
            addProperty(CSSPropertyWebkitMarginAfterCollapse, beforeCollapse, important);
            return true;
        }
        id = m_range.consumeIncludingWhitespace().id();
        if (!CSSParserFastPaths::isValidKeywordPropertyAndValue(CSSPropertyWebkitMarginAfterCollapse, id))
            return false;
        addProperty(CSSPropertyWebkitMarginAfterCollapse, cssValuePool().createIdentifierValue(id), important);
        return true;
    }
    case CSSPropertyOverflow: {
        CSSValueID id = m_range.consumeIncludingWhitespace().id();
        if (!CSSParserFastPaths::isValidKeywordPropertyAndValue(CSSPropertyOverflowY, id))
            return false;
        if (!m_range.atEnd())
            return false;
        GCRefPtr<CSSValue> overflowYValue = cssValuePool().createIdentifierValue(id);

        GCRefPtr<CSSValue> overflowXValue;

        // FIXME: -webkit-paged-x or -webkit-paged-y only apply to overflow-y. If this value has been
        // set using the shorthand, then for now overflow-x will default to auto, but once we implement
        // pagination controls, it should default to hidden. If the overflow-y value is anything but
        // paged-x or paged-y, then overflow-x and overflow-y should have the same value.
        if (id == CSSValueWebkitPagedX || id == CSSValueWebkitPagedY)
            overflowXValue = cssValuePool().createIdentifierValue(CSSValueAuto);
        else
            overflowXValue = overflowYValue;
        addProperty(CSSPropertyOverflowX, overflowXValue, important);
        addProperty(CSSPropertyOverflowY, overflowYValue, important);
        return true;
    }
    case CSSPropertyFont: {
        const CSSParserToken& token = m_range.peek();
        if (token.id() >= CSSValueCaption && token.id() <= CSSValueStatusBar)
            return consumeSystemFont(important);
        return consumeFont(important);
    }
    case CSSPropertyBorderSpacing:
        return consumeBorderSpacing(important);
    case CSSPropertyWebkitColumns: {
        // TODO(rwlbuis): investigate if this shorthand hack can be removed.
        m_currentShorthand = oldShorthand;
        return consumeColumns(important);
    }
    case CSSPropertyAnimation:
        return consumeAnimationShorthand(animationShorthandForParsing(), unresolvedProperty == CSSPropertyAliasWebkitAnimation, important);
    case CSSPropertyTransition:
        return consumeAnimationShorthand(transitionShorthandForParsing(), false, important);
    case CSSPropertyTextDecoration: {
        // Fall through 'text-decoration-line' parsing if CSS 3 Text Decoration
        // is disabled to match CSS 2.1 rules for parsing 'text-decoration'.
        if (RuntimeEnabledFeatures::css3TextDecorationsEnabled())
            return consumeShorthandGreedily(textDecorationShorthand(), important);
        // TODO(rwlbuis): investigate if this shorthand hack can be removed.
        m_currentShorthand = oldShorthand;
        GCRefPtr<CSSValue> textDecoration = consumeTextDecorationLine(m_range);
        if (!textDecoration || !m_range.atEnd())
            return false;
        addProperty(CSSPropertyTextDecoration, textDecoration.release(), important);
        return true;
    }
    case CSSPropertyMargin:
        return consume4Values(marginShorthand(), important);
    case CSSPropertyPadding:
        return consume4Values(paddingShorthand(), important);
    case CSSPropertyMotion:
        return consumeShorthandGreedily(motionShorthand(), important);
    case CSSPropertyWebkitTextEmphasis:
        return consumeShorthandGreedily(webkitTextEmphasisShorthand(), important);
    case CSSPropertyOutline:
        return consumeShorthandGreedily(outlineShorthand(), important);
    case CSSPropertyWebkitBorderStart:
        return consumeShorthandGreedily(webkitBorderStartShorthand(), important);
    case CSSPropertyWebkitBorderEnd:
        return consumeShorthandGreedily(webkitBorderEndShorthand(), important);
    case CSSPropertyWebkitBorderBefore:
        return consumeShorthandGreedily(webkitBorderBeforeShorthand(), important);
    case CSSPropertyWebkitBorderAfter:
        return consumeShorthandGreedily(webkitBorderAfterShorthand(), important);
    case CSSPropertyWebkitTextStroke:
        return consumeShorthandGreedily(webkitTextStrokeShorthand(), important);
#if 0 // BKTODO:
    case CSSPropertyMarker: {
        ImplicitScope implicitScope(this);
        RefPtrWillBeRawPtr<CSSValue> marker = parseSingleValue(CSSPropertyMarkerStart);
        if (!marker || !m_range.atEnd())
            return false;
        addProperty(CSSPropertyMarkerStart, marker, important);
        addProperty(CSSPropertyMarkerMid, marker, important);
        addProperty(CSSPropertyMarkerEnd, marker.release(), important);
        return true;
    }
#endif
    case CSSPropertyFlex:
        return consumeFlex(important);
    case CSSPropertyFlexFlow:
        return consumeShorthandGreedily(flexFlowShorthand(), important);
    case CSSPropertyWebkitColumnRule:
        return consumeShorthandGreedily(webkitColumnRuleShorthand(), important);
    case CSSPropertyListStyle:
        return consumeShorthandGreedily(listStyleShorthand(), important);
    case CSSPropertyBorderRadius: {
        GCRefPtr<CSSPrimitiveValue> horizontalRadii[4];
        GCRefPtr<CSSPrimitiveValue> verticalRadii[4];
        if (!consumeRadii(horizontalRadii, verticalRadii, m_range, m_context.mode(), unresolvedProperty == CSSPropertyAliasWebkitBorderRadius))
            return false;
        addProperty(CSSPropertyBorderTopLeftRadius, CSSValuePair::create(horizontalRadii[0], verticalRadii[0], CSSValuePair::DropIdenticalValues), important);
        addProperty(CSSPropertyBorderTopRightRadius, CSSValuePair::create(horizontalRadii[1], verticalRadii[1], CSSValuePair::DropIdenticalValues), important);
        addProperty(CSSPropertyBorderBottomRightRadius, CSSValuePair::create(horizontalRadii[2], verticalRadii[2], CSSValuePair::DropIdenticalValues), important);
        addProperty(CSSPropertyBorderBottomLeftRadius, CSSValuePair::create(horizontalRadii[3], verticalRadii[3], CSSValuePair::DropIdenticalValues), important);
        return true;
    }
    default:
        m_currentShorthand = oldShorthand;
        return false;
    }
}

} // namespace blink
