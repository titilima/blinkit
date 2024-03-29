
// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSPropertyNames_h
#define CSSPropertyNames_h

#include "core/css/parser/CSSParserMode.h"
#include "wtf/HashFunctions.h"
#include "wtf/HashTraits.h"
#include <string.h>

namespace WTF {
class AtomicString;
class String;
}

namespace blink {

enum CSSPropertyID {
    CSSPropertyInvalid = 0,
    CSSPropertyVariable = 1,
    CSSPropertyColor = 2,
    CSSPropertyDirection = 3,
    CSSPropertyFontFamily = 4,
    CSSPropertyFontKerning = 5,
    CSSPropertyFontSize = 6,
    CSSPropertyFontSizeAdjust = 7,
    CSSPropertyFontStretch = 8,
    CSSPropertyFontStyle = 9,
    CSSPropertyFontVariant = 10,
    CSSPropertyFontVariantLigatures = 11,
    CSSPropertyFontWeight = 12,
    CSSPropertyFontFeatureSettings = 13,
    CSSPropertyWebkitFontSmoothing = 14,
    CSSPropertyWebkitLocale = 15,
    CSSPropertyTextOrientation = 16,
    CSSPropertyWebkitTextOrientation = 17,
    CSSPropertyWritingMode = 18,
    CSSPropertyWebkitWritingMode = 19,
    CSSPropertyTextRendering = 20,
    CSSPropertyZoom = 21,
    CSSPropertyAlignContent = 22,
    CSSPropertyAlignItems = 23,
    CSSPropertyAlignSelf = 24,
    CSSPropertyAnimationDelay = 25,
    CSSPropertyAnimationDirection = 26,
    CSSPropertyAnimationDuration = 27,
    CSSPropertyAnimationFillMode = 28,
    CSSPropertyAnimationIterationCount = 29,
    CSSPropertyAnimationName = 30,
    CSSPropertyAnimationPlayState = 31,
    CSSPropertyAnimationTimingFunction = 32,
    CSSPropertyBackdropFilter = 33,
    CSSPropertyBackfaceVisibility = 34,
    CSSPropertyBackgroundAttachment = 35,
    CSSPropertyBackgroundBlendMode = 36,
    CSSPropertyBackgroundClip = 37,
    CSSPropertyBackgroundColor = 38,
    CSSPropertyBackgroundImage = 39,
    CSSPropertyBackgroundOrigin = 40,
    CSSPropertyBackgroundPositionX = 41,
    CSSPropertyBackgroundPositionY = 42,
    CSSPropertyBackgroundRepeatX = 43,
    CSSPropertyBackgroundRepeatY = 44,
    CSSPropertyBackgroundSize = 45,
    CSSPropertyBorderBottomColor = 46,
    CSSPropertyBorderBottomLeftRadius = 47,
    CSSPropertyBorderBottomRightRadius = 48,
    CSSPropertyBorderBottomStyle = 49,
    CSSPropertyBorderBottomWidth = 50,
    CSSPropertyBorderCollapse = 51,
    CSSPropertyBorderImageOutset = 52,
    CSSPropertyBorderImageRepeat = 53,
    CSSPropertyBorderImageSlice = 54,
    CSSPropertyBorderImageSource = 55,
    CSSPropertyBorderImageWidth = 56,
    CSSPropertyBorderLeftColor = 57,
    CSSPropertyBorderLeftStyle = 58,
    CSSPropertyBorderLeftWidth = 59,
    CSSPropertyBorderRightColor = 60,
    CSSPropertyBorderRightStyle = 61,
    CSSPropertyBorderRightWidth = 62,
    CSSPropertyBorderTopColor = 63,
    CSSPropertyBorderTopLeftRadius = 64,
    CSSPropertyBorderTopRightRadius = 65,
    CSSPropertyBorderTopStyle = 66,
    CSSPropertyBorderTopWidth = 67,
    CSSPropertyBottom = 68,
    CSSPropertyBoxShadow = 69,
    CSSPropertyBoxSizing = 70,
    CSSPropertyCaptionSide = 71,
    CSSPropertyClear = 72,
    CSSPropertyClip = 73,
    CSSPropertyColumnFill = 74,
    CSSPropertyContain = 75,
    CSSPropertyContent = 76,
    CSSPropertyCounterIncrement = 77,
    CSSPropertyCounterReset = 78,
    CSSPropertyCursor = 79,
    CSSPropertyDisplay = 80,
    CSSPropertyEmptyCells = 81,
    CSSPropertyFlexBasis = 82,
    CSSPropertyFlexDirection = 83,
    CSSPropertyFlexGrow = 84,
    CSSPropertyFlexShrink = 85,
    CSSPropertyFlexWrap = 86,
    CSSPropertyFloat = 87,
    CSSPropertyGridAutoColumns = 88,
    CSSPropertyGridAutoFlow = 89,
    CSSPropertyGridAutoRows = 90,
    CSSPropertyGridColumnEnd = 91,
    CSSPropertyGridColumnGap = 92,
    CSSPropertyGridColumnStart = 93,
    CSSPropertyGridRowEnd = 94,
    CSSPropertyGridRowGap = 95,
    CSSPropertyGridRowStart = 96,
    CSSPropertyGridTemplateAreas = 97,
    CSSPropertyGridTemplateColumns = 98,
    CSSPropertyGridTemplateRows = 99,
    CSSPropertyHeight = 100,
    CSSPropertyImageRendering = 101,
    CSSPropertyImageOrientation = 102,
    CSSPropertyIsolation = 103,
    CSSPropertyJustifyContent = 104,
    CSSPropertyJustifyItems = 105,
    CSSPropertyJustifySelf = 106,
    CSSPropertyLeft = 107,
    CSSPropertyLetterSpacing = 108,
    CSSPropertyLineHeight = 109,
    CSSPropertyListStyleImage = 110,
    CSSPropertyListStylePosition = 111,
    CSSPropertyListStyleType = 112,
    CSSPropertyMarginBottom = 113,
    CSSPropertyMarginLeft = 114,
    CSSPropertyMarginRight = 115,
    CSSPropertyMarginTop = 116,
    CSSPropertyMaskSourceType = 117,
    CSSPropertyMaxHeight = 118,
    CSSPropertyMaxWidth = 119,
    CSSPropertyMinHeight = 120,
    CSSPropertyMinWidth = 121,
    CSSPropertyMixBlendMode = 122,
    CSSPropertyMotionOffset = 123,
    CSSPropertyMotionPath = 124,
    CSSPropertyMotionRotation = 125,
    CSSPropertyObjectFit = 126,
    CSSPropertyObjectPosition = 127,
    CSSPropertyOpacity = 128,
    CSSPropertyOrder = 129,
    CSSPropertyOrphans = 130,
    CSSPropertyOutlineColor = 131,
    CSSPropertyOutlineOffset = 132,
    CSSPropertyOutlineStyle = 133,
    CSSPropertyOutlineWidth = 134,
    CSSPropertyOverflowWrap = 135,
    CSSPropertyOverflowX = 136,
    CSSPropertyOverflowY = 137,
    CSSPropertyPaddingBottom = 138,
    CSSPropertyPaddingLeft = 139,
    CSSPropertyPaddingRight = 140,
    CSSPropertyPaddingTop = 141,
    CSSPropertyPageBreakAfter = 142,
    CSSPropertyPageBreakBefore = 143,
    CSSPropertyPageBreakInside = 144,
    CSSPropertyPerspective = 145,
    CSSPropertyPerspectiveOrigin = 146,
    CSSPropertyPointerEvents = 147,
    CSSPropertyPosition = 148,
    CSSPropertyQuotes = 149,
    CSSPropertyResize = 150,
    CSSPropertyRight = 151,
    CSSPropertyScrollBehavior = 152,
    CSSPropertyScrollSnapType = 153,
    CSSPropertyScrollSnapPointsX = 154,
    CSSPropertyScrollSnapPointsY = 155,
    CSSPropertyScrollSnapDestination = 156,
    CSSPropertyScrollSnapCoordinate = 157,
    CSSPropertyShapeImageThreshold = 158,
    CSSPropertyShapeMargin = 159,
    CSSPropertyShapeOutside = 160,
    CSSPropertySize = 161,
    CSSPropertySpeak = 162,
    CSSPropertyTableLayout = 163,
    CSSPropertyTabSize = 164,
    CSSPropertyTextAlign = 165,
    CSSPropertyTextAlignLast = 166,
    CSSPropertyTextCombineUpright = 167,
    CSSPropertyTextDecoration = 168,
    CSSPropertyTextDecorationColor = 169,
    CSSPropertyTextDecorationLine = 170,
    CSSPropertyTextDecorationStyle = 171,
    CSSPropertyTextIndent = 172,
    CSSPropertyTextJustify = 173,
    CSSPropertyTextOverflow = 174,
    CSSPropertyTextShadow = 175,
    CSSPropertyTextTransform = 176,
    CSSPropertyTextUnderlinePosition = 177,
    CSSPropertyTop = 178,
    CSSPropertyTouchAction = 179,
    CSSPropertyTransform = 180,
    CSSPropertyTransformOrigin = 181,
    CSSPropertyTransformStyle = 182,
    CSSPropertyTranslate = 183,
    CSSPropertyRotate = 184,
    CSSPropertyScale = 185,
    CSSPropertyTransitionDelay = 186,
    CSSPropertyTransitionDuration = 187,
    CSSPropertyTransitionProperty = 188,
    CSSPropertyTransitionTimingFunction = 189,
    CSSPropertyUnicodeBidi = 190,
    CSSPropertyVerticalAlign = 191,
    CSSPropertyVisibility = 192,
    CSSPropertyWebkitAppearance = 193,
    CSSPropertyWebkitAppRegion = 194,
    CSSPropertyWebkitBackgroundClip = 195,
    CSSPropertyWebkitBackgroundComposite = 196,
    CSSPropertyWebkitBackgroundOrigin = 197,
    CSSPropertyWebkitBorderHorizontalSpacing = 198,
    CSSPropertyWebkitBorderImage = 199,
    CSSPropertyWebkitBorderVerticalSpacing = 200,
    CSSPropertyWebkitBoxAlign = 201,
    CSSPropertyWebkitBoxDecorationBreak = 202,
    CSSPropertyWebkitBoxDirection = 203,
    CSSPropertyWebkitBoxFlex = 204,
    CSSPropertyWebkitBoxFlexGroup = 205,
    CSSPropertyWebkitBoxLines = 206,
    CSSPropertyWebkitBoxOrdinalGroup = 207,
    CSSPropertyWebkitBoxOrient = 208,
    CSSPropertyWebkitBoxPack = 209,
    CSSPropertyWebkitBoxReflect = 210,
    CSSPropertyWebkitClipPath = 211,
    CSSPropertyWebkitColumnBreakAfter = 212,
    CSSPropertyWebkitColumnBreakBefore = 213,
    CSSPropertyWebkitColumnBreakInside = 214,
    CSSPropertyWebkitColumnCount = 215,
    CSSPropertyWebkitColumnGap = 216,
    CSSPropertyWebkitColumnRuleColor = 217,
    CSSPropertyWebkitColumnRuleStyle = 218,
    CSSPropertyWebkitColumnRuleWidth = 219,
    CSSPropertyWebkitColumnSpan = 220,
    CSSPropertyWebkitColumnWidth = 221,
    CSSPropertyWebkitFilter = 222,
    CSSPropertyWebkitHighlight = 223,
    CSSPropertyWebkitHyphenateCharacter = 224,
    CSSPropertyWebkitLineBreak = 225,
    CSSPropertyWebkitLineClamp = 226,
    CSSPropertyWebkitMarginAfterCollapse = 227,
    CSSPropertyWebkitMarginBeforeCollapse = 228,
    CSSPropertyWebkitMarginBottomCollapse = 229,
    CSSPropertyWebkitMarginTopCollapse = 230,
    CSSPropertyWebkitMaskBoxImageOutset = 231,
    CSSPropertyWebkitMaskBoxImageRepeat = 232,
    CSSPropertyWebkitMaskBoxImageSlice = 233,
    CSSPropertyWebkitMaskBoxImageSource = 234,
    CSSPropertyWebkitMaskBoxImageWidth = 235,
    CSSPropertyWebkitMaskClip = 236,
    CSSPropertyWebkitMaskComposite = 237,
    CSSPropertyWebkitMaskImage = 238,
    CSSPropertyWebkitMaskOrigin = 239,
    CSSPropertyWebkitMaskPositionX = 240,
    CSSPropertyWebkitMaskPositionY = 241,
    CSSPropertyWebkitMaskRepeatX = 242,
    CSSPropertyWebkitMaskRepeatY = 243,
    CSSPropertyWebkitMaskSize = 244,
    CSSPropertyWebkitPerspectiveOriginX = 245,
    CSSPropertyWebkitPerspectiveOriginY = 246,
    CSSPropertyWebkitPrintColorAdjust = 247,
    CSSPropertyWebkitRtlOrdering = 248,
    CSSPropertyWebkitRubyPosition = 249,
    CSSPropertyWebkitTapHighlightColor = 250,
    CSSPropertyWebkitTextCombine = 251,
    CSSPropertyWebkitTextEmphasisColor = 252,
    CSSPropertyWebkitTextEmphasisPosition = 253,
    CSSPropertyWebkitTextEmphasisStyle = 254,
    CSSPropertyWebkitTextFillColor = 255,
    CSSPropertyWebkitTextSecurity = 256,
    CSSPropertyWebkitTextStrokeColor = 257,
    CSSPropertyWebkitTextStrokeWidth = 258,
    CSSPropertyWebkitTransformOriginX = 259,
    CSSPropertyWebkitTransformOriginY = 260,
    CSSPropertyWebkitTransformOriginZ = 261,
    CSSPropertyWebkitUserDrag = 262,
    CSSPropertyWebkitUserModify = 263,
    CSSPropertyWebkitUserSelect = 264,
    CSSPropertyWhiteSpace = 265,
    CSSPropertyWidows = 266,
    CSSPropertyWidth = 267,
    CSSPropertyWillChange = 268,
    CSSPropertyWordBreak = 269,
    CSSPropertyWordSpacing = 270,
    CSSPropertyWordWrap = 271,
    CSSPropertyZIndex = 272,
    CSSPropertyWebkitBorderEndColor = 273,
    CSSPropertyWebkitBorderEndStyle = 274,
    CSSPropertyWebkitBorderEndWidth = 275,
    CSSPropertyWebkitBorderStartColor = 276,
    CSSPropertyWebkitBorderStartStyle = 277,
    CSSPropertyWebkitBorderStartWidth = 278,
    CSSPropertyWebkitBorderBeforeColor = 279,
    CSSPropertyWebkitBorderBeforeStyle = 280,
    CSSPropertyWebkitBorderBeforeWidth = 281,
    CSSPropertyWebkitBorderAfterColor = 282,
    CSSPropertyWebkitBorderAfterStyle = 283,
    CSSPropertyWebkitBorderAfterWidth = 284,
    CSSPropertyWebkitMarginEnd = 285,
    CSSPropertyWebkitMarginStart = 286,
    CSSPropertyWebkitMarginBefore = 287,
    CSSPropertyWebkitMarginAfter = 288,
    CSSPropertyWebkitPaddingEnd = 289,
    CSSPropertyWebkitPaddingStart = 290,
    CSSPropertyWebkitPaddingBefore = 291,
    CSSPropertyWebkitPaddingAfter = 292,
    CSSPropertyWebkitLogicalWidth = 293,
    CSSPropertyWebkitLogicalHeight = 294,
    CSSPropertyWebkitMinLogicalWidth = 295,
    CSSPropertyWebkitMinLogicalHeight = 296,
    CSSPropertyWebkitMaxLogicalWidth = 297,
    CSSPropertyWebkitMaxLogicalHeight = 298,
    CSSPropertyAll = 299,
    CSSPropertyFontDisplay = 300,
    CSSPropertyMaxZoom = 301,
    CSSPropertyMinZoom = 302,
    CSSPropertyOrientation = 303,
    CSSPropertyPage = 304,
    CSSPropertySrc = 305,
    CSSPropertyUnicodeRange = 306,
    CSSPropertyUserZoom = 307,
    CSSPropertyWebkitFontSizeDelta = 308,
    CSSPropertyWebkitTextDecorationsInEffect = 309,
    CSSPropertyAnimation = 310,
    CSSPropertyBackground = 311,
    CSSPropertyBackgroundPosition = 312,
    CSSPropertyBackgroundRepeat = 313,
    CSSPropertyBorder = 314,
    CSSPropertyBorderBottom = 315,
    CSSPropertyBorderColor = 316,
    CSSPropertyBorderImage = 317,
    CSSPropertyBorderLeft = 318,
    CSSPropertyBorderRadius = 319,
    CSSPropertyBorderRight = 320,
    CSSPropertyBorderSpacing = 321,
    CSSPropertyBorderStyle = 322,
    CSSPropertyBorderTop = 323,
    CSSPropertyBorderWidth = 324,
    CSSPropertyFlex = 325,
    CSSPropertyFlexFlow = 326,
    CSSPropertyFont = 327,
    CSSPropertyGrid = 328,
    CSSPropertyGridArea = 329,
    CSSPropertyGridColumn = 330,
    CSSPropertyGridGap = 331,
    CSSPropertyGridRow = 332,
    CSSPropertyGridTemplate = 333,
    CSSPropertyListStyle = 334,
    CSSPropertyMargin = 335,
    CSSPropertyMotion = 336,
    CSSPropertyOutline = 337,
    CSSPropertyOverflow = 338,
    CSSPropertyPadding = 339,
    CSSPropertyTransition = 340,
    CSSPropertyWebkitBorderAfter = 341,
    CSSPropertyWebkitBorderBefore = 342,
    CSSPropertyWebkitBorderEnd = 343,
    CSSPropertyWebkitBorderStart = 344,
    CSSPropertyWebkitColumnRule = 345,
    CSSPropertyWebkitColumns = 346,
    CSSPropertyWebkitMarginCollapse = 347,
    CSSPropertyWebkitMask = 348,
    CSSPropertyWebkitMaskBoxImage = 349,
    CSSPropertyWebkitMaskPosition = 350,
    CSSPropertyWebkitMaskRepeat = 351,
    CSSPropertyWebkitTextEmphasis = 352,
    CSSPropertyWebkitTextStroke = 353,
    CSSPropertyAliasEpubCaptionSide = 583,
    CSSPropertyAliasEpubTextCombine = 763,
    CSSPropertyAliasEpubTextEmphasis = 864,
    CSSPropertyAliasEpubTextEmphasisColor = 764,
    CSSPropertyAliasEpubTextEmphasisStyle = 766,
    CSSPropertyAliasEpubTextOrientation = 529,
    CSSPropertyAliasEpubTextTransform = 688,
    CSSPropertyAliasEpubWordBreak = 781,
    CSSPropertyAliasEpubWritingMode = 531,
    CSSPropertyAliasWebkitAlignContent = 534,
    CSSPropertyAliasWebkitAlignItems = 535,
    CSSPropertyAliasWebkitAlignSelf = 536,
    CSSPropertyAliasWebkitAnimation = 822,
    CSSPropertyAliasWebkitAnimationDelay = 537,
    CSSPropertyAliasWebkitAnimationDirection = 538,
    CSSPropertyAliasWebkitAnimationDuration = 539,
    CSSPropertyAliasWebkitAnimationFillMode = 540,
    CSSPropertyAliasWebkitAnimationIterationCount = 541,
    CSSPropertyAliasWebkitAnimationName = 542,
    CSSPropertyAliasWebkitAnimationPlayState = 543,
    CSSPropertyAliasWebkitAnimationTimingFunction = 544,
    CSSPropertyAliasWebkitBackfaceVisibility = 546,
    CSSPropertyAliasWebkitBackgroundSize = 557,
    CSSPropertyAliasWebkitBorderBottomLeftRadius = 559,
    CSSPropertyAliasWebkitBorderBottomRightRadius = 560,
    CSSPropertyAliasWebkitBorderRadius = 831,
    CSSPropertyAliasWebkitBorderTopLeftRadius = 576,
    CSSPropertyAliasWebkitBorderTopRightRadius = 577,
    CSSPropertyAliasWebkitBoxShadow = 581,
    CSSPropertyAliasWebkitBoxSizing = 582,
    CSSPropertyAliasWebkitFlex = 837,
    CSSPropertyAliasWebkitFlexBasis = 594,
    CSSPropertyAliasWebkitFlexDirection = 595,
    CSSPropertyAliasWebkitFlexFlow = 838,
    CSSPropertyAliasWebkitFlexGrow = 596,
    CSSPropertyAliasWebkitFlexShrink = 597,
    CSSPropertyAliasWebkitFlexWrap = 598,
    CSSPropertyAliasWebkitFontFeatureSettings = 525,
    CSSPropertyAliasWebkitJustifyContent = 616,
    CSSPropertyAliasWebkitOpacity = 640,
    CSSPropertyAliasWebkitOrder = 641,
    CSSPropertyAliasWebkitPerspective = 657,
    CSSPropertyAliasWebkitPerspectiveOrigin = 658,
    CSSPropertyAliasWebkitShapeImageThreshold = 670,
    CSSPropertyAliasWebkitShapeMargin = 671,
    CSSPropertyAliasWebkitShapeOutside = 672,
    CSSPropertyAliasWebkitTransform = 692,
    CSSPropertyAliasWebkitTransformOrigin = 693,
    CSSPropertyAliasWebkitTransformStyle = 694,
    CSSPropertyAliasWebkitTransition = 852,
    CSSPropertyAliasWebkitTransitionDelay = 698,
    CSSPropertyAliasWebkitTransitionDuration = 699,
    CSSPropertyAliasWebkitTransitionProperty = 700,
    CSSPropertyAliasWebkitTransitionTimingFunction = 701,
};

const int firstCSSProperty = 2;
const int numCSSProperties = 352;
const int lastCSSProperty = 353;
const int lastUnresolvedCSSProperty = 864;
const size_t maxCSSPropertyNameLength = 40;

const char* getPropertyName(CSSPropertyID);
const WTF::AtomicString& getPropertyNameAtomicString(CSSPropertyID);
WTF::String getPropertyNameString(CSSPropertyID);
WTF::String getJSPropertyName(CSSPropertyID);

inline CSSPropertyID convertToCSSPropertyID(int value)
{
    ASSERT((value >= firstCSSProperty && value <= lastCSSProperty) || value == CSSPropertyInvalid || value == CSSPropertyVariable);
    return static_cast<CSSPropertyID>(value);
}

inline CSSPropertyID resolveCSSPropertyID(CSSPropertyID id)
{
    return convertToCSSPropertyID(id & ~512);
}

inline bool isPropertyAlias(CSSPropertyID id) { return id & 512; }

CSSPropertyID unresolvedCSSPropertyID(const WTF::String&);

CSSPropertyID cssPropertyID(const WTF::String&);

} // namespace blink

namespace WTF {
template<> struct DefaultHash<blink::CSSPropertyID> { typedef IntHash<unsigned> Hash; };
template<> struct HashTraits<blink::CSSPropertyID> : GenericHashTraits<blink::CSSPropertyID> {
    static const bool emptyValueIsZero = true;
    static void constructDeletedValue(blink::CSSPropertyID& slot, bool) { slot = static_cast<blink::CSSPropertyID>(blink::lastUnresolvedCSSProperty + 1); }
    static bool isDeletedValue(blink::CSSPropertyID value) { return value == (blink::lastUnresolvedCSSProperty + 1); }
};
}

namespace std {
template<>
struct hash<blink::CSSPropertyID>
{
    size_t operator()(blink::CSSPropertyID id) const noexcept
    {
        return static_cast<size_t>(id);
    }
};
}

#endif // CSSPropertyNames_h
