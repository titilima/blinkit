// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: unicode.cpp
// Description: Unicode Helpers
//      Author: Ziming Li
//     Created: 2019-03-08
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "wtf/text/Unicode.h"

namespace WTF {
namespace Unicode {

CharCategory category(UChar32 c)
{
    return static_cast<CharCategory>(U_GET_GC_MASK(c));
}

Direction direction(UChar32 c)
{
    return static_cast<Direction>(u_charDirection(c));
}

bool isAlphanumeric(UChar32 c)
{
    return u_isalnum(c);
}

bool isPrintableChar(UChar32 c)
{
    return !!u_isprint(c);
}

bool isPunct(UChar32 c)
{
    return !!u_ispunct(c);
}

} // namespace Unicode
} // namespace WTF
