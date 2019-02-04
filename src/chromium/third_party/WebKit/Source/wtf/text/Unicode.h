// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: Unicode.h
// Description: Unicode Helpers
//      Author: Ziming Li
//     Created: 2019-02-03
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_UNICODE_H
#define BLINKIT_BLINK_UNICODE_H

#pragma once

// Define platform neutral 8 bit character type (L is for Latin-1).
typedef unsigned char LChar;

#if defined(OS_WIN)
typedef wchar_t UChar;
#endif

typedef int32_t UChar32;

/**
 * Helper Macros from ICU
 */
#define U_IS_BMP(c)                 ((uint32_t)(c)<=0xffff)
#define U16_LEAD(supplementary)     (UChar)(((supplementary)>>10)+0xd7c0)
#define U16_TRAIL(supplementary)    (UChar)(((supplementary)&0x3ff)|0xdc00)

namespace WTF {
namespace Unicode {

enum Direction {
    LeftToRight,
    RightToLeft,
    EuropeanNumber,
    EuropeanNumberSeparator,
    EuropeanNumberTerminator,
    ArabicNumber,
    CommonNumberSeparator,
    BlockSeparator,
    SegmentSeparator,
    WhiteSpaceNeutral,
    OtherNeutral,
    LeftToRightEmbedding,
    LeftToRightOverride,
    RightToLeftArabic,
    RightToLeftEmbedding,
    RightToLeftOverride,
    PopDirectionalFormat,
    NonSpacingMark,
    BoundaryNeutral
};

Direction direction(UChar32 c);

int umemcasecmp(const UChar *a, const UChar *b, int len);

} // namespace Unicode
} // namespace WTF

static_assert(sizeof(UChar) == 2, "UChar should be two bytes");

#endif // BLINKIT_BLINK_UNICODE_H
