// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: unicode_win.cpp
// Description: Unicode Helpers
//      Author: Ziming Li
//     Created: 2018-12-09
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include <Windows.h>
#include "wtf/text/Unicode.h"

namespace WTF {
namespace Unicode {

DecompositionType decompositionType(UChar32 c)
{
    assert(false); // BKTODO:
    return DecompositionNone;
}

int foldCase(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    int n = LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_LINGUISTIC_CASING | LCMAP_LOWERCASE, src, srcLength,
        result, resultLength);
    *error = n > 0;
    return n;
}

UChar32 foldCase(UChar32 c)
{
    wchar_t wch = static_cast<wchar_t>(c);
    LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_LINGUISTIC_CASING | LCMAP_LOWERCASE, &wch, 1, &wch, 1);
    return wch;
}

UChar32 mirroredChar(UChar32 c)
{
    assert(false); // BKTODO:
    return false;
}

int toLower(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    assert(false); // BKTODO:
    return 0;
}

UChar32 toLower(UChar32 c)
{
    assert(false); // BKTODO:
    return 0;
}

UChar32 toTitleCase(UChar32 c)
{
    assert(false); // BKTODO:
    return 0;
}

int toUpper(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    assert(false); // BKTODO:
    return 0;
}

UChar32 toUpper(UChar32 c)
{
    assert(false); // BKTODO:
    return 0;
}

int umemcasecmp(const UChar *a, const UChar *b, int len)
{
    assert(false); // BKTODO:
    return 0;
}

} // namespace Unicode
} // namespace WTF
