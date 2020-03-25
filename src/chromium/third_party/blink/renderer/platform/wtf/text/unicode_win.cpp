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

#include <Windows.h>

namespace WTF {
namespace Unicode {

CharDecompositionType DecompositionType(UChar32 c)
{
    ASSERT(false); // BKTODO:
    return kDecompositionNone;
}

CharDirection Direction(UChar32 c)
{
    ASSERT(false); // BKTODO:
    return kLeftToRight;
}

UChar32 FoldCase(UChar32 c)
{
    wchar_t wch = static_cast<wchar_t>(c);
    LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_LINGUISTIC_CASING | LCMAP_LOWERCASE, &wch, 1, &wch, 1);
    return wch;
}

int FoldCase(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    ASSERT(false); // BKTODO:
    return 0;
}

UChar32 ToLower(UChar32 c)
{
    ASSERT(false); // BKTODO:
    return c;
}

UChar32 ToUpper(UChar32 c)
{
    ASSERT(false); // BKTODO:
    return c;
}

int ToLower(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    ASSERT(false); // BKTODO:
    return 0;
}

int ToUpper(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    ASSERT(false); // BKTODO:
    return 0;
}

int Umemcasecmp(const UChar *a, const UChar *b, int len)
{
    ASSERT(false); // BKTODO:
    return 0;
}

}  // namespace Unicode
}  // namespace WTF
