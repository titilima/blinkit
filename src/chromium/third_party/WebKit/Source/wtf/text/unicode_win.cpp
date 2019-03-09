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

#include "wtf/text/Unicode.h"

namespace WTF {
namespace Unicode {

DecompositionType decompositionType(UChar32 c)
{
    assert(false); // BKTODO:
    return DecompositionNone;
}

Direction direction(UChar32 c)
{
    assert(false); // BKTODO:
    return LeftToRight;
}

int foldCase(UChar *result, int resultLength, const UChar *src, int srcLength, bool *error)
{
    assert(false); // BKTODO:
    return 0;
}

UChar32 foldCase(UChar32 c)
{
    assert(false); // BKTODO:
    return 0;
}

bool hasLineBreakingPropertyComplexContext(UChar32 c)
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
