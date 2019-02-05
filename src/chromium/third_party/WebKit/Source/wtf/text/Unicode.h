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

#include <unicode/uchar.h>

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
