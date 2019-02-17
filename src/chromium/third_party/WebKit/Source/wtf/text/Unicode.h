// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: Unicode.h
// Description: Unicode Helpers
//      Author: Ziming Li
//     Created: 2019-02-03
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_UNICODE_H
#define BLINKIT_BLINK_UNICODE_H

#pragma once

// Define platform neutral 8 bit character type (L is for Latin-1).
typedef unsigned char LChar;

#include <unicode/uchar.h>

namespace WTF {
namespace Unicode {

enum Direction {
    LeftToRight = U_LEFT_TO_RIGHT,
    RightToLeft = U_RIGHT_TO_LEFT,
    EuropeanNumber = U_EUROPEAN_NUMBER,
    EuropeanNumberSeparator = U_EUROPEAN_NUMBER_SEPARATOR,
    EuropeanNumberTerminator = U_EUROPEAN_NUMBER_TERMINATOR,
    ArabicNumber = U_ARABIC_NUMBER,
    CommonNumberSeparator = U_COMMON_NUMBER_SEPARATOR,
    BlockSeparator = U_BLOCK_SEPARATOR,
    SegmentSeparator = U_SEGMENT_SEPARATOR,
    WhiteSpaceNeutral = U_WHITE_SPACE_NEUTRAL,
    OtherNeutral = U_OTHER_NEUTRAL,
    LeftToRightEmbedding = U_LEFT_TO_RIGHT_EMBEDDING,
    LeftToRightOverride = U_LEFT_TO_RIGHT_OVERRIDE,
    RightToLeftArabic = U_RIGHT_TO_LEFT_ARABIC,
    RightToLeftEmbedding = U_RIGHT_TO_LEFT_EMBEDDING,
    RightToLeftOverride = U_RIGHT_TO_LEFT_OVERRIDE,
    PopDirectionalFormat = U_POP_DIRECTIONAL_FORMAT,
    NonSpacingMark = U_DIR_NON_SPACING_MARK,
    BoundaryNeutral = U_BOUNDARY_NEUTRAL
};

enum DecompositionType {
    DecompositionNone = U_DT_NONE,
    DecompositionCanonical = U_DT_CANONICAL,
    DecompositionCompat = U_DT_COMPAT,
    DecompositionCircle = U_DT_CIRCLE,
    DecompositionFinal = U_DT_FINAL,
    DecompositionFont = U_DT_FONT,
    DecompositionFraction = U_DT_FRACTION,
    DecompositionInitial = U_DT_INITIAL,
    DecompositionIsolated = U_DT_ISOLATED,
    DecompositionMedial = U_DT_MEDIAL,
    DecompositionNarrow = U_DT_NARROW,
    DecompositionNoBreak = U_DT_NOBREAK,
    DecompositionSmall = U_DT_SMALL,
    DecompositionSquare = U_DT_SQUARE,
    DecompositionSub = U_DT_SUB,
    DecompositionSuper = U_DT_SUPER,
    DecompositionVertical = U_DT_VERTICAL,
    DecompositionWide = U_DT_WIDE,
};

enum CharCategory {
    NoCategory = 0,
    Other_NotAssigned = U_MASK(U_GENERAL_OTHER_TYPES),
    Letter_Uppercase = U_MASK(U_UPPERCASE_LETTER),
    Letter_Lowercase = U_MASK(U_LOWERCASE_LETTER),
    Letter_Titlecase = U_MASK(U_TITLECASE_LETTER),
    Letter_Modifier = U_MASK(U_MODIFIER_LETTER),
    Letter_Other = U_MASK(U_OTHER_LETTER),

    Mark_NonSpacing = U_MASK(U_NON_SPACING_MARK),
    Mark_Enclosing = U_MASK(U_ENCLOSING_MARK),
    Mark_SpacingCombining = U_MASK(U_COMBINING_SPACING_MARK),

    Number_DecimalDigit = U_MASK(U_DECIMAL_DIGIT_NUMBER),
    Number_Letter = U_MASK(U_LETTER_NUMBER),
    Number_Other = U_MASK(U_OTHER_NUMBER),

    Separator_Space = U_MASK(U_SPACE_SEPARATOR),
    Separator_Line = U_MASK(U_LINE_SEPARATOR),
    Separator_Paragraph = U_MASK(U_PARAGRAPH_SEPARATOR),

    Other_Control = U_MASK(U_CONTROL_CHAR),
    Other_Format = U_MASK(U_FORMAT_CHAR),
    Other_PrivateUse = U_MASK(U_PRIVATE_USE_CHAR),
    Other_Surrogate = U_MASK(U_SURROGATE),

    Punctuation_Dash = U_MASK(U_DASH_PUNCTUATION),
    Punctuation_Open = U_MASK(U_START_PUNCTUATION),
    Punctuation_Close = U_MASK(U_END_PUNCTUATION),
    Punctuation_Connector = U_MASK(U_CONNECTOR_PUNCTUATION),
    Punctuation_Other = U_MASK(U_OTHER_PUNCTUATION),

    Symbol_Math = U_MASK(U_MATH_SYMBOL),
    Symbol_Currency = U_MASK(U_CURRENCY_SYMBOL),
    Symbol_Modifier = U_MASK(U_MODIFIER_SYMBOL),
    Symbol_Other = U_MASK(U_OTHER_SYMBOL),

    Punctuation_InitialQuote = U_MASK(U_INITIAL_PUNCTUATION),
    Punctuation_FinalQuote = U_MASK(U_FINAL_PUNCTUATION)
};

CharCategory category(UChar32 c);

DecompositionType decompositionType(UChar32 c);

Direction direction(UChar32 c);

UChar32 foldCase(UChar32 c);

bool hasLineBreakingPropertyComplexContext(UChar32 c);

bool isPrintableChar(UChar32 c);

UChar32 toTitleCase(UChar32 c);

int umemcasecmp(const UChar *a, const UChar *b, int len);

} // namespace Unicode
} // namespace WTF

static_assert(sizeof(UChar) == 2, "UChar should be two bytes");

#endif // BLINKIT_BLINK_UNICODE_H
