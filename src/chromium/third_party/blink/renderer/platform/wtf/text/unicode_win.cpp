// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: unicode.cpp
// Description: Unicode Helpers
//      Author: Ziming Li
//     Created: 2019-10-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "unicode.h"

namespace WTF {
namespace Unicode {

CharDirection Direction(UChar32 c)
{
    assert(false); // BKTODO:
    return kLeftToRight;
}

UChar32 FoldCase(UChar32 c)
{
    assert(false); // BKTODO:
    return c;
}

int FoldCase(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    assert(false); // BKTODO:
    return 0;
}

UChar32 ToLower(UChar32 c)
{
    assert(false); // BKTODO:
    return c;
}

UChar32 ToUpper(UChar32 c)
{
    assert(false); // BKTODO:
    return c;
}

int ToLower(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    assert(false); // BKTODO:
    return 0;
}

int ToUpper(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    assert(false); // BKTODO:
    return 0;
}

int Umemcasecmp(const UChar *a, const UChar *b, int len)
{
    assert(false); // BKTODO:
    return 0;
}

}  // namespace Unicode
}  // namespace WTF
