// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSPropertyParser.h
// Description: CSSPropertyParser Class
//      Author: Ziming Li
//     Created: 2021-07-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2009, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2009 - 2010  Torch Mobile (Beijing) Co. Ltd. All rights reserved.
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

#ifndef CSSPropertyParser_h
#define CSSPropertyParser_h

#include "core/css/CSSColorValue.h"
#include "core/css/CSSGradientValue.h"
#include "core/css/CSSGridTemplateAreasValue.h"
#include "core/css/CSSImageValue.h"
#include "core/css/CSSPropertySourceData.h"
#include "core/css/parser/CSSParserTokenRange.h"
#include "platform/Length.h"

namespace blink {

class BorderImageParseContext;
class CSSBorderImageSliceValue;
class CSSBasicShapeValue;
class CSSBasicShapeEllipseValue;
class CSSBasicShapeCircleValue;
class CSSBasicShapeInsetValue;
class CSSBasicShapePolygonValue;
class CSSCustomIdentValue;
class CSSFunctionValue;
class CSSGradientValue;
class CSSGridLineNamesValue;
struct CSSParserString;
struct CSSParserValue;
class CSSParserValueList;
class CSSPrimitiveValue;
class CSSProperty;
class CSSQuadValue;
class CSSShadowValue;
class CSSStringValue;
class CSSValue;
class CSSValueList;
class StylePropertyShorthand;

enum class UnitlessQuirk {
    Allow,
    Forbid
};

// Inputs: PropertyID, isImportant bool, CSSParserValueList.
// Outputs: Vector of CSSProperties

class CSSPropertyParser {
    STACK_ALLOCATED();
public:

    enum Units {
        FUnknown = 0x0000,
        FInteger = 0x0001,
        FNumber = 0x0002, // Real Numbers
        FPercent = 0x0004,
        FLength = 0x0008,
        FAngle = 0x0010,
        FTime = 0x0020,
        FFrequency = 0x0040,
        FPositiveInteger = 0x0080,
        FRelative = 0x0100,
        FResolution = 0x0200,
        FNonNeg = 0x0400,
        FUnitlessQuirk = 0x0800
    };

    static bool parseValue(CSSPropertyID, bool important,
        const CSSParserTokenRange&, const CSSParserContext&,
        std::vector<CSSProperty>&, StyleRule::Type);

    static bool isSystemColor(CSSValueID);
    static bool isColorKeyword(CSSValueID);
    static bool isValidNumericValue(double);

    // TODO(rwlbuis): move to CSSPropertyParser.cpp once CSSParserToken conversion is done.
    static GCRefPtr<CSSValue> createCSSImageValueWithReferrer(const AtomicString& rawValue, const CSSParserContext& context)
    {
        GCRefPtr<CSSValue> imageValue = CSSImageValue::create(rawValue, context.completeURL(rawValue));
        ASSERT(false); // BKTODO: toCSSImageValue(imageValue.get())->setReferrer(context.referrer());
        return imageValue;
    }

    // TODO(rwlbuis): move to CSSPropertyParser.cpp once CSSParserToken conversion is done.
    static bool isGeneratedImage(CSSValueID id)
    {
        return id == CSSValueLinearGradient || id == CSSValueRadialGradient
            || id == CSSValueRepeatingLinearGradient || id == CSSValueRepeatingRadialGradient
            || id == CSSValueWebkitLinearGradient || id == CSSValueWebkitRadialGradient
            || id == CSSValueWebkitRepeatingLinearGradient || id == CSSValueWebkitRepeatingRadialGradient
            || id == CSSValueWebkitGradient || id == CSSValueWebkitCrossFade;
    }

private:
    CSSPropertyParser(const CSSParserTokenRange&, const CSSParserContext&,
        std::vector<CSSProperty>&);

    // TODO(timloh): Rename once the CSSParserValue-based parseValue is removed
    bool parseValueStart(CSSPropertyID unresolvedProperty, bool important);
    bool consumeCSSWideKeyword(CSSPropertyID unresolvedProperty, bool important);
    GCRefPtr<CSSValue> parseSingleValue(CSSPropertyID);

    bool parseValue(CSSPropertyID, bool important);

    bool inShorthand() const { return m_inParseShorthand; }
    bool inQuirksMode() const { return isQuirksModeBehavior(m_context.mode()); }

    bool parseViewportDescriptor(CSSPropertyID propId, bool important);
    bool parseFontFaceDescriptor(CSSPropertyID);

    void addProperty(CSSPropertyID, CSSValue *, bool important, bool implicit = false);
    void addProperty(CSSPropertyID, const GCRefPtr<CSSValue> &, bool important, bool implicit = false);
    void addExpandedPropertyForValue(CSSPropertyID propId, const GCRefPtr<CSSValue> &, bool);

    GCRefPtr<CSSPrimitiveValue> parseValidPrimitive(CSSValueID ident, CSSParserValue*);

    bool parseShorthand(CSSPropertyID, const StylePropertyShorthand&, bool important);
    bool parseShorthand(CSSPropertyID, bool important);
    bool consumeShorthandGreedily(const StylePropertyShorthand&, bool important);
    bool consume4Values(const StylePropertyShorthand&, bool important);

    bool parse4Values(CSSPropertyID, const CSSPropertyID* properties, bool important);

    bool parseFillImage(CSSParserValueList*, GCRefPtr<CSSValue>&);

    enum FillPositionFlag { InvalidFillPosition = 0, AmbiguousFillPosition = 1, XFillPosition = 2, YFillPosition = 4 };
    enum FillPositionParsingMode { ResolveValuesAsPercent = 0, ResolveValuesAsKeyword = 1 };
    GCRefPtr<CSSPrimitiveValue> parseFillPositionComponent(CSSParserValueList*, unsigned& cumulativeFlags, FillPositionFlag& individualFlag, FillPositionParsingMode = ResolveValuesAsPercent, Units = FUnknown);
    GCRefPtr<CSSValue> parseFillPositionX(CSSParserValueList*);
    GCRefPtr<CSSValue> parseFillPositionY(CSSParserValueList*);
    void parse2ValuesFillPosition(CSSParserValueList*, GCRefPtr<CSSValue>&, GCRefPtr<CSSValue>&, Units = FUnknown);
    bool isPotentialPositionValue(CSSParserValue*);
    void parseFillPosition(CSSParserValueList*, GCRefPtr<CSSValue>&, GCRefPtr<CSSValue>&, Units = FUnknown);
    void parse3ValuesFillPosition(CSSParserValueList*, GCRefPtr<CSSValue>&, GCRefPtr<CSSValue>&, CSSPrimitiveValue*, CSSPrimitiveValue*);
    void parse4ValuesFillPosition(CSSParserValueList*, GCRefPtr<CSSValue>&, GCRefPtr<CSSValue>&, CSSPrimitiveValue*, CSSPrimitiveValue*);

    void parseFillRepeat(GCRefPtr<CSSValue>&, GCRefPtr<CSSValue>&);
    GCRefPtr<CSSValue> parseFillSize(CSSPropertyID);

    bool parseFillProperty(CSSPropertyID propId, CSSPropertyID& propId1, CSSPropertyID& propId2, GCRefPtr<CSSValue>&, GCRefPtr<CSSValue>&);
    bool parseFillShorthand(CSSPropertyID, const CSSPropertyID* properties, int numProperties, bool important);

    void addFillValue(GCRefPtr<CSSValue> &lval, const GCRefPtr<CSSValue> &rval);

    bool parseCubicBezierTimingFunctionValue(CSSParserValueList*& args, double& result);

    // Legacy parsing allows <string>s for animation-name
    bool consumeAnimationShorthand(const StylePropertyShorthand&, bool useLegacyParsing, bool important);

    bool consumeColumns(bool important);

    GCRefPtr<CSSValue> parseGridPosition();
    bool parseIntegerOrCustomIdentFromGridPosition(GCRefPtr<CSSPrimitiveValue>& numericValue, GCRefPtr<CSSCustomIdentValue>& gridLineName);
    bool parseGridItemPositionShorthand(CSSPropertyID, bool important);
    bool parseGridTemplateRowsAndAreas(CSSValue *, bool important);
    bool parseGridTemplateShorthand(bool important);
    bool parseGridShorthand(bool important);
    bool parseGridAreaShorthand(bool important);
    bool parseGridGapShorthand(bool important);
    bool parseSingleGridAreaLonghand(GCRefPtr<CSSValue>&);
    GCRefPtr<CSSValue> parseGridTrackList();
    bool parseGridTrackRepeatFunction(CSSValueList&);
    GCRefPtr<CSSValue> parseGridTrackSize(CSSParserValueList& inputList);
    GCRefPtr<CSSPrimitiveValue> parseGridBreadth(CSSParserValue*);
    bool parseGridTemplateAreasRow(NamedGridAreaMap&, const size_t, size_t&);
    GCRefPtr<CSSValue> parseGridTemplateAreas();
    bool parseGridLineNames(CSSParserValueList&, CSSValueList&, CSSGridLineNamesValue* = nullptr);
    GCRefPtr<CSSValue> parseGridAutoFlow(CSSParserValueList&);

    bool parseLegacyPosition(CSSPropertyID, bool important);
    bool parseItemPositionOverflowPosition(CSSPropertyID, bool important);
    GCRefPtr<CSSValue> parseContentDistributionOverflowPosition();

    GCRefPtr<CSSValue> parseShapeProperty(CSSPropertyID propId);
    GCRefPtr<CSSValue> parseBasicShapeAndOrBox();
    GCRefPtr<CSSValue> parseBasicShape();
    GCRefPtr<CSSPrimitiveValue> parseShapeRadius(CSSParserValue*);

    GCRefPtr<CSSBasicShapeCircleValue> parseBasicShapeCircle(CSSParserValueList* args);
    GCRefPtr<CSSBasicShapeEllipseValue> parseBasicShapeEllipse(CSSParserValueList* args);
    GCRefPtr<CSSBasicShapePolygonValue> parseBasicShapePolygon(CSSParserValueList* args);
    GCRefPtr<CSSBasicShapeInsetValue> parseBasicShapeInset(CSSParserValueList* args);

    bool consumeFont(bool important);
    bool consumeSystemFont(bool important);

    bool consumeBorderSpacing(bool important);

    bool parseColorParameters(const CSSParserValue*, int* colorValues, bool parseAlpha);
    bool parseHSLParameters(const CSSParserValue*, double* colorValues, bool parseAlpha);
    GCRefPtr<CSSValue> parseColor(const CSSParserValue*, bool acceptQuirkyColors = false);
    bool parseColorFromValue(const CSSParserValue*, RGBA32&, bool acceptQuirkyColors = false);

    // CSS3 Parsing Routines (for properties specific to CSS3)
    bool parseBorderImageShorthand(CSSPropertyID, bool important);
    GCRefPtr<CSSValue> parseBorderImage(CSSPropertyID);
    bool parseBorderImageRepeat(GCRefPtr<CSSValue>&);
    bool parseBorderImageSlice(CSSPropertyID, GCRefPtr<CSSBorderImageSliceValue>&);
    bool parseBorderImageWidth(GCRefPtr<CSSQuadValue>&);
    bool parseBorderImageOutset(GCRefPtr<CSSQuadValue>&);
    bool parseRadii(GCRefPtr<CSSPrimitiveValue> radii[4], GCRefPtr<CSSPrimitiveValue> radii2[4], CSSParserValueList*, CSSPropertyID = CSSPropertyInvalid);

    GCRefPtr<CSSValue> parseReflect();

    bool consumeFlex(bool important);

    // Image generators
    bool parseDeprecatedGradient(CSSParserValueList*, GCRefPtr<CSSValue>&);
    bool parseDeprecatedLinearGradient(CSSParserValueList*, GCRefPtr<CSSValue>&, CSSGradientRepeat repeating);
    bool parseDeprecatedRadialGradient(CSSParserValueList*, GCRefPtr<CSSValue>&, CSSGradientRepeat repeating);
    bool parseLinearGradient(CSSParserValueList*, GCRefPtr<CSSValue>&, CSSGradientRepeat repeating);
    bool parseRadialGradient(CSSParserValueList*, GCRefPtr<CSSValue>&, CSSGradientRepeat repeating);
    bool parseGradientColorStops(CSSParserValueList*, CSSGradientValue*, bool expectComma);

    bool parseCrossfade(CSSParserValueList*, GCRefPtr<CSSValue>&);

    GCRefPtr<CSSValue> parseImageSet(CSSParserValueList*);

    GCRefPtr<CSSValueList> parseFilter();
    GCRefPtr<CSSFunctionValue> parseBuiltinFilterArguments(CSSParserValueList*, CSSValueID);

    bool parseCalculation(CSSParserValue*, ValueRange);

    bool parseGeneratedImage(CSSParserValueList*, GCRefPtr<CSSValue>&);

    GCRefPtr<CSSPrimitiveValue> createPrimitiveNumericValue(CSSParserValue*);
    GCRefPtr<CSSStringValue> createPrimitiveStringValue(CSSParserValue*);
    GCRefPtr<CSSCustomIdentValue> createPrimitiveCustomIdentValue(CSSParserValue*);

    class ImplicitScope {
        STACK_ALLOCATED();
        WTF_MAKE_NONCOPYABLE(ImplicitScope);
    public:
        ImplicitScope(CSSPropertyParser* parser)
            : m_parser(parser)
        {
            m_parser->m_implicitShorthand = true;
        }

        ~ImplicitScope()
        {
            m_parser->m_implicitShorthand = false;
        }

    private:
        CSSPropertyParser* m_parser;
    };

    class ShorthandScope {
        STACK_ALLOCATED();
    public:
        ShorthandScope(CSSPropertyParser* parser, CSSPropertyID propId) : m_parser(parser)
        {
            if (!(m_parser->m_inParseShorthand++))
                m_parser->m_currentShorthand = propId;
        }
        ~ShorthandScope()
        {
            if (!(--m_parser->m_inParseShorthand))
                m_parser->m_currentShorthand = CSSPropertyInvalid;
        }

    private:
        CSSPropertyParser* m_parser;
    };

    enum ReleaseParsedCalcValueCondition {
        ReleaseParsedCalcValue,
        DoNotReleaseParsedCalcValue
    };

    friend inline Units operator|(Units a, Units b)
    {
        return static_cast<Units>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
    }

    bool validCalculationUnit(CSSParserValue*, Units, ReleaseParsedCalcValueCondition releaseCalc = DoNotReleaseParsedCalcValue);

    bool shouldAcceptUnitLessValues(CSSParserValue*, Units, CSSParserMode);

    inline bool validUnit(CSSParserValue* value, Units unitflags, ReleaseParsedCalcValueCondition releaseCalc = DoNotReleaseParsedCalcValue) { return validUnit(value, unitflags, m_context.mode(), releaseCalc); }
    bool validUnit(CSSParserValue*, Units, CSSParserMode, ReleaseParsedCalcValueCondition releaseCalc = DoNotReleaseParsedCalcValue);

    bool parseBorderImageQuad(Units, GCRefPtr<CSSQuadValue>&);
    int colorIntFromValue(CSSParserValue*);

    bool buildBorderImageParseContext(CSSPropertyID, BorderImageParseContext&);

    bool parseDeprecatedGradientColorStop(CSSParserValue*, CSSGradientColorStop&);
    GCRefPtr<CSSValue> parseDeprecatedGradientStopColor(const CSSParserValue*);

    void commitBorderImageProperty(CSSPropertyID, const GCRefPtr<CSSValue> &, bool important);

private:
    // Inputs:
    CSSParserValueList* m_valueList;
    CSSParserTokenRange m_range;
    const CSSParserContext& m_context;

    // Outputs:
    std::vector<CSSProperty>& m_parsedProperties;

    // Locals during parsing:
    int m_inParseShorthand;
    CSSPropertyID m_currentShorthand;
    bool m_implicitShorthand;
    GCRefPtr<CSSCalcValue> m_parsedCalculation;
};

CSSPropertyID unresolvedCSSPropertyID(const CSSParserString&);
CSSValueID cssValueKeywordID(const CSSParserString&);

// TODO(rwlbuis): move to CSSPropertyParser.cpp once CSSParserToken conversion is done.
void completeBorderRadii(GCRefPtr<CSSPrimitiveValue> radii[4]);

} // namespace blink

#endif // CSSPropertyParser_h
