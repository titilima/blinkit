// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: unicode_icu.cpp
// Description: Unicode Helpers
//      Author: Ziming Li
//     Created: 2020-12-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "unicode.h"

#include <unicode/uchar.h>
#include <unicode/ustring.h>

namespace WTF {
namespace Unicode {

CharDecompositionType DecompositionType(UChar32 c)
{
    ASSERT(false); // BKTODO:
    return kDecompositionNone;
}

CharDirection Direction(UChar32 c)
{
    return static_cast<CharDirection>(u_charDirection(c));
}

UChar32 FoldCase(UChar32 c)
{
    return u_foldCase(c, U_FOLD_CASE_DEFAULT);
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
